#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
int CVal(char c){
	if(c >= '0' && c <= '9')
		return (int) c - '0';
	else
		return (int) c - 'A' + 10;
}
char IVal(int i){
	if( i >= 0 && i <= 9)
		return (char)(i + '0');
	else
		return (char)(i - 10 + 'A');
}
void flip(char * str){
	int len = strlen(str);
	int i = 0;
	while(i < len / 2){
		char t = str[i];
		str[i] = str[len - i - 1];
		str[len - i - 1] = t;
		i++;
	}
}
void convert(int basefrom, int baseto, char * number){
	printf("Number read in base %d: %s\n" , basefrom, number);
	int len = strlen(number);
	int numbase10 = 0;
	int power = 1;
	int i = len - 1;
	while(i >= 0){
		if(CVal(number[i]) >= basefrom){
			printf("Wrong digit in number.\n");
			exit(1);
		}
		numbase10 += CVal(number[i]) * power;
		power = power * basefrom;
		i--;
	}
	printf("Converted to base 10: %d\n", numbase10);
	
	char res[100];
	int index = 0;
	while(numbase10 > 0){
		res[index++] = IVal(numbase10 % baseto);
		numbase10 = numbase10 / baseto;
	}
	res[index] = '\0';
	flip(res);
	printf("Converted to base %d: %s\n", baseto, res);
	/*if(basefrom != 10){
		int i = strlen(number);
		int temp2 = 0;
		while(i >= 0){
			temp2 = atoi(number[i]) * pow(basefrom, i);
			temp1 = temp1 + temp2;
			i--;
		}
	}
	if(baseto != 10){
		int b4remainder = temp1;
		char remainder;
		while(b4remainder != 0){
			remainder = remainder + (char) b4remainder % baseto;
			b4remainder = b4remainder / baseto;
		}
		temp1 = atoi(remainder);
	}
	return temp1;*/
}
int main(int argc, char ** argv)
{
	int basefrom;
	int baseto;
        char * number;
	if(argc < 4){
		printf("Usage:  convert <basefrom> <baseto> <number>\n");
		exit(1);
	}
	basefrom = atoi(argv[1]);
	baseto = atoi(argv[2]);
	number = argv[3];
	convert(basefrom, baseto, number);
	exit(0);
}
