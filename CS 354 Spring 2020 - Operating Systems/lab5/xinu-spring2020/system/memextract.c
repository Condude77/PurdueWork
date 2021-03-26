/* memextract.c - memextract */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  memextract  -  extract memory block from list for Lab 5 part 4.2
 *------------------------------------------------------------------------
 */
void memextract(struct inusememblk *hdptr, char *memptr){
	memblockkflag = 1;

	struct inusememblk *prev;
	struct inusememblk *curr;

	prev = &hdptr->memblockptr;
	curr = hdptr->mnext;

	while(curr != NULL){		/* Search for memory to be deleted	*/
		if(curr == memptr){
			break;
		}		
		prev = curr;
		curr = curr->mnext;
	}
	prev->mnext = curr->mnext;	/* Unlink from list	*/
	
}
