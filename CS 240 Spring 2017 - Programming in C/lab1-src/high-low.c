#include <stdio.h>

int main() {
  printf("Welcome to the High Low game...\n");
  while (1){
	int low = 1;
	int high = 100;
	int mid = (low+high)/2;
	printf("Think of a number between 1 and 100 and press <enter>");
	getchar();
	while(1){
		printf("Is it higher than %d? (y/n)\n" ,mid);
		char a;
		a = getchar();
		if (a == 'y'){
			low = mid + 1;
		}else if (a == 'n'){
			high = mid - 1;
		}else{
			printf("Type y or n\n");
		}
		mid = (low+high)/2;
		getchar();
		if(high<low){
			break;
		}
	}
	printf("\n>>>>>> The number is %d\n", low);
	printf("\nDo you want to continue playing (y/n)?");
	char a;
	a = getchar();
	if (a == 'n'){
		break;
	}
	getchar();
  }
  printf("Thanks for playing!!!\n");

}

