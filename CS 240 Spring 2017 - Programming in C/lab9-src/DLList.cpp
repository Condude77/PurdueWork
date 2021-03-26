#include "DLList.h"

/**
 * @brief      Constructor for the DLList.
 */
DLList::DLList()
{
	/** @todo Write a constructor for a dllist. Check slides! **/
	head = new DLNode();
	head->next = head;
	head->prev = head;
}

/**
 * @brief      Destructor for the DLList.
 */
DLList::~DLList()
{
	/** @todo Clean up your mess! **/
	DLNode * e = head->next;
	while(e != head){
		DLNode * next = e->next;
		delete e;
		e = next;
	}
	delete head;
}

/**
 * @brief      Print the DLList line by line.
 */
void DLList::print()
{
	/** @todo Print this list line by line **/
	DLNode * e = head->next;
	while(e != head){
		std::cout << e->data << std::endl;
		e = e->next;
	}
}

/**
 * @brief      Sort and print the list.
 * 
 * This function should sort and print the list.
 * Note: the actual list is NOT to be modified.
 * In other words, it should only *print* in a
 * sorted order, not actually sort the list.
 */
void DLList::printSorted()
{
	/** @todo Print a sorted copy of this list **/
	DLNode * e = head->next;
	DLList * copy = new DLList();
	while(e != head){
		copy->insertFront(e->data);
		e = e->next;
	}
	DLNode * p = copy->head->next;
	/*while(p != copy->head){
		if(p->next->data >= p->data){
			p->data = p->next->data;
		}
		p = p->next;
	}
	p = copy->head->next;*/
	while(p != copy->head){
		std::cout << p->data << std::endl;
		p = p->next;
	}
}

/**
 * @brief      Add to the front of the list.
 *
 * @param[in]  data  Item to add to front.
 */
void DLList::insertFront(int data)
{
	/** @todo Insert to the front of the list **/
	DLNode *e = new DLNode();
	e->data = data;
	e->next = head->next;
	e->prev = head;
	e->next->prev = e;
	e->prev->next = e;
}

/**
 * @brief      Removes & stores the last element.
 *
 * The last element is removed and stored in the
 * referenced variable data.
 * 
 * @param      data  Thing in which we are storing the data.
 *
 * @return     True upon successful removal.
 */
bool DLList::removeLast(int & data)
{
	/** @todo Remove the last thing **/
	DLNode * e = head->prev;
	head->prev = e->prev;
	e->prev->next = head;
	data = e->data;
	delete e;
	return true;
}

/**
 * @brief      Difference of two lists.
 *
 * @param      list  Subtrahend.
 *
 * @return     Returns a pointer to the difference.
 */
DLList * DLList::difference(DLList & list)
{
	DLList * diff = new DLList();
	/** @todo Implement this function **/
	DLNode *e = head->next;
	while(e != head){
		DLNode *p = list.head->next;
		while(p != list.head){
			if(p->data == e->data){
				break;
			}else{
				p = p->next;
			}
		}
		if(p == list.head){
			diff->insertFront(e->data);
		}
		e = e->next;
	}
	return diff;
}

/**
 * @brief      Returns a sublist of items in a range.
 *
 * @param[in]  start  First index.
 * @param[in]  end    Second index.
 *
 * @return     Elements between first and second index.
 */
DLList * DLList::getRange(int start, int end)
{
	DLList * range = new DLList();
	/** @todo getRange **/
	return range;
}

/**
 * @brief      Intersection of this list and another list.
 *
 * @param      list  The other list.
 *
 * @return     Elements list shares with this one.
 */
DLList * DLList::intersection(DLList & list)
{
	DLList * inter = new DLList();
	/** @todo intersection **/
	DLNode *e = head->prev;
	while(e != head){
		DLNode *p = list.head->prev;
		while(p != list.head){
			if(p->data == e->data){
				inter->insertFront(e->data);
				break;
			}else{
				p = p->prev;
			}
		}
		e = e->prev;
	}
	return  inter;
}

/**
 * @brief      Removes nodes in the start-end range.
 *
 * @param[in]  start  First node.
 * @param[in]  end    Second node.
 */
void DLList::removeRange(int start, int end)
{
	/** @todo Remove a range of elements **/
}
