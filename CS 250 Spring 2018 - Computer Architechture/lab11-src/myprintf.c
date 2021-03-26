#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printx(int);
void printd(int);

int myprintf(const char* format, ...){
	int len = strlen(format);
	int *arg = (int*)&format;
	arg++;
	
	for(int i = 0; i < len; i++){
		if(format[i] == '%'){
			if(format[i+1] == '%'){
				putchar('%');
			}else if(format[i+1] == 'c'){
				int c = *arg;
				arg++;
				i++;
				putchar(c);
			}else if(format[i+1] == 's'){
				char *str = *((char**)arg);
				int strlength = strlen(str);
				for(int j = 0; j < strlength; j++){
					putchar(str[j]);
					
				}
				arg++;
				i++;
			}else if(format[i+1] == 'x'){
				int x = *arg;
				printx(x);
				arg++;
				i++;
			}else if(format[i+1] == 'd'){
				int d = *arg;
				printd(d);
				arg++;
				i++;
			}
		}else{
			putchar(format[i]);
		}
	}
}
