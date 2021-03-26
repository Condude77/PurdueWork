#ifndef MYQUEUE_H
#define MYQUEUE_H
#define MAX_SIZE 10000
#include<iostream>

using namespace std;
template <class T>
class myqueue{
  private:
        // Declare necessary variables and methods
	T arr[MAX_SIZE];
	int num;
	int first;
  public:
        myqueue(){
		num = 0;
		int first = 0;
	}
    	void enqueue(T x){
		if(num == MAX_SIZE){
			cout <<"error"<< endl;
		}else{
			arr[num+first] = x;
			num++;
		}
	}
    	T front(){
		if(num != 0){
			T data = arr[first];
			return data;
		}
	}
        T dequeue(){
		if(num != 0){
			num--;
			T data = arr[first];
			first++;
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
