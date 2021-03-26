
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void mymemdump(FILE * fd, char * p , int len) {
    // Add your code here.
    // You may see p as an array.
    // p[0] will return the element 0 
    // p[1] will return the element 1 and so on
	int count = 1;
    	fprintf(fd, "0x%016lX: ", (unsigned long) p); // Print address of the beginning of p. You need to print it every 16 byte	
    	for(int i = 0; i < len-1; i++){
		int c = p[i]&0xFF;
		fprintf(fd, "%02X ", c);
		int j = i+1;
		if (j % 16 == 0 && len/16 >= 1) {
			fprintf(fd,"%1c", ' ');
			for(int k = 0; k < 16 && j < 17; k++){
       	 			int c = p[k]&0xFF; // Get value at [p]. The &0xFF is to make sure you truncate to 8bits or one byte.
        			fprintf(fd, "%c", (c>=32&&c<=127)?c:'.');
			}
			for(int k = j - 16; k < j && j >= 17; k++){
				int c = p[k]&0xFF;
				fprintf(fd, "%c", (c>=32&&c<=127)?c:'.');
			}
			fprintf(fd, "\n");
			fprintf(fd, "0x%016lX: ", (unsigned long) p +(0x10*count++));
		}
	}
	int c = p[len - 1]&0xFF;
	fprintf(fd, "%02X ", c);
	fprintf(fd,"%1c", ' ');
	if(len % 16 == 0){
		for(int i = len-16; i < len; i++){
			int c = p[i]&0xFF;
			fprintf(fd, "%c", (c>=32&&c<=127)?c:'.');
		}
	}else{
		for(int i = 0; i < 16 - len % 16; i++){
			fprintf(fd, "%3c", ' ');
		}
		for(int i = len - (len % 16); i < len; i++){
			int c = p[i]&0xFF;
			fprintf(fd, "%c", (c>=32&&c<=127)?c:'.');
		}
	}
	fprintf(fd, "\n");
	
}

