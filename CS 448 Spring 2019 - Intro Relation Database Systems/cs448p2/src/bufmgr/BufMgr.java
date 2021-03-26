package bufmgr;

import java.io.IOException;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.Queue;

import chainexception.ChainException;
import diskmgr.DiskMgrException;
import diskmgr.FileIOException;
import diskmgr.InvalidPageNumberException;
import diskmgr.InvalidRunSizeException;
import global.Minibase;
import global.Page;
import global.PageId;

public class BufMgr {
	// Instance variables - some of these are accessed during the test cases
	// DO NOT CHANGE THE BELOW INSTANCE VARIABLE NAMES - you should use them as appropriate in your code
	// You may add additional ones as you need to
	
	//bufFrames: the buffer pool. An array of Page objects
	private Page[] bufFrames = null;
	//bufDescr: An arracy of FrameDescriptor objects, holding information about the contents of each frame.
	protected FrameDescriptor[] bufDescr = null;
	//numOfFrames: the number of frames used for the maximum capacity of the buffer pool
	private int numOfFrames = -1;
	//replacementPolicy: will be set to FIFO when the constructor is called. Can be ignored for this assignment
	private String replacementPolicy = "";
	// hashTable: Hash table to track which frame in the buffer a page is in
	// Key: PageID - Value: Frame
	private HashMap<Integer, Integer> hashTable = null;
	// fifo: queue for FIFO page replacement - all unpinned pages will be stored here,
	// with the first element being the pageID that was unpinned the longest time
	// ago
	private Queue<Integer> fifo = null;
	//END OF REQUIRED INSTANCE VARIABLES

	//Linked List that holds the values to be replaced

	/**
	 * Resets a FrameDescriptor to the default values with no pageID
	 */
	protected void resetFrameDescriptor(int frameId) {
		resetFrameDescriptor(frameId, -1);
	}

	/**
	 * Resets a FrameDescriptor to the default values with the given pageID
	 */
	protected void resetFrameDescriptor(int frameId, int pageno) {
		bufDescr[frameId].PageNumber = pageno;
		bufDescr[frameId].PinCount = 0;
		bufDescr[frameId].DirtyBit = false;
	}

	/**
	 * Create the BufMgr object. Allocate pages (frames) for the buffer pool in main
	 * memory and make the buffer manage aware that the replacement policy is
	 * specified by replacerArg (e.g., LH, Clock, LRU, MRU, LIRS, etc.).
	 *
	 * @param numbufs
	 *            number of buffers in the buffer pool
	 * @param lookAheadSize
	 *            number of pages to be looked ahead - can be ignored for this assignment
	 * @param replacementPolicy
	 *            Name of the replacement policy
	 */
	public BufMgr(int numbufs, int lookAheadSize, String replacementPolicy) {
		// we ignore replacementPolicy as there is only one policy implemented in the
		// system
		numOfFrames = numbufs;
		bufFrames = new Page[numOfFrames];
		bufDescr = new FrameDescriptor[numOfFrames];
		fifo = new LinkedList<Integer>();
		this.replacementPolicy = replacementPolicy;
		for (int i = 0; i < numOfFrames; i++) {
			bufFrames[i] = new Page();
			bufDescr[i] = new FrameDescriptor();
			bufDescr[i].PageNumber = -1;
			bufDescr[i].PinCount = 0;
			bufDescr[i].DirtyBit = false;
			fifo.add(i);
		}
		hashTable = new HashMap<Integer, Integer>();
	}

	/**
	 * Pin a page. First check if this page is already in the buffer pool. If it is,
	 * increment the pin_count and return a pointer to this page. If the pin_count
	 * was 0 before the call, the page was a replacement candidate, but is no longer
	 * a candidate. If the page is not in the pool, choose a frame (from the set of
	 * replacement candidates) to hold this page, read the page (using the
	 * appropriate method from diskmgr package) and pin it. Also, must write out the
	 * old page in chosen frame if it is dirty before reading new page.(You can
	 * assume that emptyPage==false for this assignment.)
	 *
	 * @param pageno
	 *            page number in the Minibase.
	 * @param page
	 *            the pointer point to the page.
	 * @throws BufferPoolExceededException if there are no valid replacement candidates when attempting to pin a page not already in memory
	 * @throws DiskMgrException if there is an error from the DiskMgr layer. This is likely caused by incorrect implementations of other methods in the BufferManager
	 */
	public void pinPage(PageId pageno, Page page, boolean emptyPage)
			throws BufferPoolExceededException, DiskMgrException{
		//YOUR CODE HERE
		Integer frameNum = hashTable.get(new Integer(pageno.pid));
		if(frameNum != null){
			if(bufDescr[frameNum].PinCount == 0){
			//if pin count is 0 then we remove it from the list
				fifo.remove(frameNum);
			}
			bufDescr[frameNum].PinCount++;
			//set the page reference to the correct location
			page.setPage(bufFrames[frameNum]);
		}else{
			if(fifo.peek() == null){
				//peek list to see if it is null
				throw new BufferPoolExceededException("buffer is full in pin page");
			}else{
				frameNum = fifo.remove();
				if(bufDescr[frameNum].PageNumber != -1){
					if(bufDescr[frameNum].DirtyBit == true){
						//if page is dirty the flush it
						//PageId temp = new PageId(bufDescr[frameNum].PageNumber);
						try{
							flushPage(new PageId(bufDescr[frameNum].PageNumber));
						}catch(HashEntryNotFoundException e){
							throw new DiskMgrException("Page number not found");
						}
					}
					if(hashTable.get(bufDescr[frameNum].PageNumber) != -1){
						//if page is still in hash table then remove it
						hashTable.remove(bufDescr[frameNum].PageNumber);
					}
				}
				//set the page reference to the correct location
				page.setPage(bufFrames[frameNum]);
				//read new page from disk
				Minibase.DiskManager.read_page(pageno, page);
				//set descriptor for new page
				PageId id = new PageId(pageno.pid);
				bufDescr[frameNum] = new FrameDescriptor();
				bufDescr[frameNum].PageNumber = id.pid;
 				bufDescr[frameNum].PinCount = 1;
 				bufDescr[frameNum].DirtyBit = false;
				//insert into hash table
				hashTable.put(pageno.pid, frameNum);
			}
		}
	}

	/**
	 * Unpin a page specified by a pageId. This method should be called with
	 * dirty==true if the client has modified the page. If so, this call should set
	 * the dirty bit for this frame. Further, if pin_count>0, this method should
	 * decrement it. If pin_count=0 before this call, throw an exception to report
	 * error. (For testing purposes, we ask you to throw an exception named
	 * PageUnpinnedException in case of error.)
	 *
	 * @param pageno
	 *            the PageID of the page
	 * @param dirty
	 *            whether or not the page is dirty
	 * @throws HashEntryNotFoundException the page is not in memory
	 * @throws PageUnpinnedException the page is already unpinned
	 */
	public void unpinPage(PageId pageno, boolean dirty)
			throws HashEntryNotFoundException, PageUnpinnedException {
		//YOUR CODE HERE
		Integer frameNum = hashTable.get(new Integer(pageno.pid));
		if(frameNum == null){
			throw new HashEntryNotFoundException("Hash entry not found");
		}else{
			if(bufDescr[frameNum].PinCount == 0){
				throw new PageUnpinnedException("Page unpinned error");
			}
			if(dirty == true){
				bufDescr[frameNum].DirtyBit = true;
			}
			bufDescr[frameNum].PinCount--;
			if(bufDescr[frameNum].PinCount == 0){
				//if pin count = 0 add page to be replaced
				fifo.add(new Integer(frameNum));
			}
		}
	}

	/**
	 * Allocate new pages. Call DB object to allocate a run of new pages and find a
	 * frame in the buffer pool for the first page and pin it. (This call allows a
	 * client of the Buffer Manager to allocate pages on disk.) If buffer is full,
	 * i.e., you can't find a frame for the first page, ask DB to deallocate all
	 * these pages, and return null.
	 *
	 * @param firstpage
	 *            the address of the first page.
	 * @param howmany
	 *            total number of allocated new pages.
	 *
	 * @return the first page id of the new pages.__ null, if error.
	 * @throws DiskMgrException if there is an error from the DiskMgr layer. This is likely caused by incorrect implementations of other methods of the Buffer Manager
	 * @throws BufferPoolExceededException if the new page cannot be pinned after the run is allocated due to the buffer being full. If this exception is thrown, the newly allocated pages should be deallocated
	 */
	public PageId newPage(Page firstpage, int howmany) throws DiskMgrException, BufferPoolExceededException {
		//YOUR CODE HERE
		PageId newpid = new PageId();
		try{
			Minibase.DiskManager.allocate_page(newpid, howmany);
		}catch(BufMgrException e){
			throw new DiskMgrException("Allocate page error");
		}

		try{
			pinPage(newpid, firstpage, false);
		}catch(Exception e){
			try{
				Minibase.DiskManager.deallocate_page(newpid, howmany);
			}catch(BufMgrException x){
				throw new DiskMgrException("Pin error in new page");
			}
			throw new DiskMgrException("Pin error in new page");
		}

		return newpid;
	}
	
	/**
	 * This method should be called to delete a page that is on disk. This routine
	 * must call the method in diskmgr package to deallocate the page.
	 *
	 * @param globalPageId
	 *            the page number in the data base.
	 * @throws PagePinnedException if the page is still pinned
	 * @throws DiskMgrException if there is an error in the DiskMgr layer. This is likely caused by incorrect implementations in other methods of the Buffer Manager
	 */
	public void freePage(PageId globalPageId) throws PagePinnedException, DiskMgrException {
		// we only delete a page if it is on disk and we don't delete it if it is pinned
		Integer frameId = hashTable.get(globalPageId.pid);
		if (frameId != null && bufDescr[frameId].PinCount > 0) {
			throw new PagePinnedException(
					"BUFMGR (freePage): Page with id " + globalPageId.pid + " cannot be freed as it is pinned.");
		} else {
			if (frameId != null) // this was a page in memory with pin count of zero, delete it from all memory
									// references
			{
				hashTable.remove(globalPageId.pid);
				resetFrameDescriptor(frameId);
			}
			// then delete from disk
			try {
				Minibase.DiskManager.deallocate_page(globalPageId);
			} catch (BufMgrException e) {
				throw new DiskMgrException(e.getMessage());
			}

		}
	}

	/**
	 * Used to flush a particular page of the buffer pool to disk. This method calls
	 * the write_page method of the diskmgr package.
	 *
	 * @param pageid
	 *            the page number in the database.
	 * @throws HashEntryNotFoundException if the page is not in memory
	 * @throws DiskMgrException if there is an error in the DiskMgr layer. This is likely caused by incorrect implementations in other methods of the Buffer Manager
	 */
	public void flushPage(PageId pageid) throws HashEntryNotFoundException, DiskMgrException {
		// find the frame holding that page
		Integer frameId = hashTable.get(pageid.pid);
		if (frameId == null) {
			throw new HashEntryNotFoundException(
					"BufMgr.flushPage: Page with id " + pageid.pid + " does not exist in the buffer bool.");
		} else {
			Minibase.DiskManager.write_page(pageid, bufFrames[frameId]);
			bufDescr[frameId].DirtyBit = false;
		}
	}

	/**
	 * Used to flush all dirty pages in the buffer pool to disk
	 * @throws DiskMgrException if there is an error in the DiskMgr layer. This is likely caused by incorrect implementations in other methods of the Buffer Manager
	 */
	public void flushAllPages() throws DiskMgrException {
		for (int i = 0; i < numOfFrames; i++) {
			if (bufDescr[i].DirtyBit == true) {
				Minibase.DiskManager.write_page(new PageId(bufDescr[i].PageNumber), bufFrames[i]);
				bufDescr[i].DirtyBit = false;
			}
		}
	}

	/**
	 * Returns the total number of buffer frames.
	 */
	public int getNumBuffers() {
		return numOfFrames;
	}

	/**
	 * Returns the total number of unpinned buffer frames.
	 */
	public int getNumUnpinned() {
		int numUnpinned = 0;
		for (int i = 0; i < numOfFrames; i++) {
			if (bufDescr[i].PinCount <= 0) {
				numUnpinned++;
			}
		}
		return numUnpinned;
	}

	//*** DO NOT CHANGE ANY EXISTING METHODS BELOW THIS LINE ***
	// Accessor methods for use in test cases
	public FrameDescriptor getFrameDesc(int frameNum) {
		return bufDescr[frameNum];
	}

	public Page getPageFromFrame(int frameNum) {
		return bufFrames[frameNum];
	}

	public Integer getFrameFromPage(PageId pid) {
		return hashTable.get(new Integer(pid.pid));
	}
}
