#include <iostream>
#include <string>
#include "graph.h"
 
using namespace std;
 

 // Part 1 : Find the connected components and the bridge
void part1()
{
	int numC;
	int numR;
	cin >> numC;
	cin >> numR;
	Graph * g = new Graph(numC, numR);
	int end = numR * 3;
	for(int i = 0; i < end; i++){
		string s;
		string d;
		double w;
		cin >> s;
		cin >> d;
		cin >> w;
		g->addRoute(s, d, w);
	}
	g->analyseGraph();
}

 // Part 2 :  Find a ticket using Dijkstra	
void part2()
{

}

 // Part 3 :Try to do a tour of all cities 
void part3()
{
 

}
 
 int main()
 {
	 double cmd;
	 cin >> cmd;

	 if(cmd == 1) part1();
	 else if(cmd == 2) part2();
	 else if(cmd == 3) part3();

	 return 0;
 }
