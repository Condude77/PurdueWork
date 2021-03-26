
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <math.h>

#include "rpn.h"
#include "nextword.h"
#include "stack.h"

double rpn_eval(char * fileName, double x) {
	FILE *fd;
	fd = fopen(fileName, "r");
	char * word = malloc(sizeof(char) * 100); 
	word = nextword(fd);
	int underflowCheck = 1;
	int xCheck = 0;
	while(word != NULL && word[0] != EOF){
		double a, b, c;
		switch(word[0]){
		case '+':
			a = stack_pop();
			b = stack_pop();
			if(a != 0 && b != 0){
				c = b + a;
				stack_push(c);
			}else{
				underflowCheck = 0;
			}
			break;
		case '-':
			a = stack_pop();
			b = stack_pop();
			if(a != 0 && b != 0){
				c = b - a;
				stack_push(c);
			}else{
				underflowCheck = 0;
			}
			break;
		case '/':
			a = stack_pop();
			b = stack_pop();
			if(a != 0){
				c = b / a;
				stack_push(c);
			}else{
				underflowCheck = 0;
			}
			break;
		case '*':
			a = stack_pop();
			b - stack_pop();
			if(a != 0 && b != 0){
				c = b * a;
				stack_push(c);
			}else{
				underflowCheck = 0;
			}
			break;
		case 'c':
			a = stack_pop();
			if(a != 0){
				c = cos(a);
				stack_push(c);
			}else{
				underflowCheck = 0;
			}
			break;
		case 's':
			a = stack_pop();
			if(a != 0){
				c = sin(a);
				stack_push(c);
			}else{
				underflowCheck = 0;
			}
			break;
		case 'p':
			a = stack_pop();
			b = stack_pop();
			if(a > 0 && b != 0){
				c = pow(b, a);
				stack_push(c);
			}else{
				underflowCheck = 0;
			}
			break;
		case 'e':
			a = stack_pop();
			if(a != 0){
				c = exp(a);
				stack_push(c);
			}else{
				underflowCheck = 0;
			}
			break;
		case 'l':
			a = stack_pop();
			if(a > 0){
				c = log(a);
				stack_push(c);
			}else{
				underflowCheck = 0;
			}
			break;
		case 'x':
			stack_push(x);
			xCheck = 1;
			break;
		default:
			stack_push(atof(word));
			break;
		}
		word = nextword(fd);
	}
	double res = stack_pop();
	double emptyCheck = stack_pop();
	if(emptyCheck != 0){
		printf("Elements remain in the stack\n");
		exit(1);
	}else if(!underflowCheck){
		if(!xCheck){
			printf("Stack underflow\n");
			exit(1);
		}
	}else{
		return res;
	}
}

