package relop;


/**
 * The projection operator extracts columns from a relation; unlike in
 * relational algebra, this operator does NOT eliminate duplicate tuples.
 */
public class Projection extends Iterator {
	
	Iterator iter;
	Schema schem;
	Integer[] fields;
	boolean open = false;

  /**
   * Constructs a projection, given the underlying iterator and field numbers.
   */
  public Projection(Iterator aIter, Integer... aFields) {
	this.iter = aIter;
	this.fields = aFields;
	this.schem = new Schema(fields.length);
	Schema temp = iter.getSchema();
	for(int i = 0; i < fields.length; i++){
		//create the real schema using the fields and iter
		//do this to get every element
		schem.initField(i, temp.fieldType(fields[i]), temp.fieldLength(fields[i]), temp.fieldName(fields[i]));
	}
	setSchema(schem);
	open = true;
  }

  /**
   * Gives a one-line explanation of the iterator, repeats the call on any
   * child iterators, and increases the indent depth along the way.
   */
  public void explain(int depth) {
	  for(int i = 0; i < depth; i++){
		  System.out.printf("    ");
	  }
	  System.out.printf("Projection\n");
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
	open = false;
  }

  /**
   * Returns true if there are more tuples, false otherwise.
   */
  public boolean hasNext() {
	if(open){
		return iter.hasNext();
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
		Tuple old = iter.getNext();
		Tuple nTuple = new Tuple(schem);
		for(int i = 0; i < fields.length; i++){
			nTuple.setField(i, old.getField(fields[i]));
		}
		return nTuple;
	}else{
		throw new IllegalStateException("No More Tuples");
	}
  }

} // public class Projection extends Iterator
