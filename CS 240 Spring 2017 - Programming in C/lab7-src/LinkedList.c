
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "LinkedList.h"

//
// Initialize a linked list
//
void llist_init(LinkedList * list)
{
	list->head = NULL;
}

//
// It prints the elements in the list in the form:
// 4, 6, 2, 3, 8,7
//
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

//
// Appends a new node with this value at the beginning of the list
//
void llist_add(LinkedList * list, int value) {
	// Create new node
	ListNode * n = (ListNode *) malloc(sizeof(ListNode));
	n->value = value;
	
	// Add at the beginning of the list
	n->next = list->head;
	list->head = n;
}

//
// Returns true if the value exists in the list.
//
int llist_exists(LinkedList * list, int value) {
	ListNode *p;
	p = list->head;
	while(p != NULL){
		if((value - p->value) == 0){
			return 1;
		}
		p = p->next;
	}
	return 0;
}

//
// It removes the entry with that value in the list.
//
int llist_remove(LinkedList * list, int value) {
	ListNode *p;
	p = list->head;
	ListNode* prev = NULL;
	while(p != NULL){
		if((value - p->value) == 0){
			break;
		}
		prev = p;
		p = p->next;
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

//
// It stores in *value the value that correspond to the ith entry.
// It returns 1 if success or 0 if there is no ith entry.
//
int llist_get_ith(LinkedList * list, int ith, int * value) {
	ListNode *p = list->head;
	int i = 0;
	while(p != NULL){
		if(i == ith){
			*value = p->value;
			return 1;
		}
		p = p->next;
		i++;
	}
	return 0;
}

//
// It removes the ith entry from the list.
// It returns 1 if success or 0 if there is no ith entry.
//
int llist_remove_ith(LinkedList * list, int ith) {
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

//
// It returns the number of elements in the list.
//
int llist_number_elements(LinkedList * list) {
	int total = 0;
	ListNode *p = list->head;
	while(p != NULL){
		total++;
		p = p->next;
	}
	return total;
}


//
// It saves the list in a file called file_name. The format of the
// file is as follows:
//
// value1\n
// value2\n
// ...
//
int llist_save(LinkedList * list, char * file_name) {
	ListNode *p = list->head;
	FILE * fd = fopen(file_name, "ab+");
	if(fd == NULL){
		return 0;
	}else{
		while(p != NULL){
			fprintf(fd, "%d\n", p->value);
			p = p->next;
		}
	}
	fclose(fd);
	return 0;
}

//
// It reads the list from the file_name indicated. If the list already has entries, 
// it will clear the entries.
//
int llist_read(LinkedList * list, char * file_name) {
	FILE * fd = fopen(file_name, "r");
	ListNode *p = list->head;
	int read;
	if(fd == NULL){
		return 0;
	}
	if(p != NULL){
		llist_clear(list);
	}
	while(!feof(fd)){
		fscanf(fd, "%d\n", &read);
		llist_add(list, read);
	}
	fclose(fd);
	return 1;
}


//
// It sorts the list. The parameter ascending determines if the
// order si ascending (1) or descending(0).
//
void llist_sort(LinkedList * list, int ascending) {
	ListNode *p = list->head;
	ListNode *t = p->next;
	int temp;
	if(ascending == 1){
		while(p != NULL){
			while(t != NULL){
				if(p->value > t->value){
					temp = p->value;
					p->value = t->value;
					t->value = temp;
				}
				t = t->next;
			}
			p = p->next;
			t = p;
		}
	}
	if(ascending == 0){
		while(p != NULL){
			while(t != NULL){
				if(p->value < t->value){
					temp = p->value;
					p->value = t->value;
					t->value = temp;
				}
				t = t->next;
			}
			p = p->next;
			t = p;
		}
	}
}

//
// It removes the first entry in the list and puts value in *value.
// It also frees memory allocated for the node
//
int llist_remove_first(LinkedList * list, int * value) {
	ListNode *p = list->head;
	if(p != NULL){
		*value = p->value;
		list->head = p->next;
		free(p);
		return 1;
	}
	return 0;
}

//
// It removes the last entry in the list and puts value in *value/
// It also frees memory allocated for node.
//
int llist_remove_last(LinkedList * list, int *value) {
  	ListNode *p = list->head;
	if(list->head == NULL){
		return 0;
	}
	while(p->next->next != NULL){
		p = p->next;
	}
	*value = p->next->value;
	free(p->next);
	p->next = NULL;
	return 1;
}

//
// Insert a value at the beginning of the list.
// There is no check if the value exists. The entry is added
// at the beginning of the list.
//
void llist_insert_first(LinkedList * list, int value) {
	ListNode *n = (ListNode *)malloc(sizeof(ListNode));
	ListNode *p = list->head;
	n->value = value;
	if(p == NULL){
		p = n;
		n->next = NULL;
	}else{
		list->head = n;
		n->next = p;
	}
}

//
// Insert a value at the end of the list.
// There is no check if the name already exists. The entry is added
// at the end of the list.
//
void llist_insert_last(LinkedList * list, int value) {
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

//
// Clear all elements in the list and free the nodes
//
void llist_clear(LinkedList *list)
{
	ListNode *p = list->head;
	while(p != NULL){
		p = p->next;
		if(p == NULL){
			list->head = NULL;
			break;
		}
		free(p);
	}
}
