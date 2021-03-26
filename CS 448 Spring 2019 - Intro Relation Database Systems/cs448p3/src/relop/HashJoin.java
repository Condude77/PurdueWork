package relop;

import heap.HeapFile;
import index.HashIndex;
import global.SearchKey;
import global.RID;
import global.AttrOperator;
import global.AttrType;

public class HashJoin extends Iterator {
	
	Iterator leftIter;	//temp iterator for joining schema
	Iterator rightIter;	//temp iterator for joining schema
	int leftCol = 0;	//int for left col for compare and join
	int rightCol = 0;	//int for right col for compare and join
	HashTableDup dup; 	//store hash memory to check against join
	boolean open = false;	//status for table

	IndexScan lScan;	//scan for joining for left iter
	IndexScan rScan;	//scan for joining for right iter
	
	int current = 0;	//current hash function
	Tuple held[] = null;	//array for storing tuples
	Tuple ret;		//tuple to be returned to the user
	Tuple check;		//checks tuples that are held
	int counter = 0;	//counter for join tables
	
	public HashJoin(Iterator aIter1, Iterator aIter2, int aJoinCol1, int aJoinCol2){
		this.leftIter = aIter1;
		this.rightIter = aIter2;
		this.leftCol = aJoinCol1;
		this.rightCol = aJoinCol2;
		schema = Schema.join(leftIter.schema, rightIter.schema);
		convert(leftIter, leftCol, true);	//index scan for left side
		convert(rightIter, rightCol, false);	//index scan for right side
		dup = new HashTableDup();
	} 

	/**
	* Gives a one-line explanation of the iterator, repeats the call on any
	* child iterators, and increases the indent depth along the way.
	*/
	public void explain(int depth) {
		for(int i = 0; i < depth; i++){
			System.out.printf("    ");
		}
	  	System.out.printf("HashJoin\n");
	}

	/**
	* Restarts the iterator, i.e. as if it were just constructed.
	*/
	public void restart() {
		lScan.restart();
		rScan.restart();
	}

	/**
	* Returns true if the iterator is open; false otherwise.
	*/
	public boolean isOpen() {
		if(lScan.isOpen() && rScan.isOpen()){
			return true;
		}else{
			return false;
		}
	}

	/**
	* Closes the iterator, releasing any resources (i.e. pinned pages).
	*/
	public void close() {
		lScan.close();
		rScan.close();
	}

	/**
	* Returns true if there are more tuples, false otherwise.
	*/
	public boolean hasNext() {
		if(held != null){
			if(counter == held.length - 1){
				//restart has next once we have reached the end of the array
				counter = 0;
				held = null;
				return hasNext();
			}else{
				for(;counter < held.length; counter++){
					if(check.getField(rightCol).equals(held[counter].getField(leftCol))){
						//check the tuples in the right vs left
						//set the return tuple
						ret = Tuple.join(held[counter++], check, schema);
						return true;
					}
				}
				//restart has next
				counter = 0;
				held = null;
				return hasNext();
			}
		}else{
			int hashCheck = rScan.getNextHash();
			if(current != hashCheck){
				current = hashCheck;
				lScan.restart();
				dup.clear();
				while(lScan.hasNext() && lScan.getNextHash() != current){
					//loop untol we are in the correct position
					lScan.getNext();
				}
				while(lScan.hasNext() && lScan.getNextHash() == current){
					Tuple lTuple = lScan.getNext();
					SearchKey temp = new SearchKey(lTuple.getField(leftCol).toString());
					dup.add(temp, lTuple);
				}
			}
			if(rScan.hasNext()){
				check = rScan.getNext();
				SearchKey compare = new SearchKey(check.getField(rightCol).toString());
				//get all tuples in the left side and add them to global array to check
				held = dup.getAll(compare);
				if(held != null){
					for(;counter < held.length; counter++){
						if(check.getField(rightCol).equals(held[counter].getField(leftCol))){
							//check the right tuple against all tuples
							ret = Tuple.join(held[counter++], check, schema);
							return true;
						}
					}
					//restart hasNext
					counter = 0;
					held = null;
					return hasNext();
				}
				//restart hasNext
				counter = 0;
				held = null;
				return hasNext();
			}else{
				//done with the right side
				held = null;
				return false;
			}
		}
	}

	/**
	* Gets the next tuple in the iteration.
	* 
	* @throws IllegalStateException if no more tuples
	*/
	public Tuple getNext() {
		if(ret != null){
			return ret;
		}else{
			throw new IllegalStateException("No More Tuples");
		}
	}

	public void convert(Iterator start, int col, boolean side){
		IndexScan tScan = null;
		FileScan tFile = null;
		HeapFile tHeap = null;
		HashIndex index = new HashIndex(null);
		
		if(start instanceof IndexScan){
			//if it is an IndexScan then just set to start
			tScan = (IndexScan)start;
		}else{
			if(start instanceof FileScan){
				//if it is a FileScan then set temp file to start
				//and set temp heap
				tFile = (FileScan)start;
				tHeap = tFile.file;
				while(tFile.hasNext()){
					index.insertEntry(new SearchKey(tFile.getNext().getField(col)), tFile.getLastRID());
				}
				tScan = new IndexScan(start.schema, index, tHeap);
			}else{
				tHeap = new HeapFile(null);
				while(start.hasNext()){
					tHeap.insertRecord(start.getNext().data);
				}
				tFile = new FileScan(start.schema, tHeap);
				while(tFile.hasNext()){
					index.insertEntry(new SearchKey(tFile.getNext().getField(col)), tFile.getLastRID());
				}
				tScan = new IndexScan(start.schema, index, tHeap);
			}
		}
		if(side){
			lScan = tScan;
		}else{
			rScan = tScan;
		}
		tFile.close();
	}
} // end class HashJoin;
