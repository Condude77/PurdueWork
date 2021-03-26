package relop;

import global.SearchKey;
import heap.HeapFile;
import index.HashIndex;
import index.HashScan;
import global.RID;

/**
 * Wrapper for hash scan, an index access method.
 */
public class KeyScan extends Iterator {
	
	HeapFile file = null;
	HashScan scan = null;
	HashIndex index = null;
	SearchKey key = null;;
	boolean open = false;
	
  /**
   * Constructs an index scan, given the hash index and schema.
   */
  public KeyScan(Schema aSchema, HashIndex aIndex, SearchKey aKey, HeapFile aFile) {
	setSchema(aSchema);
	this.file = aFile;
	this.index = aIndex;
	this.key = aKey;
	this.scan = index.openScan(key);
	this.open = true;
  }

  /**
   * Gives a one-line explanation of the iterator, repeats the call on any
   * child iterators, and increases the indent depth along the way.
   */
  public void explain(int depth) {
	  for(int i = 0; i < depth; i++){
		  System.out.printf("    ");
	  }
	  System.out.printf("KeyScan\n");
  }

  /**
   * Restarts the iterator, i.e. as if it were just constructed.
   */
  public void restart() {
	if(open){
		//if open then close it to release pinned pages
		this.scan.close();
	}
	this.scan = index.openScan(key);
  }

  /**
   * Returns true if the iterator is open; false otherwise.
   */
  public boolean isOpen() {
	return open;
  }

  /**
   * Closes the iterator, releasing any resources (i.e. pinned pages).
   */
  public void close() {
	if(open){
		//if open then release pinned pages
		scan.close();
	}
	open = false;
  }

  /**
   * Returns true if there are more tuples, false otherwise.
   */
  public boolean hasNext() {
	if(open){
		//call to see if there is a next
		return scan.hasNext();
	}else{
		return false;
	}
  }

  /**
   * Gets the next tuple in the iteration.
   * 
   * @throws IllegalStateException if no more tuples
   */
  public Tuple getNext() {
	if(open){
		RID ridData = scan.getNext(); //get RID of next Tuple
		byte[] data = file.selectRecord(ridData); //get data of next Tuple
		Tuple nTuple = new Tuple(this.getSchema(), data);
		return nTuple;
	}else{
		throw new IllegalStateException("No More Tuples");
	}
  }

} // public class KeyScan extends Iterator
