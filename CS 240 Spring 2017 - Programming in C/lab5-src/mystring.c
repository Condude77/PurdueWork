
#include <stdlib.h>
#include "mystring.h"

int mystrlen(char * s) {
	int len = 0;
	while(*s++ != '\0'){
		len++;
	}
	return len;
}

char * mystrcpy(char * dest, char * src) {
	char* p = src;
	char* q = dest;
	while(*p){
		*q = *p;
		q++;
		p++;
	}
	*q = '\0';
	return dest;
}

char * mystrcat(char * dest, char * src) {
	char* p = src;
	char* q = dest;
	while(*q){
		q++;
	}
	while(*p){
		*q = *p;
		p++;
		q++;
	}
	*q = '\0';
	return dest;
}

int mystrcmp(char * s1, char * s2) {
	while(*s1){
		if(*s2 == '\0'){
			return 1;
		}
		if(*s2 > *s1){
			return -1;
		}
		if(*s1 > *s2){
			return 1;
		}
		s1++;
		s2++;
	}
	if(*s2 != '\0'){
		return -1;
	}
	return 0;
}

char * mystrstr(char * hay, char * needle) {
	char *cp = hay;
	char *q;
	char *p;
	while(*cp){
		p = cp;
		q = needle;
		while(*p && *q && (*p == *q)){
			p++;
			q++;
		}
		if(!*q){
			return cp;
		}
		cp++;
	}
	return NULL;
}

char * mystrdup(char * s) {
	size_t len = mystrlen(s);
	char* res = malloc(len + 1);
	char* p = s;
	char* q = res;
	while(*p){
		*q = *p;
		q++;
		p++;
	}
	*q = '\0';
	return res;
}

char * mymemcpy(char * dest, char * src, int n)
{
	char* p = src;
	char* q = dest;
	int i = 0;
	while(i < n){
		*q = *p;
		p++;
		q++;
		i++;
	}
	return q;
}

