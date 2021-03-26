#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pointer_to_function.h"

// Problem (1/3)
/*****************************************************************************
 * TODO Check if the key matches the value of the integer element
 *
 * Parameters: key     -- the key to match
 *             element -- an int cast as a void *
 *
 * Return: return 0 if they are not equal and 1 if they are equal
 *
 * Return Type: int
 ****************************************************************************/
int intMatches(int key, void * element) {
    // Write Your Code Here
    int temp = *((int *) element);
    if(key - temp == 0){
    	return 1;
    }
    return 0;
}

// Problem (2/3)
/*****************************************************************************
 * TODO Check if the key matches the student's id field
 * 
 * Parameters: key     -- the key to match
 *             element -- a student * cast as a void *
 *
 * Return: return 0 if the id in the student fields don't match and 1 if they do
 *
 * Return Type: int
 ****************************************************************************/
int studentIdMatches(int key, void * element) {
    // Write Your Code Here
    student s = *((student *) element);
    if(key == s.id){
    	return 1;
    }
    return 0;
}

// Problem (3/3)
/****************************************************************************
 * TODO: Check if there is an element in the array with a specific key
 *
 * Parameters: n           -- number of elements in the aray
 *             elementSize -- the size of each element
 *             array       -- the array of elements to sum
 *             key         -- the element you are looking for in the array
 *             result      -- if the item was found make result point to the 
 *                            item otherwise it should be null
 *             MatchesFunc -- function pointer to function that checks if an
 *                            entry in the array matches the key
 *
 * Return: 0 if the element was not in the array and 1 if it was
 *
 * Return Type: int
 ***************************************************************************/
int findElement(int n, int elementSize, void * array, int key, void * result, MatchesFunc matches) {
    // Write Your Code Here
    void * element;
    for(int i = 0; i < n; i++){
    	element = (void *) ((char *)array + i *elementSize);
    	if(matches(key, element)){
		memcpy(result, element, elementSize);
		return 1;
	}
    }
    return 0;
}
