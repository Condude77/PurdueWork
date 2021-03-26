#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_DIGITS 32

// Problem (1/2)
/*****************************************************************************
 * TODO: convert a number from the given base to decimal
 *
 * Parameters: number -- the number you are converting an integer
 *             base   -- the base of the number you are given
 * 
 * Return: The number as an integer
 *
 * Return Type: int
*****************************************************************************/
int CtoI(char c){
	if(c >= '0' && c <= '9'){
		return (int) (c - '0');
	}else{
		return (int) (c - 'A' + 10);
	}
}
int toInteger(char * number, int base) {
// Write Your Code Here
	int len = strlen(number);
	int power = 1;
	int i = len - 1;
	int newNum = 0;
	while(i >= 0){
		newNum += CtoI(number[i]) * power;
		power = power * base;
		i--;
	}
	return newNum;
}

// Problem (2/2)
/*****************************************************************************
 * TODO: convert a number from the given base to decimal
 *
 * Parameters: number -- the number you are converting a string
 *             base   -- the base you are converting the numebr to
 * 
 * Return: The number as a string in base "base"
 *
 * Return Type: char *
*****************************************************************************/
char ItoC(int i){
	if(i >= 0 && i <= 9){
		return (char)(i + '0');
	}else{
		return (char)(i - 10 + 'A');
	}
}
void flip(char * str){
	int len = strlen(str);
	int i = 0;
	while(i < len /2){
		char t = str[i];
		str[i] = str[len - i - 1];
		str[len - i - 1] = t;
		i++;
	}
}
char * toBase(int number, int base) {
	char res[100];
	int i = 0;
	while(number > 0){
		res[i++] = ItoC(number % base);
		number = number / base;
	}
	res[i] = '\0';
	flip(res);
	return res;
}
