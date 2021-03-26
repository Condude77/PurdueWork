
#include <stdio.h>

#include "array.h"

// Return sum of the array
double sumArray(int n, double * array) {
  double sum = 0;
  
  double * p = array;
  double * pend = p+n;

  while (p < pend) {
    sum += *p;
    p++;
  }

  return sum;
}

// Return maximum element of array
double maxArray(int n, double * array) {
	double* p = array;
	double* pend = p+n;
	double max = *p;
	while(p < pend){
		if(*p > max){
			max = *p;
		}
		p++;
	}
	return max;
}

// Return minimum element of array
double minArray(int n, double * array) {
	double* p = array;
	double* pend = p+n;
	double min = *p;
	while(p < pend){
		if(*p < min){
			min = *p;
		}
		p++;
	}
	return min;
}

// Find the position int he array of the first element x 
// such that min<=x<=max or -1 if no element was found
int findArray(int n, double * array, double min, double max) {
 	double* p = array;
	double* pend = p+n;
	int pos = 0;
	while(p < pend){
		if(*p >= min && *p <= max){
			return pos;
		}
		pos++;
		p++;
	}
 	return -1;
}

// Sort array without using [] operator. Use pointers 
// Hint: Use a pointer to the current and another to the next element
int sortArray(int n, double * array) {
	double* p = array;
	double* pend = p+n;
	double* q = p+1;
	double temp;
	for(p = array; p < pend; p++){
		for(q = p; q < pend - 1; q++){
			if(*p > *q){
				temp = *q;
			*q = *p;
			*p = temp;
			}
		}
	}
	return 0;
}

// Print array
void printArray(int n, double * array) {
	int i = 0;
	double* p = array;
	double* pend = p+n;
	while(p < pend){
		printf("%d:%.6f\n", i, *p);
		i++;
		p++;
	}
}

