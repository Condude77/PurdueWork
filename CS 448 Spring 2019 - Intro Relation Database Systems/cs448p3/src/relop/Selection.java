package relop;

/**
 * The selection operator specifies which tuples to retain under a condition; in
 * Minibase, this condition is simply a set of independent predicates logically
 * connected by OR operators.
 */
public class Selection extends Iterator {
	Iterator iter;
	Predicate[] pred;
	Tuple nTuple;
	boolean open = false;
  /**
   * Constructs a selection, given the underlying iterator and predicates.
   */
  public Selection(Iterator aIter, Predicate... aPreds) {
	this.pred = aPreds;
	this.iter = aIter;
	schema = iter.getSchema();
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
	  System.out.printf("Selection\n");
  }

  /**
   * Restarts the iterator, i.e. as if it were just constructed.
   */
  public void restart() {
	if(open){
		this.iter.close();
	}
	this.iter.restart();
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
		this.iter.close();
	}
	this.open = false;
  }

  /**
   * Returns true if there are more tuples, false otherwise.
   */
  public boolean hasNext() {
	while(iter.hasNext()){
		Tuple temp = iter.getNext();
		if(pred.length == 0){
			//if no more predicates to check then we return tuples
			nTuple = temp;
			return true;
		}
		for(Predicate p : pred){
			if(p.evaluate(temp)){
				//if the Tuple is true then set return tuple to temp
				nTuple = temp;
				return true;
			}
		}
	}
	//no more tuples so set to null and return false
	nTuple = null;
	return false;
  }

  /**
   * Gets the next tuple in the iteration.
   * 
   * @throws IllegalStateException if no more tuples
   */
  public Tuple getNext() {
	if(nTuple != null){
		return nTuple;
	}else{
		throw new IllegalStateException("No More Tuples");
	}
  }

} // public class Selection extends Iterator
