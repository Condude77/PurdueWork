
#include <stdio.h>
#include "stack.h"
#include <stdlib.h>

int top=0;
double stack[MAXSTACK];

void stack_clear() 
{
  	top = 0;
  	int i = 0;
  	while(stack[i] != 0){
  		stack[i] = 0;
  	}
}

double stack_pop()
{
	// Add implementation here`
	if(top == 0){
		return 0;
	}
	double temp = stack[--top];
	return temp;
}

void stack_push(double val)
{
	// Add implementation here
	/*top++;
	int bot = top - 1;
	if(bot == -1){
		stack[0] = val;
	}else if(bot != -1){
		stack[bot] = val;
	}
	//top++;*/
	if(top < MAXSTACK - 1){
		stack[top++] = val;
	}
}

void stack_print()
{
	printf("Stack:\n");
	if(top != 0){
		for(int c = 0; c < top; c++){
			printf("%d: %0.6f\n", c, stack[c]);
		}
	}else{
		printf("Stack is empty");
	}
}

int stack_top()
{
  return top;
}

int stack_max()
{
  return MAXSTACK;
}

int stack_is_empty()
{
  return top == 0;
}


