
#include <stdio.h>
#include <stdlib.h>

//
// Separates the file into words
//

#define MAXWORD 200
char word[MAXWORD];
int wordLength;

// It returns the next word from fd.
// If there are no more more words it returns NULL. 
char * nextword(FILE * fd) {
  	int c = 0;
	char * word = malloc(sizeof(char) * MAXWORD);
	word[c] = fgetc(fd);
	while(word[c] != EOF){// While it is not EOF read char
		while(word[c] == ' ' || word[c] == '\n' || word[c] == '\t' || word[c] == '\r'){
			word[c] = fgetc(fd);
		}
		while(word[c] != ' ' && word[c] != '\n' && word[c] != '\t' && word[c] != '\r' && word[c] != EOF){// While it is not EOF and it is a non-space char
			c++;
			word[c] = fgetc(fd);
		}
		wordLength = 0;
		while(word[wordLength] != EOF){
			if(word[wordLength] <= 32 || word[wordLength] >= 127){
				word[wordLength] = '\0';
				break;
			}
			wordLength++;
		}
		if(word[0] == EOF){
			return NULL;
		}
		return word;
	}
	return NULL;
}

