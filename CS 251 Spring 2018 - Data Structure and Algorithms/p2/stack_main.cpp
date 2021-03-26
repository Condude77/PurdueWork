#include<iostream>
#include<string>
#include "mystack.h"

using namespace std;

int main(){
	string type;
	cin >> type;
	if(type == "int"){
		mystack<int> stack;
		string cmd;
	        cin >> cmd;
	        while(!cin.eof()){
	                if(cmd == "push"){
	                        int x;
	                        cin >> x;
	                        stack.push(x);
	                }else if(cmd == "top"){
	                        if(stack.empty() == 1){
					cout << "error" << endl;
				}else{
					cout << stack.top() << endl;
				}
	                }else if(cmd == "pop"){
	                        if(stack.empty() == 1){
                                        cout << "error" << endl;
                                }else{  
					cout << stack.pop() << endl;
				}
	                }else if(cmd == "empty"){
	                        cout << stack.empty() << endl;
	                }else if(cmd == "size"){
	                        cout << stack.size() << endl;
	                }else{
	                        cout << "error" << endl;
	                }
	                cin >> cmd;
	        }
	}else if(type == "double"){
		mystack<double> stack;
		string cmd;
	        cin >> cmd;
	        while(!cin.eof()){
	                if(cmd == "push"){
	                        double x;
	                        cin >> x;
	                        stack.push(x);
	                }else if(cmd == "top"){
	                        if(stack.empty() == 1){
                                        cout << "error" << endl;
                                }else{  
					cout << stack.top() << endl;
				}
	                }else if(cmd == "pop"){
	                        if(stack.empty() == 1){
                                        cout << "error" << endl;
                                }else{  
					cout << stack.pop() << endl;
	                	}
			}else if(cmd == "empty"){
	                        cout << stack.empty() << endl;
	                }else if(cmd == "size"){
	                        cout << stack.size() << endl;
	                }else{
	                        cout << "error" << endl;
	                }
	                cin >> cmd;
	        }
	}else if(type == "char"){
		mystack<char> stack;
		string cmd;
	        cin >> cmd;
	        while(!cin.eof()){
	                if(cmd == "push"){
	                        char x;
	                        cin >> x;
	                        stack.push(x);
	                }else if(cmd == "top"){
	                        if(stack.empty() == 1){
                                        cout << "error" << endl;
                                }else{  
					cout << stack.top() << endl;
	                	}
			}else if(cmd == "pop"){
	                        if(stack.empty() == 1){
                                        cout << "error" << endl;
                                }else{  
					cout << stack.pop() << endl;
				}
	                }else if(cmd == "empty"){
	                        cout << stack.empty() << endl;
	                }else if(cmd == "size"){
	                        cout << stack.size() << endl;
	                }else{
	                        cout << "error" << endl;
	                }
	                cin >> cmd;
	        }
	}else if(type == "string"){
		mystack<string> stack;
		string cmd;
	        cin >> cmd;
	        while(!cin.eof()){
	                if(cmd == "push"){
	                        string x;
	                        cin >> x;
	                        stack.push(x);
	                }else if(cmd == "top"){
	                        if(stack.empty() == 1){
                                        cout << "error" << endl;
                                }else{  
					cout << stack.top() << endl;
				}
	                }else if(cmd == "pop"){
	                        if(stack.empty() == 1){
                                        cout << "error" << endl;
                                }else{  
					cout << stack.pop() << endl;
				}
	                }else if(cmd == "empty"){
	                        cout << stack.empty() << endl;
	                }else if(cmd == "size"){
	                        cout << stack.size() << endl;
	                }else{
	                        cout << "error" << endl;
	                }
	                cin >> cmd;
	        }
	}
	cout << "\n";
	return 1;
}
