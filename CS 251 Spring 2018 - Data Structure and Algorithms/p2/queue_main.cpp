#include<iostream>
#include<string>
#include "myqueue.h"

using namespace std;

int main(){
        string type;
        cin >> type;
        if(type == "int"){
                myqueue<int> queue;
                string cmd;
                cin >> cmd;
                while(!cin.eof()){
                        if(cmd == "enqueue"){
                                int x;
                                cin >> x;
                                queue.enqueue(x);
                        }else if(cmd == "front"){
                                if(queue.empty() == 1){
                                        cout << "error" << endl;
                                }else{
                                        cout << queue.front() << endl;
                                }
                        }else if(cmd == "dequeue"){
                                if(queue.empty() == 1){
                                        cout << "error" << endl;
                                }else{
                                        cout << queue.dequeue() << endl;
                                }
                        }else if(cmd == "empty"){
                                cout << queue.empty() << endl;
                        }else if(cmd == "size"){
                                cout << queue.size() << endl;
                        }else{
                                cout << "error" << endl;
                        }
                        cin >> cmd;
                }
        }else if(type == "double"){
                myqueue<double> queue;
                string cmd;
                cin >> cmd;
                while(!cin.eof()){
                        if(cmd == "enqueue"){
                                double x;
                                cin >> x;
                                queue.enqueue(x);
                        }else if(cmd == "front"){
                                if(queue.empty() == 1){
                                        cout << "error" << endl;
                                }else{
                                        cout << queue.front() << endl;
                                }
                        }else if(cmd == "dequeue"){
                                if(queue.empty() == 1){
                                        cout << "error" << endl;
                                }else{
                                        cout << queue.dequeue() << endl;
                                }
                        }else if(cmd == "empty"){
                                cout << queue.empty() << endl;
                        }else if(cmd == "size"){
                                cout << queue.size() << endl;
                        }else{
                                cout << "error" << endl;
                        }
                        cin >> cmd;
                }
        }else if(type == "char"){
                myqueue<char> queue;
                string cmd;
                cin >> cmd;
                while(!cin.eof()){
                        if(cmd == "enqueue"){
                                char x;
                                cin >> x;
                                queue.enqueue(x);
                        }else if(cmd == "front"){
                                if(queue.empty() == 1){
                                        cout << "error" << endl;
                                }else{
                                        cout << queue.front() << endl;
                                }
                        }else if(cmd == "dequeue"){
                                if(queue.empty() == 1){
                                        cout << "error" << endl;
                                }else{
                                        cout << queue.dequeue() << endl;
                                }
                        }else if(cmd == "empty"){
                                cout << queue.empty() << endl;
                        }else if(cmd == "size"){
                                cout << queue.size() << endl;
                        }else{
                                cout << "error" << endl;
                        }
                        cin >> cmd;
                }
        }else if(type == "string"){
                myqueue<string> queue;
                string cmd;
                cin >> cmd;
                while(!cin.eof()){
                        if(cmd == "enqueue"){
                                string x;
                                cin >> x;
                                queue.enqueue(x);
                        }else if(cmd == "front"){
                                if(queue.empty() == 1){
                                        cout << "error" << endl;
                                }else{
                                        cout << queue.front() << endl;
                                }
                        }else if(cmd == "dequeue"){
                                if(queue.empty() == 1){
                                        cout << "error" << endl;
                                }else{
                                        cout << queue.dequeue() << endl;
                                }
                        }else if(cmd == "empty"){
                                cout << queue.empty() << endl;
                        }else if(cmd == "size"){
                                cout << queue.size() << endl;
                        }else{
                                cout << "error" << endl;
                        }
                        cin >> cmd;
                }
        }
        cout << "\n";
        return 1;
}
