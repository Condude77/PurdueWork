
#include <stdlib.h>
#include "mystring.h"

// Type "man string" to see what every function expects.

int mystrlen(char * s) {
	if(s == NULL){
		return 0;
	}
	int i = 0;
	while (*s){
		s++;
		i++;
	}
	return i;
}

char * mystrcpy(char * dest, char * src) {
	if(src == NULL){
		return NULL;
	}
	char * i = dest;
	while(*dest = *src){
		dest++;
		src++;
	}
	return i;
}

char * mystrcat(char * dest, char * src) {
	int i = 0;
	int j = 0;
	while(dest[i] != '\0'){
		i++;
	}
	while(src[j] != '\0'){
		dest[i] = src[j];
		i++;
		j++;
	}
	dest[i] = '\0';
	return dest;
}

int mystrcmp(char * s1, char * s2) {
	while(*s1 != '\0'){
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
	int i;
	int j;
	i = 0;
	while(hay[i]){
		j = 0;
		while(hay[j+i] == needle[j]){
			if(needle[j+1] == '\0'){
				return(&hay[i]);
			}
			j++;
		}
		i++;
	}
	return NULL;
}

char * mystrdup(char * s) {
	size_t len = mystrlen(s);
	char *res = malloc(len + 1);
	size_t i = 0;
	while(i <= len){
		res[i] = s[i];
		i++;
	}
	return res;
}

char * mymemcpy(char * dest, char * src, int n)
{
	char *csrc = (char *)src;
	char *cdest = (char *)dest;
	int i = 0;
	while(i < n){
		cdest[i] = csrc[i];
		i++;
	}
	return cdest;
}

