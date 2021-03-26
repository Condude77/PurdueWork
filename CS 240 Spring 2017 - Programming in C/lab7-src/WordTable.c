
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "WordTable.h"

// Initializes a word table
void wtable_init(WordTable * wtable)
{
	// Allocate and initialize space for the table
	wtable->nWords = 0;
	wtable->maxWords = 10;
	wtable->wordArray = (WordInfo *) malloc(wtable->maxWords * sizeof(WordInfo));
	for (int i = 0; i < wtable->maxWords; i++) {
		llist_init(&wtable->wordArray[i].positions);
	}
}

// Add word to the tableand position. Position is added to the corresponding linked list.
void wtable_add(WordTable * wtable, char * word, int position)
{
	// Find first word if it exists
	for (int i = 0; i < wtable->nWords; i++) {
		if ( strcmp(wtable->wordArray[i].word, word)== 0 ) {
			// Found word. Add position in the list of positions
			llist_insert_last(&wtable->wordArray[i].positions, position);
			return;
		}
	}

	// Word not found.

	// Make sure that the array has space.
	// Expand the wordArray here.
	if(wtable->nWords == wtable->maxWords){
		wtable->maxWords = (wtable->maxWords) * 2;
		wtable->wordArray = (WordInfo *) realloc(wtable->wordArray, (wtable->maxWords) * sizeof(WordInfo));
	}

	// Add new word and position
	wtable->wordArray[wtable->nWords].word = strdup(word);
	llist_insert_last(&wtable->wordArray[wtable->nWords].positions, position);
	wtable->nWords++;
}

// Print contents of the table.
void wtable_print(WordTable * wtable, FILE * fd)
{
	fprintf(fd, "------- WORD TABLE -------\n");

	// Print words
	for (int i = 0; i < wtable->nWords; i++) {
		fprintf(fd, "%d: %s: ", i, wtable->wordArray[i].word);
		llist_print( &wtable->wordArray[i].positions);
	}
}

// Get positions where the word occurs
LinkedList * wtable_getPositions(WordTable * wtable, char * word)
{
	// Write your code here
	for(int i = 0; i < wtable-> nWords; i++){
		if(strcmp(wtable->wordArray[i].word, word) == 0){
			return &wtable->wordArray[i].positions;
		}
	}
	return NULL;
}

//
// Separates the string into words
//

#define MAXWORD 200
char word[MAXWORD];
int wordLength;
int wordCount;
int charCount;
int wordPos;

// It returns the next word from stdin.
// If there are no more more words it returns NULL.
// A word is a sequence of alphabetical characters.
static char * nextword(FILE * fd) {
	// Write your code here
	char c;
	int i = 0;
	wordLength = 0;
	while((c = fgetc(fd)) != EOF){
		charCount++;
		if(((c>='A'&&c<='Z')||(c>='a'&&c<='z')) && c != '\n' && c != '\t' && c != ' '){
			word[i] = c;
			i++;
			wordLength++;
		}else if(i != 0){
			wordPos = charCount - wordLength - 1;
			word[i] = '\0';
			i = 0;
			return word;
		}
	}
	return NULL;
}

// Conver string to lower case
void toLower(char *s) {
	// Write your code here
	if(s == NULL){
		return;
	}
	int i = 0;
	char c = s[i];
	while(c != '\0'){
		if(c >= 'A' && c <= 'Z'){
			c = (c - 'A') + 'a';
			s[i] = c;
		}
		i++;
		c = s[i];
	}
}


// Read a file and obtain words and positions of the words and save them in table.
int wtable_createFromFile(WordTable * wtable, char * fileName, int verbose)
{
	// Write your code here
	FILE *fd = fopen(fileName, "r");
	if(fd == NULL){
		return 0;
	}
	char * c;
	int count = 0;
	while((c = nextword(fd)) != NULL){
		toLower(c);
		if(verbose == 1){
			printf("%d: word=%s, pos=%d\n", count, c, wordPos);
		}
		wtable_add(wtable, c, wordPos);
		count++;
	}
	return 0;
}

// Sort table in alphabetical order.
void wtable_sort(WordTable * wtable)
{
	// Write your code here
	int i = 0;
	while(i < wtable->nWords - 1){
		int j = i + 1;
		while(j < wtable->nWords){
			if(strcmp(wtable->wordArray[i].word, wtable->wordArray[j].word) >= 1){
				char *temp = wtable->wordArray[i].word;
				wtable->wordArray[i].word = wtable->wordArray[j].word;
				wtable->wordArray[j].word = temp;
				LinkedList pos = wtable->wordArray[i].positions;
				wtable->wordArray[i].positions = wtable->wordArray[j].positions;
				wtable->wordArray[j].positions = pos;
			}
			j++;
		}
		i++;
	}
}

// Print all segments of text in fileName that contain word.
// at most 200 character. Use fseek to position file pointer.
// Type "man fseek" for more info. 
int wtable_textSegments(WordTable * wtable, char * word, char * fileName)
{
	// Write your code here
	printf("===== Segments for word \"%s\" in book \"%s\" =====\n", word, fileName);
	LinkedList *pos = wtable_getPositions(wtable, word);
	ListNode *p;
	p = pos->head;
	FILE * fd = fopen(fileName, "r");
	while(p != NULL){
		printf("---------- pos=%d-----\n......", p->value);
		fseek(fd, p->value, SEEK_SET);
		for(int i = 0; i < MAXWORD; i++){
			char c = fgetc(fd);
			printf("%c", c);
		}
		printf("......\n");
		p = p->next;
	}
}

