package relop;

import global.SearchKey;
import heap.HeapFile;
import index.HashIndex;
import index.BucketScan;
import global.RID;

/**
 * Wrapper for bucket scan, an index access method.
 */
public class IndexScan extends Iterator {

	HeapFile file = null;
	BucketScan scan = null;
	HashIndex index = null;
	boolean open = false;

  /**
   * Constructs an index scan, given the hash index and schema.
   */
  public IndexScan(Schema schema, HashIndex index, HeapFile file) {
	this.file = file;
	setSchema(schema);
	this.index = index;
	this.scan = this.index.openScan();
	open = true;
	
  }

  /**
   * Gives a one-line explaination of the iterator, repeats the call on any
   * child iterators, and increases the indent depth along the way.
   */
  public void explain(int depth) {
	  for(int i = 0; i < depth; i++){
		  System.out.printf("    ");
	  }
	  System.out.printf("IndexScan\n");
  }

  /**
   * Restarts the iterator, i.e. as if it were just constructed.
   */
  public void restart() {
	if(open){
		//close to release pinned pages
		this.scan.close();
	}
	this.scan = this.index.openScan();
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
		//if open release pinned pages
		scan.close();
	}
	open = false;
}

  /**
   * Returns true if there are more tuples, false otherwise.
   */
  public boolean hasNext() {
	if(open){
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
		RID ridData = scan.getNext();			//get RID of next tuple
		byte[] data = file.selectRecord(ridData);	//get data of next tuple
		Tuple nTuple = new Tuple(getSchema(), data);
		return nTuple;
	}else{
		throw new IllegalStateException("No More Tuples");
	}
  }

  /**
   * Gets the key of the last tuple returned.
   */
  public SearchKey getLastKey() {
	return scan.getLastKey();
  }

  /**
   * Returns the hash value for the bucket containing the next tuple, or maximum
   * number of buckets if none.
   */
  public int getNextHash() {
	return scan.getNextHash();
  }

} // public class IndexScan extends Iterator
