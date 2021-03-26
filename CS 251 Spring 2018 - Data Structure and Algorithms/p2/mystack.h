#ifndef MYSTACK_H
#define MYSTACK_H
#define MAX_SIZE 10000
#include<iostream>

using namespace std;

template <class T>
class mystack{
  private:
        // Declare necessary variables and methods
	T arr[MAX_SIZE];
	int num;
  public:
        mystack(){
		num = 0;
	}

        void push(T x){
		if(num == MAX_SIZE){
			cout << "error" << endl;
		}else{
			arr[num] = x;
			num++;
		}
	}
		
        T top(){
		if(num != 0){	
			T data = arr[num-1];
			return data;
		}
	}
        T pop(){
		if(num != 0){
			num--;
			T data = arr[num];
			return data;
		}
	}
        bool empty(){
		return num == 0;
	}
        int size(){
		return num;
	}
};

#endif
