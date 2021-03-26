#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "LinkedList.h"

//Initialize the linked list
void llist_init(LinkedList * list)
{
        list->head = NULL;
}

// Prints the linked list
void llist_print(LinkedList * list) {
        
        ListNode * e;

        if (list->head == NULL) {
                printf("{EMPTY}\n");
                return;
        }

        printf("{");

        e = list->head;
        while (e != NULL) {
                printf("%d", e->value);
                e = e->next;
                if (e!=NULL) {
                        printf(", ");
                }
        }
        printf("}\n");
}

// Problem ( 1/4 ) 
/******************************************************************************
 * TODO: Return the number of elements (ListNodes) in the linked list.
 *
 * Parameters: list -- find the number of elements contained in this single
 * linked list
 *
 * Return: the number of elements 
 *
 * Return Type: integer
 *****************************************************************************/
int llist_number_elements(LinkedList * list) {
    // Write Your Code Here
    ListNode *p = list->head;
    int i = 0;
    while(p != NULL){
    	p = p->next;
	i++;
    }
    return i;
}

// Problem ( 2/4 ) 
/******************************************************************************
 * TODO: Create a new ListNode and add it to the end of the single linked
 * 	 list. There is no check if the value exists in the list. 
 *
 * Parameters: list -- The single linked list to which you will insert the new
 * 		       ListNode to the end of.
 *             value -- The new ListNode will contain this value.
 *
 * Return: void
 * 
 * Return Type: void
 *****************************************************************************/
void llist_insert_last(LinkedList * list, int value) {
    // Write Your Code Here
	ListNode *p = list->head;
	ListNode *n = (ListNode*)malloc(sizeof(ListNode));
	n->value = value;
	if(p == NULL){
		list->head = n;
		list->head->next = NULL;
	}else{
		while(p->next != NULL){
			p = p->next;
		}
		p->next = n;
		n->next = NULL;
	}
}

// Problem ( 3/4 )
/******************************************************************************
 * TODO: Remove the ith entry from the single linked list.   	 
 * 
 * Parameters: list -- The single linked list to which you will be removing
 * 		       the listNode from.
 * 	       ith -- The index where you will remove the listNode from the
 * 	              single linked list. 
 *
 * Return: Return 1 if the ListNode is removed.
 * 	   Return 0 if the ith entry does not exist in the list or the list is
 * 	   empty.
 *
 * Return Type: integer
 *****************************************************************************/
int llist_remove_ith(LinkedList * list, int ith) {
    // Write Your Code Here
    ListNode *p = list->head;
    int i = 0;
    ListNode *prev = NULL;
    while(p != NULL){
    	if(i == ith){
		break;
	}
	prev = p;
	p = p->next;
	i++;
   }
   if(p == NULL){
   	return 0;
   }
   if(prev == NULL){
   	list->head = p->next;
   }else{
   	prev->next = p->next;
   }
   free(p);
   return 1;
}

// Problem ( 4/4 )
/******************************************************************************
 * TODO: Iterate through list1 and list2. Check if any values intersect between
 *       the two lists. If the values match, add the intersecting elements
 *       to list3. Use the function llist_add(LinkedList * list, int value)
 *       to add a value to the list.
 * 
 * Parameters: list1 -- singled linked list, find values that intersect with
 *                      list2
 *             list2 -- single linked list, find values that intersect with
 *                      list1
 *             list3 -- store the intersection of list1 and list2 in this list
 *
 * Return: void 
 *
 * Return Type: void
 *****************************************************************************/
void llist_intersection(LinkedList * list1, LinkedList * list2, LinkedList * list3) {
    // Write Your Code Here
	ListNode *p = list1->head;
	ListNode *t = (ListNode *)malloc(sizeof(ListNode));
	ListNode *i = list3->head;
	while(p != NULL){
		ListNode *n = list2->head;
		while(n != NULL){
			if(p->value == n->value){
				t->value = p->value;
				if(i == NULL){
					list3->head = t;
					list3->head->next = NULL;
				}else{
					i->next = t;
					t->next = NULL;
				}
			}
			n = n->next;
		}
		p = p->next;
	}
}
