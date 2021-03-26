#ifndef EXPR_EVAL_H
#define EXPR_EVAL_H
#include<iostream>
#include<string>
#include<sstream>
#include<stdlib.h>
#include<math.h>
#include "mystack.h"

using namespace std;

class expr_eval{
  private:
        // Declare which ever necessary variables and methods
	//mystack<double> values;
	//mystack<string> expression;
  public:
        double eval(string exp, double x){
		double total;
		stringstream ss(exp);
		char sign;
		ss >> sign;
		if(sign == '+'){
			string op1str, op2str; 
			double op1, op2; 
			ss >> op1str; 
			ss >> op2str;
			if(op1str == "x"){
				op1 = x;
			}else{
				op1 = atof(op1str.c_str());
			}
			if(op2str == "x"){
				op2 = x;	
			}else{
				op2 = atof(op2str.c_str());
			}
			total = op2 + op1;
		}else if(sign == '-'){
			string op1str, op2str;
                        double op1, op2;
                        ss >> op1str;
                        ss >> op2str;
                        if(op1str == "x"){
                                op1 = x;
                        }else{
                                op1 = atof(op1str.c_str());
                        }
                        if(op2str == "x"){
                                op2 = x;
                        }else{
                                op2 = atof(op2str.c_str());
                        }
                        total = op2 - op1;
		}else if(sign == '*'){
			string op1str, op2str;
                        double op1, op2;
                        ss >> op1str;
                        ss >> op2str;
                        if(op1str == "x"){
                                op1 = x;
                        }else{
                                op1 = atof(op1str.c_str());
                        }
                        if(op2str == "x"){
                                op2 = x;
                        }else{
                                op2 = atof(op2str.c_str());
                        }
                        total = op2 * op1;
		}else if(sign == '/'){
			string op1str, op2str;
                        double op1, op2;
                        ss >> op1str;
                        ss >> op2str;
                        if(op1str == "x"){
                                op1 = x;
                        }else{
                                op1 = atof(op1str.c_str());
                        }
                        if(op2str == "x"){
                                op2 = x;
                        }else{
                                op2 = atof(op2str.c_str());
                        }
                        total = op2 / op1;
		}else if(sign == '^'){
			string op1str, op2str;
                        double op1, op2;
                        ss >> op1str;
                        ss >> op2str;
                        if(op1str == "x"){
                                op1 = x;
                        }else{
                                op1 = atof(op1str.c_str());
                        }
                        if(op2str == "x"){
                                op2 = x;
                        }else{
                                op2 = atof(op2str.c_str());
                        }
                        total = pow(op2, op1);
		}else if(sign == 's'){
			string opstr;
			double op;
			ss >> opstr;
			if(opstr == "x"){
				op = x;
			}else{
				op = atof(opstr.c_str());
			}
			total = sin(op);
		}else if(sign == 'c'){
			string opstr;
                        double op;
                        ss >> opstr;
                        if(opstr == "x"){
                                op = x;
                        }else{
                                op = atof(opstr.c_str());
                        }
                        total = cos(op);
		}else if(sign == 't'){
			string opstr;
                        double op;
                        ss >> opstr;
                        if(opstr == "x"){
                                op = x;
                        }else{
                                op = atof(opstr.c_str());
                        }
                        total = tan(op);
		}else if(sign == 'l'){
			string opstr;
                        double op;
                        ss >> opstr;
                        if(opstr == "x"){
                                op = x;
                        }else{
                                op = atof(opstr.c_str());
                        }
                        total = log10(op);
		}else{
			//cout << "error: " << sign << endl;
			total = 0;
		}
		return total;
	}
};

#endif
