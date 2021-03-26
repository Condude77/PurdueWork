/*
 *  CS 24000 First mid-semester exam.
 *   Problem #29
 *
 *   unsigned int emptyLinesCounter(const char * str)
 *
 *  Arguments:
 *		char * str -- a pointer to a string
 *		
 *  Description:
 *		Given a string of input, counts number of empty lines in str. An
 *		empty line is defined as a line that contain no character except for newline 
 *		('\n') character or that may contain only white spaces (' ', '\t', '\r') 
 *		and a newline character ('\n'))
 *
 */

 /* ======================== Start your edits here =========================== */
 
 /*
  *  NOTE NOTE NOTE NOTE NOTE
  *  You are not allowed to add any other libraries or library includes in addition
  *   to <stdlib.h> other than <math.h> (if you believe you need it).
  */
 #include <stdlib.h>
 
 /* 
  * Complete the function below for the exam
  */

unsigned int emptyLinesCounter(const char * str)
 {
	int total = 0;
	int i = 0;
	while(str[i] != '\0'){
		if(str[i] == '\n'){
			if(str[i-1] == ' ' || str[i-1] == '\t' || str[i-1] == '\r' || str[i-1] == '\n'){
				total++;
			}
		}
		i++;
	}
	if(str[i] == '\0'){
		if(str[i-1] == ' ' || str[i-1] == '\t' || str[i-1] == '\r' || str[i-1] == '\n'){
			total++;
		}
	}
	return total;
 }
  
 /* ======================== End your edits here =========================== */

 /*
 ************************************************************************
  *  Do NOT change any of the lines between here and the end of the file!
 ************************************************************************
  */

#include <stdio.h>
#include <assert.h>

char *Usage = "Usage: %s <string>\n";

int main(int argc, char *argv[])
{
	unsigned int result;
	
	if (argc != 2) 
	{
		fprintf(stderr, Usage, argv[0]);
		exit(1);
	}
	
	result = emptyLinesCounter(argv[1]);
	printf("Empty lines: %u\n", result);
	
	return 0;
}
	
	
