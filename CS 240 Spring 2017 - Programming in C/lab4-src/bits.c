#include <stdio.h>
#include <math.h>

// It prints the bits in bitmap as 0s and 1s.
void printBits(unsigned int bitmap)
{
	// Add your code here
	int i = 31;
	int pos;
	while(i >= 0){			//prints the decimal value of the number
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
		printf("%d", i % 10);
		i--;
	}
	printf("\n");
}


// Sets the ith bit in *bitmap with the value in bitValue (0 or 1)
void setBitAt( unsigned int *bitmap, int i, int bitValue ) {
	// Add your code here
	if(bitValue == 1){
		*bitmap |= 1 << i;
	}else{
		*bitmap &= ~(1 << i);
	}
}

// It returns the bit value (0 or 1) at bit i
int getBitAt( unsigned int bitmap, unsigned int i) {
	// Add your code here
	return (bitmap >> i) & 1; //shifts the bitmap to that point and then compares if is 1 or not
}

// It returns the number of bits with a value "bitValue".
// if bitValue is 0, it returns the number of 0s. If bitValue is 1, it returns the number of 1s.
int countBits( unsigned int bitmap, unsigned int bitValue) {
	// Add your code here
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

// It returns the number of largest consecutive 1s in "bitmap".
// Set "*position" to the beginning bit index of the sequence.
int maxContinuousOnes(unsigned int bitmap, int * position) {
	// Add your code here
	int highest = 0;
	int curr = 0;
	int highpos = 0;
	int curpos;
	int i = 31;
	while(i >= 0){
		curpos = bitmap >> i;
		if((curpos & 1) && curr == 0){
			curr++;
		}else if(!(curpos & 1)){
			if(curr > highest){
				highest = curr;
				highpos = i;
				curr = 0;
			}else{
				curr = 0;
			}
		}else if((curpos & 1) && curr != 0){
			curr++;
		}
		i--;
	}
	*position = highpos + 1;
	return highest;

}


