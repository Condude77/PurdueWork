#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Problem (1/4)
/******************************************************************************
 * TODO: Print the array.
 *       The format should be (array index)(colon)(array element)(newline) 
 * 
 * Parameters: n -- the number of elements in the array
 *             array -- a double array
 *
 * Return: void 
 *
 * Return Type: void
 *****************************************************************************/
void printArray(int n, double * array) {
// Write Your Code Here
	for(int i = 0; i < n; i++){
		printf("%d:%.6f\n", i, array[i]);
	}
}

// Problem (2/4)
/******************************************************************************
 * TODO: Return the minimum element of array 
 * 
 * Parameters: array -- a double array
 *             n -- the number of elements in the array
 *
 * Return: minimum element in array 
 *
 * Return Type: double
 *****************************************************************************/
double minArray(int n, double * array) {
// Write Your Code Here
	double curMin = array[0];
	for(int i = 0; i < n; i++){
		if(curMin > array[i]){
			curMin = array[i];
		}
	}
	return curMin;
}

// Problem ( 3/4 ) 
/******************************************************************************
 * TODO: Reverse the given string 'str'. 
 * E.g. reverse_str("smile") should return "elims"
 * 
 * Parameters: str -- The given string to be reversed.
 *
 * Return: A pointer to str, str should be reversed 
 *
 * Return Type: char pointer
 *****************************************************************************/
char * reverse_str (char * str ) {
    // Write Your Code Here
    int length = strlen(str);
    char rev[length];
    int j;
    for(int i = length-1, j =0; j < length/2; i--, j++){
    	rev[j] = str[j];
	str[j] = str[i];
	str[i] = rev[j];
    }
    return str;
}

// Problem ( 4/4 ) 
/******************************************************************************
 * TODO: Determine if the string str2 is a permutation of str1. A permutation
 * is the rearranging of characters in a different order. 
 * E.g. the string "act" is a permutation of "cat" 
 *
 * Hint: count the occurences of each letter
 * 
 * Parameters: str1 -- The original string
 *	       str2 -- Determine if this string is a permutation of str1 
 *
 * Return: 1 if str2 is a permutation
 *         0 if str2 is not a permutation
 *
 * Return Type: integer
 *****************************************************************************/
int is_permutation ( char * str1, char * str2 ) {
    // Write Your Code Here    
    int len1 = strlen(str1);
    int len2 = strlen(str2);
    if(len1 - len2 != 0){
    	return 0;
    }
    int isGood = 1;
    for(int i = 0; i < len1; i++){
    	char cur = str1[i];
	int j = 0;
    	for(j = 0; j < len1; j++){
		if(cur == str2[j]){
			break;
		}
	}
	if(cur != str2[j]){
		isGood = 0;
		break;
	}
    }
    return isGood;
}
