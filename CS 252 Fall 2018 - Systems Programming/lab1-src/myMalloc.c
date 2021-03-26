#include <errno.h>
#include <pthread.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "myMalloc.h"
#include "printing.h"

/* Due to the way assert() prints error messges we use out own assert function
 * for deteminism when testing assertions
 */
#ifdef TEST_ASSERT
  inline static void assert(int e) {
    if (!e) {
      const char * msg = "Assertion Failed!\n";
      write(2, msg, strlen(msg));
      exit(1);
      }
  }
#else
  #include <assert.h>
#endif

/*
 * Mutex to ensure thread safety for the freelist
 */
static pthread_mutex_t mutex;

/*
 * Array of sentinel nodes for the freelists
 */
header freelistSentinels[N_LISTS];

/*
 * Pointer to the second fencepost in the most recently allocated chunk from
 * the OS. Used for coalescing chunks
 */
header * lastFencePost;

/*
 * Pointer to maintian the base of the heap to allow printing based on the
 * distance from the base of the heap
 */ 
void * base;

/*
 * List of chunks allocated by  the OS for printing boundary tags
 */
header * osChunkList [MAX_OS_CHUNKS];
size_t numOsChunks = 0;

/*
 * direct the compiler to run the init function before running main
 * this allows initialization of required globals
 */
static void init (void) __attribute__ ((constructor));

// Helper functions for manipulating pointers to headers
static inline header * get_header_from_offset(void * ptr, ptrdiff_t off);
static inline header * get_left_header(header * h);
static inline header * ptr_to_header(void * p);

// Helper functions for allocating more memory from the OS
static inline void initialize_fencepost(header * fp, size_t left_size);
static inline void insert_os_chunk(header * hdr);
static inline void insert_fenceposts(void * raw_mem, size_t size);
static header * allocate_chunk(size_t size);

// Helper functions for freeing a block
static inline void deallocate_object(void * p);

// Helper functions for allocating a block
static inline header * allocate_object(size_t raw_size);

// Helper functions for verifying that the data structures are structurally 
// valid
static inline header * detect_cycles();
static inline header * verify_pointers();
static inline bool verify_freelist();
static inline header * verify_chunk(header * chunk);
static inline bool verify_tags();

static void init();

static bool isMallocInitialized;

/**
 * @brief Helper function to retrieve a header pointer from a pointer and an 
 *        offset
 *
 * @param ptr base pointer
 * @param off number of bytes from base pointer where header is located
 *
 * @return a pointer to a header offset bytes from pointer
 */
static inline header * get_header_from_offset(void * ptr, ptrdiff_t off) {
	return (header *)((char *) ptr + off);
}

/**
 * @brief Helper function to get the header to the right of a given header
 *
 * @param h original header
 *
 * @return header to the right of h
 */
header * get_right_header(header * h) {
	return get_header_from_offset(h, get_size(h));
}

/**
 * @brief Helper function to get the header to the left of a given header
 *
 * @param h original header
 *
 * @return header to the right of h
 */
inline static header * get_left_header(header * h) {
  return get_header_from_offset(h, -h->left_size);
}

/**
 * @brief Fenceposts are marked as always allocated and may need to have
 * a left object size to ensure coalescing happens properly
 *
 * @param fp a pointer to the header being used as a fencepost
 * @param left_size the size of the object to the left of the fencepost
 */
inline static void initialize_fencepost(header * fp, size_t left_size) {
	set_state(fp,FENCEPOST);
	set_size(fp, ALLOC_HEADER_SIZE);
	fp->left_size = left_size;
}

/**
 * @brief Helper function to maintain list of chunks from the OS for debugging
 *
 * @param hdr the first fencepost in the chunk allocated by the OS
 */
inline static void insert_os_chunk(header * hdr) {
  if (numOsChunks < MAX_OS_CHUNKS) {
    osChunkList[numOsChunks++] = hdr;
  }
}

/**
 * @brief given a chunk of memory insert fenceposts at the left and 
 * right boundaries of the block to prevent coalescing outside of the
 * block
 *
 * @param raw_mem a void pointer to the memory chunk to initialize
 * @param size the size of the allocated chunk
 */
inline static void insert_fenceposts(void * raw_mem, size_t size) {
  // Convert to char * before performing operations
  char * mem = (char *) raw_mem;

  // Insert a fencepost at the left edge of the block
  header * leftFencePost = (header *) mem;
  initialize_fencepost(leftFencePost, ALLOC_HEADER_SIZE);

  // Insert a fencepost at the right edge of the block
  header * rightFencePost = get_header_from_offset(mem, size - ALLOC_HEADER_SIZE);
  initialize_fencepost(rightFencePost, size - 2 * ALLOC_HEADER_SIZE);
}

/**
 * @brief Allocate another chunk from the OS and prepare to insert it
 * into the free list
 *
 * @param size The size to allocate from the OS
 *
 * @return A pointer to the allocable block in the chunk (just after the 
 * first fencpost)
 */
static header * allocate_chunk(size_t size) {
  void * mem = sbrk(size);
  
  insert_fenceposts(mem, size);
  header * hdr = (header *) ((char *)mem + ALLOC_HEADER_SIZE);
  set_state(hdr, UNALLOCATED);
  set_size(hdr, size - 2 * ALLOC_HEADER_SIZE);
  hdr->left_size = ALLOC_HEADER_SIZE;
  return hdr;
}

/**
 * @brief Helper allocate an object given a raw request size from the user
 *
 * @param raw_size number of bytes the user needs
 *
 * @return A block satisfying the user's request
 */
static inline header * allocate_object(size_t raw_size) {
  // TODO implement allocation
  //1. calc required block size by adding size of metadata and rounding up
  if(raw_size == 0){
	return NULL;
  }
  size_t  actual_size = (raw_size + ALLOC_HEADER_SIZE + 7) & ~7;
  if(actual_size < sizeof(header)){
	actual_size = sizeof(header);
  }
  //printf("Actual size = %d\n", (int)actual_size);
  //2. Find appropriate free list
  int found = 0;
  int sent = (actual_size / 8) - 1;
  //int s = (int) (get_size(freelist) / 8);
  if(sent >= N_LISTS - 1){ 
	sent = N_LISTS - 1;
  }
  //printf("s = %d\n", s);
  header * freelist = &freelistSentinels[sent];
  freelist = freelist->next;
  
  header * t;// = (header *)((char *) actual_size);
  //set_size_and_state(t, actual_size, UNALLOCATED);
  //header * freelist = &freelistSentinels[N_LISTS - 1];
  set_state(freelist, UNALLOCATED);
  header * flleft;
  //header * freelist = &freelistSentinels[sent];
  //freelist->next = t;
  //freelist->prev = t;
  //t->next = freelist;
  //t->prev = freelist;
  while(found == 0){
        if(get_size(freelist) >= actual_size){
		//void * mem = sbrk(get_size(freelist));
		//t = (header *) ((char *)mem + ALLOC_HEADER_SIZE);
		t = (header *)((char *) ptr_to_header(freelist) + (get_size(freelist) - actual_size + ALLOC_HEADER_SIZE)); //976
		//print_object(t);
		flleft = get_left_header(freelist);
		set_state(t, UNALLOCATED);
		set_size(t, get_size(freelist));
		t->left_size = (get_size(freelist) - get_size(t));
		//set_size(freelist, t->left_size);
		//t->next = freelist->next;	//insert t into freelist
		//t->prev = freelist;
		//t->next->prev = t;
		//freelist->next = t;
		freelist->next = t;
		freelist->prev = t;
		t->next = freelist;
		t->prev = freelist;
		//print_object(t);
		found = 1;			//block found
	}else if(sent < N_LISTS - 1){
		sent++;
		freelist = &freelistSentinels[sent];
		freelist = freelist->next;
		//printf("freelist%d size = %d\n", sent, (int)get_size(freelist));
	}else{
		int total = 0;
		//1. create new chunk
		while(total < (int)actual_size){
                	total += ARENA_SIZE;
			//printf("total = %d\n", total);
			//printf("actual size = %d\n", (int)actual_size);
			header * c = allocate_chunk(ARENA_SIZE);
			header * prevfp = get_header_from_offset(c, -ALLOC_HEADER_SIZE);
                	//insert_os_chunk(prevfp);
			//lastFencePost = get_header_from_offset(c, get_size(c));
		
		//2. check if previous chunk is next to new one in memory
			header * pc = get_header_from_offset(prevfp, -ALLOC_HEADER_SIZE);
			//printf("checking\n");
			//print_object(c);
			//print_object(pc);
			//print_object(lastFencePost);
			//print_object(osChunkList[numOsChunks - 1]);
			//sent = (actual_size / 8) - 1;

			//if(sent >= N_LISTS - 1){
			//	sent = N_LISTS - 1;
			//}
			//printf("sent = %d\n", sent);
			freelist = &freelistSentinels[N_LISTS - 1];
			freelist = freelist->next;
			//freelist->next = freelist;
			//freelist->prev = freelist;
		//3. if new chunk is not adjacent insert into free list
			if(pc != lastFencePost){
				//printf("1\n");
				insert_os_chunk(prevfp);
				//freelist->next = c;
				//freelist->prev = c;
				//c->next = freelist;
				//c->prev = freelist;
				c->next = freelist->next;
				c->prev = freelist;
				freelist->next->prev = c;
				freelist->next = c;
				lastFencePost = get_header_from_offset(c, get_size(c));
			}else{
		//else remove fenceposts between both
				//printf("2\n");
				header * leftC = get_left_header(pc);
				//print_object(leftC);
				if(get_state(leftC) == 0){
					//printf("3\n");
					set_state(pc, UNALLOCATED);
					set_state(prevfp, UNALLOCATED);
					set_size(leftC, (get_size(leftC) + get_size(pc) + get_size(prevfp) + get_size(c)));
					//print_object(leftC);
					header * endfp = get_right_header(c);
					//print_object(endfp);
					endfp->left_size = get_size(leftC);
					//printf("sent = %d\n", sent);
					//remove old pointers
					leftC->prev->next = leftC->prev;
					leftC->prev->prev = leftC->prev;
					
					//set new pointers
					set_size(freelist, get_size(leftC));
					set_state(freelist, UNALLOCATED);
					freelist->left_size = leftC->left_size;
					//freelist->next = leftC;
					//freelist->prev = leftC;
					//leftC->next = freelist;
					//leftC->prev = freelist;
					leftC->next = freelist->next;
					leftC->prev = freelist;
					freelist->next->prev = leftC;
					freelist->next = leftC;
					//lastFencePost = endfp;
					lastFencePost = get_header_from_offset(c, get_size(c));
					set_state(lastFencePost, FENCEPOST);
					set_size(lastFencePost, ALLOC_HEADER_SIZE);
					//print_object(lastFencePost);
				}else{
					//printf("4\n");
					set_state(pc, UNALLOCATED);
					set_state(prevfp, UNALLOCATED);
					set_size(pc, (get_size(pc) + get_size(prevfp) + get_size(c)));
                                        header * endfp = get_right_header(c);
					//print_object(pc);
					//print_object(endfp);
                                        endfp->left_size = get_size(pc);
                                        //freelist->next = pc;
                                        //freelist->prev = pc;
                                        //pc->next = freelist;
                                        //pc->prev = freelist;
					pc->next = freelist->next;
					pc->prev = freelist;
					freelist->next->prev = c;
					freelist->next = c;
					//printf("new\n");
					//print_object(pc);
					//print_object(endfp);
                                        //lastFencePost = endfp;
					lastFencePost = get_header_from_offset(c, get_size(c));
					//print_object(lastFencePost);
				}
			}
		//and coalesce with unallocated mem at end of prev chunk
		
		//4. repeat as necessary to get enough memory
		}
		/*if(freelist->next == freelist){
			freelist->next = &freelistSentinels[sent];
		}
		if(freelist->prev == freelist){
			freelist->prev = &freelistSentinels[sent];
		}*/
		//printf("3\n");
		//sent = (actual_size / 8) - 1;
                //if(sent >= N_LISTS - 1){
	        //        sent = N_LISTS - 1;
                //}
		//printf("sent = %d\n", sent);
                freelist = &freelistSentinels[N_LISTS - 1];
		freelist = freelist->next;
	}
  }
  //3. If size of block is too big then split the block
  //update size and left_size as neccessay
  if((get_size(t) - actual_size) >= 32){
	//header * h = (header *) ((char *) ptr_to_header(t) - actual_size);
	set_size(freelist, get_size(t) - actual_size);
	header * h = (header *) ((char *) ptr_to_header(t) + ALLOC_HEADER_SIZE);		//976
	set_size(h, actual_size);		//update size
	set_state(h, UNALLOCATED);
	//h->left_size = get_size(t);		//update left size
	h->left_size = get_size(freelist);
	//set_size(freelist, h->left_size);
	//allocate the right to user
	//header * rh = get_right_header(h);
	//printf("splitting\n");
	//print_object(h);
	header * rh = (header *)((char *) ptr_to_header(h) + get_size(h) + ALLOC_HEADER_SIZE);	//1008
	//print_object(rh);
	rh->left_size = get_size(h);

	int s = (int) ((get_size(freelist) - ALLOC_HEADER_SIZE)/ 8)-1;
	if(s >= N_LISTS - 1){
		s = N_LISTS - 1;
	}
	//printf("s = %d\n", s);
	freelist->next = &freelistSentinels[s];
	freelist->prev = &freelistSentinels[s];
	//freelist->next = rh;
	//freelist->prev = rh;
	//rh->next = freelist;
	//rh->prev = freelist;
	//t->next = t;
	//t->prev = t;
	set_size(t, get_size(h));
	t->left_size = h->left_size;
	set_state(t, ALLOCATED);
  }else{
  	//4. Update to ALLOCATED
  	//freelist->next = t->next;			//remove it from free list
  	//t->next->prev = freelist;
  	//freelist->prev = t->next;
  	//t->next = freelist;
	//freelist->next = freelist;
	//freelist->prev = freelist;
	t->left_size = get_size(flleft);
	//int s = (int) ((get_size(freelist) - ALLOC_HEADER_SIZE) / 8) - 1;
	//if(s >= N_LISTS - 1){ 
	//	s = N_LISTS - 1;
	//}
	//set_size(freelist, t->left_size);
	//printf("s2 = %d\n", s);
	header * rh = (header *)((char *) ptr_to_header(t) + get_size(t) + ALLOC_HEADER_SIZE);
	rh->left_size = get_size(t);
	
	freelist->next = &freelistSentinels[sent];
	freelist->prev = &freelistSentinels[sent];
	set_state(freelist, ALLOCATED);
  	set_state(t, ALLOCATED);
	//printf("sent = %d\n", sent);
	//print_object(flleft);
	//print_object(t);
	//print_object(rh);
  }
  //5. Return pointer to data field of the header
  return (header *) t->data;
}

/**
 * @brief Helper to get the header from a pointer allocated with malloc
 *
 * @param p pointer to the data region of the block
 *
 * @return A pointer to the header of the block
 */
static inline header * ptr_to_header(void * p) {
  return (header *)((char *) p - ALLOC_HEADER_SIZE); //sizeof(header));
}

/**
 * @brief Helper to manage deallocation of a pointer returned by the user
 *
 * @param p The pointer returned to the user by a call to malloc
 */
static inline void deallocate_object(void * p) {
  // TODO implement deallocation
  //1. get location of header
  //printf("size of object to be allocated = %d\n", (int)get_size((header *)p));
  if(p == NULL){
	return;
  }
  header * mid = (header *)((char *) p - sizeof(header) + ALLOC_HEADER_SIZE);  //944
  //print_object(mid);
  if(get_state(mid) == 0){
	printf("Double Free Detected\n");
	assert(false);
  }
  //printf("size of object to be allocated = %d\n", (int)get_size(mid));
  int s = (int)(((get_size(mid) - ALLOC_HEADER_SIZE) / 8) - 1);
  header * freelist;
  if(s >= N_LISTS - 1){
	s = N_LISTS - 1;
  }
  freelist = &freelistSentinels[s];
  freelist = freelist->next;

  //header * mid = (header *)((char *) p - sizeof(header) + 16);	//944

  //2. get location of left and right
  header * left = get_left_header(mid);
  header * right = get_right_header(mid);

  //3. based on allocation status either insert or coalesce
  if(get_state(left) == 0 && get_state(right) == 0){
	//coalesce both
	//printf("BOTH BOTH\n");
	set_size(left, (get_size(left) + get_size(mid) + get_size(right)));
	
	s  = (int)(((get_size(left) - ALLOC_HEADER_SIZE) / 8) - 1);
	if(s >= N_LISTS - 1){
		s = N_LISTS - 1;
	}
	freelist = &freelistSentinels[s];

	
	//remove right node
	//mid->next->next->prev = mid;
	//mid->next = mid->next->next;
	right->prev->prev = right->prev;
	right->next->next = right->next;
	
	//left->next = freelist->next;
	//left->prev = freelist;
	//freelist->next->prev = left;
	//freelist->next = left;
	freelist->next = left;
	freelist->prev = left;
	left->next = freelist;
	left->prev = freelist;

	
	//update new right node size;
	header * new_right = get_right_header(right);
	new_right->left_size = get_size(left);

  }else if(get_state(left) == 0){
	//coalesce left only
	//printf("LEFT LEFT\n");
	set_size(left, (get_size(left) + get_size(mid)));
	
	//update pointers
	s = (int)(((get_size(left) - ALLOC_HEADER_SIZE) / 8) - 1);
	if(s >= N_LISTS - 1){
		s = N_LISTS - 1;
	}
	
	freelist = &freelistSentinels[s];
	//freelist = freelist->next;
	
	left->prev->prev = left->prev;
	left->next->next = left->next;
	
	left->next = freelist->next;
        left->prev = freelist;
        freelist->next->prev = left;
        freelist->next = left;
	//freelist->next = left;
	//freelist->prev = left;
	//left->next = freelist;
	//left->prev = freelist;
	
	set_state(mid, UNALLOCATED);
	
	
	//update right node
	right->left_size = get_size(left);

  }else if(get_state(right) == 0){
	//coalesce right only
	set_size(mid, (get_size(mid) + get_size(right)));
	
	s = (int)(((get_size(mid) - ALLOC_HEADER_SIZE) / 8) - 1);
	if(s >= N_LISTS - 1){ 
		s = N_LISTS - 1;
	}
	freelist = &freelistSentinels[s];

	//update pointers
	right->prev->prev = right->prev;
	right->next->next = right->next;
	
	mid->next = freelist->next;
        mid->prev = freelist;
        freelist->next->prev = mid;
        freelist->next = mid;
	//freelist->next = mid;
	//freelist->prev = mid;
	//mid->next = freelist;
	//mid->prev = freelist;
	
	//change allocation setting
	set_state(mid, UNALLOCATED);
	
	//update new right node
	header * new_right = get_right_header(right);
	new_right->left_size = get_size(mid);
  }else{
	//no coalesceing needed
	//update the pointers
	//mid->next = freelist->next;
	//freelist->prev = mid;
	//mid->prev = freelist;
	//freelist->next = mid;
	//right->prev->prev = right->prev;
	//right->next->next = right->next;
	s = (int)(((get_size(mid) - ALLOC_HEADER_SIZE) / 8) - 1);
	if(s >= N_LISTS - 1){
		s = N_LISTS - 1;
	}
	freelist = &freelistSentinels[s];
	 
	mid->next = freelist->next;
        mid->prev = freelist;
        freelist->next->prev = mid;
        freelist->next = mid;
	//printf("free\n");
	//print_object(mid);
	//freelist->next = mid;
	//freelist->prev = mid;
	//mid->next = freelist;
	//mid->prev = freelist;

	
	//update mid state
	set_state(mid, UNALLOCATED);
	//print_object(mid);
  }
  if(freelist->next == freelist){
	freelist->next = &freelistSentinels[s];
  }
  if(freelist->prev == freelist){
	freelist->prev = &freelistSentinels[s];
  }
  return;
}

/**
 * @brief Helper to detect cycles in the free list
 * https://en.wikipedia.org/wiki/Cycle_detection#Floyd's_Tortoise_and_Hare
 *
 * @return One of the nodes in the cycle or NULL if no cycle is present
 */
static inline header * detect_cycles() {
  for (int i = 0; i < N_LISTS; i++) {
    header * freelist = &freelistSentinels[i];
    for (header * slow = freelist->next, * fast = freelist->next->next; 
         fast != freelist; 
         slow = slow->next, fast = fast->next->next) {
      if (slow == fast) {
        return slow;
      }
    }
  }
  return NULL;
}

/**
 * @brief Helper to verify that there are no unlinked previous or next pointers
 *        in the free list
 *
 * @return A node whose previous and next pointers are incorrect or NULL if no
 *         such node exists
 */
static inline header * verify_pointers() {
  for (int i = 0; i < N_LISTS; i++) {
    header * freelist = &freelistSentinels[i];
    for (header * cur = freelist->next; cur != freelist; cur = cur->next) {
      if (cur->next->prev != cur || cur->prev->next != cur) {
        return cur;
      }
    }
  }
  return NULL;
}

/**
 * @brief Verify the structure of the free list is correct by checkin for 
 *        cycles and misdirected pointers
 *
 * @return true if the list is valid
 */
static inline bool verify_freelist() {
  header * cycle = detect_cycles();
  if (cycle != NULL) {
    fprintf(stderr, "Cycle Detected\n");
    print_sublist(print_object, cycle->next, cycle);
    return false;
  }

  header * invalid = verify_pointers();
  if (invalid != NULL) {
    fprintf(stderr, "Invalid pointers\n");
    print_object(invalid);
    return false;
  }

  return true;
}

/**
 * @brief Helper to verify that the sizes in a chunk from the OS are correct
 *        and that allocated node's canary values are correct
 *
 * @param chunk AREA_SIZE chunk allocated from the OS
 *
 * @return a pointer to an invalid header or NULL if all header's are valid
 */
static inline header * verify_chunk(header * chunk) {
	if (get_state(chunk) != FENCEPOST) {
		fprintf(stderr, "Invalid fencepost\n");
		print_object(chunk);
		return chunk;
	}
	
	for (; get_state(chunk) != FENCEPOST; chunk = get_right_header(chunk)) {
		if (get_size(chunk)  != get_right_header(chunk)->left_size) {
			fprintf(stderr, "Invalid sizes\n");
			print_object(chunk);
			return chunk;
		}
	}
	
	return NULL;
}

/**
 * @brief For each chunk allocated by the OS verify that the boundary tags
 *        are consistent
 *
 * @return true if the boundary tags are valid
 */
static inline bool verify_tags() {
  for (size_t i = 0; i < numOsChunks; i++) {
    header * invalid = verify_chunk(osChunkList[i]);
    if (invalid != NULL) {
      return invalid;
    }
  }

  return NULL;
}

/**
 * @brief Initialize mutex lock and prepare an initial chunk of memory for allocation
 */
static void init() {
  // Initialize mutex for thread safety
  pthread_mutex_init(&mutex, NULL);

#ifdef DEBUG
  // Manually set printf buffer so it won't call malloc when debugging the allocator
  setvbuf(stdout, NULL, _IONBF, 0);
#endif // DEBUG

  // Allocate the first chunk from the OS
  header * block = allocate_chunk(ARENA_SIZE);

  header * prevFencePost = get_header_from_offset(block, -ALLOC_HEADER_SIZE);
  insert_os_chunk(prevFencePost);

  lastFencePost = get_header_from_offset(block, get_size(block));

  // Set the base pointer to the beginning of the first fencepost in the first
  // chunk from the OS
  base = ((char *) block) - ALLOC_HEADER_SIZE; //sizeof(header);

  // Initialize freelist sentinels
  for (int i = 0; i < N_LISTS; i++) {
    header * freelist = &freelistSentinels[i];
    freelist->next = freelist;
    freelist->prev = freelist;
  }

  // Insert first chunk into the free list
  header * freelist = &freelistSentinels[N_LISTS - 1];
  freelist->next = block;
  freelist->prev = block;
  block->next = freelist;
  block->prev = freelist;
}

/* 
 * External interface
 */
void * my_malloc(size_t size) {
  pthread_mutex_lock(&mutex);
  header * hdr = allocate_object(size); 
  pthread_mutex_unlock(&mutex);
  return hdr;
}

void * my_calloc(size_t nmemb, size_t size) {
  return memset(my_malloc(size * nmemb), 0, size * nmemb);
}

void * my_realloc(void * ptr, size_t size) {
  void * mem = my_malloc(size);
  memcpy(mem, ptr, size);
  my_free(ptr);
  return mem; 
}

void my_free(void * p) {
  pthread_mutex_lock(&mutex);
  deallocate_object(p);
  pthread_mutex_unlock(&mutex);
}

bool verify() {
  return verify_freelist() && verify_tags();
}
