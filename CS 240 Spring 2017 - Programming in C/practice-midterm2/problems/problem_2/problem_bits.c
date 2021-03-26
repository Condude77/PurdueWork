#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Problem (1/4)
/******************************************************************************
 * TODO: Print the bits in bitmap as 0s and 1s
 * 
 * Parameters: bitmap -- print the binary representation of this number
 *
 * Return: void 
 *
 * Return Type: void
 *****************************************************************************/
void printBits(unsigned int bitmap)
{
    // Write Your Code Here
	int i = 31;
	int pos;
	while(i >= 0){
		pos = bitmap >> i;
		if(pos & 1){
			printf("1");
		}else{
			printf("0");
		}
		i--;
	}
	printf("\n");
	i = 31;
	while(i >= 0){
		printf("%d", i%10);
		i--;
	}
	printf("\n");
}


// Problem (2/4)
/******************************************************************************
 * TODO: Set the ith bit in *bitmap with the value in bitValue (0 or 1)
 * 
 * Parameters: bitmap -- unsigned integer
 *	       i -- index of the bit to be changed
 *             bitValue -- change the ith bit to this value
 *
 * Return: void 
 *
 * Return Type: void
 *****************************************************************************/
void setBitAt( unsigned int *bitmap, int i, int bitValue ) 
{
    // Write Your Code Here
	if(bitValue == 1){
		*bitmap |= 1 << i;
	}else{
		*bitmap &= ~(1 << i);
	}
}

// Problem (3/4)
/******************************************************************************
 * TODO: Return the bit value (0 or 1) at the ith bit of the bitmap
 * 
 * Parameters: bitmap -- unsigned integer
 *	       i -- index of the bit to be retrieved
 *
 * Return: the ith bit value 
 *
 * Return Type: integer
 *****************************************************************************/
int getBitAt( unsigned int bitmap, unsigned int i) {
    // Write Your Code Here 
    return (bitmap >> i) & 1;
}

// Problem (4/4)
/******************************************************************************
 * TODO: Return the number of bits with the value 'bitValue'
 *	 If the bitValue is 0, return the number of 0s. If the bitValue is 1,
 *	 return the number of 1s.
 * 
 * Parameters: bitmap -- unsigned integer
 *	       bitValue -- count how many times this number, either 0 or 1,
 *	       appears in bitmap
 *
 * Return: count of 0s or 1s in bitmap 
 *
 * Return Type: integer
 *****************************************************************************/
int countBits( unsigned int bitmap, unsigned int bitValue) {
    // Write Your Code Here 
   int total = 0;
   int pos;
   int i = 31;
   while(i >= 0){
   	pos = bitmap >> i;
	if(bitValue){
		if(pos & 1){
			total++;
		}
	}else{
		if(!(pos & 1)){
			total++;
		}
	}
	i--;
    }
    return total;
}

