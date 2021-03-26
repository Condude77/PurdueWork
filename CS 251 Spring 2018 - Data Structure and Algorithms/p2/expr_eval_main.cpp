#include <iostream>
#include <string>
#include <sstream>
#include <stdlib.h>
#include "expr_eval.h"
#include "mystack.h"

using namespace std;

int main(){
	expr_eval ee;
	double output;
	int flag = 0;
	while(!cin.eof()){
		mystack<double> values;
		mystack<char> ops;
		string exp;
		string n = "";
		double num = 0;
		double xval = 0;
		getline(cin, exp);
		int length = exp.length();
		if(exp.find("x") != string::npos){
			string str;
			str = exp.substr(exp.find(" "), exp.length());
			xval = atof(str.c_str());
			length = length - str.length();
		}
		for(int i = 0; i < length; i++){
			//if(exp.at(i) != '+' && exp.at(i) != '-' && exp.at(i) != '*' && exp.at(i) != '/' && exp.at(i) != '^' && exp.at(i) != 's' && exp.at(i) != 'c' && exp.at(i) != 'l' && exp.at(i) != 't' && exp.at(i) != '(' && exp.at(i) != ')' && exp.at(i) != 'o' && exp.at(i) != 'g' && exp.at(i) != 'a' && exp.at(i) != 'n'){
			if(isdigit(exp.at(i)) || exp.at(i) == '.'){
				n = n + exp.at(i);
			}else if(exp.at(i) == 'x'){
				if(n != "") { 
                                        num = atof(n.c_str());
                                        values.push(num);
                                        n = "";
                                }
				values.push('x');
			}else if(exp.at(i) != ')'){
				if(n != "") { 
					num = atof(n.c_str());
					values.push(num);
					n = "";
				}
				if(ops.empty() == 1){
					if(exp.at(i) == 's' || exp.at(i) == 'c' || exp.at(i) == 't' || exp.at(i) == 'l'){
                                                ops.push(exp.at(i));
						i = i+2;
                                        }else{
						ops.push(exp.at(i));
					}
				}else if(exp.at(i) == '('){
					ops.push('(');
				}//else if(((exp.at(i) == '+' || exp.at(i) == '-') && (ops.top() == '+' || ops.top() == '-' || ops.top() == '(')) || ((exp.at(i) == '*' || exp.at(i) == '/') && (ops.top() != 's' || ops.top() != 'c' || ops.top() != 't' || ops.top() != 'l' || ops.top() != '^')) || ((exp.at(i) == 's' || exp.at(i) == 'c' || exp.at(i) == 't' || exp.at(i) == 'l') && (ops.top() != '^'))){
				else if(((exp.at(i) == '+' && ops.top() != '-') && (ops.top() == '+' || ops.top() == '(')) || ((exp.at(i) == '*' && ops.top() != '/') && (ops.top() != 's' && ops.top() != 'c' && ops.top() != 't' && ops.top() != 'l' && ops.top() != '^')) || ((exp.at(i) == 's') && (ops.top() != '^')) || ((exp.at(i) == '-' && ops.top() != '+') && (ops.top() == '-' || ops.top() == '(')) || ((exp.at(i) == '/' && ops.top() != '*') && (ops.top() != 's' && ops.top() != 'c' && ops.top() != 't' && ops.top() != 'l' && ops.top() != '^')) || ((exp.at(i) == 'c') && (ops.top() != '^')) || ((exp.at(i) == 't') && (ops.top() != '^')) || ((exp.at(i) == 'l') && (ops.top() != '^'))){	
					//cout << "op check1:  " << ops.top() << endl;        //error checking
					ops.push(exp.at(i));
					//cout << "op check2: " << ops.top() << endl;        //error checking
				}else{
					if(exp.at(i) == 's' || exp.at(i) == 'c' || exp.at(i) == 't' || exp.at(i) == 'l'){
						i = i+2;
					}
					if(ops.top() != 's' && ops.top() != 'c' && ops.top() != 't' && ops.top() != 'l'){
						string str, v1, v2;
						stringstream ss, val1, val2;
						val1 << values.pop();
						val2 << values.pop();
						v1 = val1.str();
						v2 = val2.str();
						ss << ops.pop() << " " << v1 << " " << v2;
						getline(ss,str);
						//cout << "str1: " << str << endl;	//error checking
						double val;
						val = ee.eval(str, xval);
						values.push(val);
					}else{
						string str, v;
                                                stringstream ss, val1;
                                                val1 << values.pop();
                                                v = val1.str();
                                                str = ops.pop() + " " + v;
						//cout << "str2: " << str << endl;        //error checking
                                                double val;
                                                val = ee.eval(str, xval);
                                                values.push(val);
					}
					ops.push(exp.at(i));		
				}
			}else if(exp.at(i) == ')'){
				if(n != "") {
                                        num = atof(n.c_str());
                                        values.push(num);
                                        n = "";
                                }
				if(ops.top() == '('){
					ops.pop();
					string str, v;
                                        stringstream ss, val1;
                                        val1 << values.pop();
                                        v = val1.str(); 
                                        str = ops.pop() + " " + v; 
                                        //cout << "str7: " << str << endl;        //error checking
                                        double val;
                                        val = ee.eval(str, xval);
                                        values.push(val);
				}else{
					while(ops.top() != '('){
						if(ops.top() != 's' && ops.top() != 'c' && ops.top() != 't' && ops.top() != 'l'){
                                        	        string str, v1, v2;
                                        	        stringstream ss, val1, val2;
                                        	        val1 << values.pop();
                                        	        val2 << values.pop();
                                        	        v1 = val1.str();
                                        	        v2 = val2.str();
                                        	        ss << ops.pop() << " " << v1 << " " << v2;
                                        	        getline(ss,str);
                                        	        //cout << "str3: " << str << endl;        //error checking
                                        	        double val;
                                        	        val = ee.eval(str, xval);
                                        	        values.push(val);
                                        	}else{
                                        	        string str, v;
                                        	        stringstream ss, val1;
                                        	        val1 << values.pop();
                                        	        v = val1.str(); 
                                        	        str = ops.pop() + " " + v; 
                                        	        //cout << "str4: " << str << endl;        //error checking
                                        	        double val;
                                        	        val = ee.eval(str, xval);
                                        	        values.push(val);
                                        	}
						if(flag < 10){
							//cout << "loop check1" << endl;
							//cout << "ops top is: " << ops.top() << endl;
							flag++;
						}
						if(flag == 9){
							ops.pop();
							return 0;
						}
                                	}
					ops.pop();
				}
			}
		}
		if(n != ""){
			num = atof(n.c_str());
			values.push(num);
		}
		flag = 0;
		while(ops.empty() == 0){
			if(ops.top() != 's' && ops.top() != 'c' && ops.top() != 't' && ops.top() != 'l'){
				string str, v1, v2;
                                stringstream ss, val1, val2;
                                val1 << values.pop();
                                val2 << values.pop();
                                v1 = val1.str();
                                v2 = val2.str();
                                ss << ops.pop() << " " << v1 << " " << v2;
                                getline(ss,str);
                                //cout << "str5: " << str << endl;        //error checking
                                double val;
                                val = ee.eval(str, xval);
                                values.push(val);
                        }else{
                                string str, v;
                                stringstream ss, val1;
                                val1 << values.pop();
                                v = val1.str();
                                str = ops.pop() + " " + v;
                                //cout << "str6: " << str << endl;        //error checking
                                double val;
                                val = ee.eval(str, xval);
                                values.push(val);
			}
			if(flag < 10){
                                //cout << "loop check2" << endl;
                                flag++;
		
                        }
			if(flag == 9){
				ops.pop();
				return 0;
			}
			
		}
		output = values.pop();
		cout.precision(2);
		cout << output << endl;
	}
	return 1;
}
