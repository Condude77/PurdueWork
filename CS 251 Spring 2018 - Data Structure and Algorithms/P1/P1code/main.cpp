/**
 *  CS251 - Spring 2018: Project 1
 */

#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <sstream>

using namespace std;

int main(int argc, char** argv)
{
	/* Use input/output using C++ like notation would use "cin" and "cout" to read/write to stdin/stdout */

    /* ------------------------------- */
    /* process inputs and write output */
    /* ------------------------------- */

    /* Exit the program */  

    int colnum;
    cin >> colnum;
    int dashlength = ((colnum * 6) - 3);
    int n;
    
    while(!cin.eof()){
	
	for(int i = 0; i < dashlength; i++){
		cout << "-";
	}
	cout << "\r\n";
	for(int i = 0; i < colnum; i++){
		
		int n;
		cin >> n;
				
		if(n < 10){
			cout << "  ";
		}else if(n < 100){
			cout << " ";
		}
		
		if(i == (colnum-1)){
			cout << n << "\r\n";
		}
		else{
			cout << n << " | ";
		}
	}
    }
    for(int i = 0; i < dashlength; i++){
		cout << "-";
    }
    return 1;
}
