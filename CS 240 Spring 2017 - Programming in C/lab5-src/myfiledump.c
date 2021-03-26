
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void filedump(char * p , int len) {
	int count = 1;
	printf("0x%016lX: ", (unsigned long) p * 0);
      	for(int i = 0; i < len-1; i++){
       		int c = p[i]&0xFF;
        	printf("%02X ", c);
	 	int j = i+1;
	  	if (j % 16 == 0 && len/16 >= 1) {
	   		printf("%1c", ' ');
	 		for(int k = 0; k < 16 && j < 17; k++){
	     			int c = p[k]&0xFF;
		 		printf("%c", (c>=32&&c<127)?c:'.');
		   	}
		    	for(int k = j - 16; k < j && j >= 17; k++){
		     		int c = p[k]&0xFF;
		      		printf("%c", (c>=32&&c<127)?c:'.');
		       	}
		        printf("\n");
			printf("0x%016lX: ", (unsigned long) p * 0 +(0x10*count++));
		}
	}
	int c = p[len - 1]&0xFF;
	printf("%02X ", c);
	printf("%1c", ' ');
	if(len % 16 == 0){
		for(int i = len-16; i < len; i++){
	     		int c = p[i]&0xFF;
	      		printf("%c", (c>=32&&c<127)?c:'.');
	       	}
	}else{
		for(int i = 0; i < 16 - len % 16; i++){
		  	printf("%3c", ' ');
		}
		for(int i = len - (len % 16); i < len; i++){
		     	int c = p[i]&0xFF;
		      	printf("%c", (c>=32&&c<127)?c:'.');
		}
	}
	printf("\n");
}

int
main(int argc, char **argv) {
	if(argc < 2){
		printf("Usage: filedump filename\n");
		exit(1);
	}
	char* fileName = argv[1];
	FILE* fd = fopen(fileName, "r");
	if(fd == NULL){
		printf("Error opening file \"%s\"\n", fileName);
		exit(1);
	}
	fseek(fd, 0L, SEEK_END);
	int fileSize = ftell(fd);
	fseek(fd, 0L, SEEK_SET);
	char * buffer;
	buffer = (char*)calloc(fileSize, sizeof(char));
	if(buffer == NULL){
		return 1;
	}
	fread(buffer, sizeof(char), fileSize, fd);
	fclose(fd);
	filedump(buffer, fileSize);
	free(buffer);
}


