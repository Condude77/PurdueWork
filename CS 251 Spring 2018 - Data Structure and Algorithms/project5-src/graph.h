#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <list>
#include <iostream>
#include <vector>
#include <map>

using namespace std;

class Graph
{
	private:
		int numCities;	//vertices
		int numRoutes;	//edges
		//string *cities;
		vector <string> cities;
		double weight;
		//ist<int> *adj;
		//list<pair<int, double>> *adj;
		map<pair<string, string>, double> adj;
		
	public:
		
		Graph(int n, int m); 
		// Create the graph
		void addRoute(string a, string b, double x);	

		//part 1: Find the connected components and the bridges
		void analyseGraph();		
		
		
		// Part 2: Find a ticket using Dijkstra	
		void findCost(const string &source, const string &destination);         
		
		// Part 3: Try to do a tour of all cities
		void eulerianTour(int e);  // NOTE : If necessary, you can change the parameters to this function.

		//Extra Functions
		//
		//gets value of strings in array
		int arrayLocation(string val);

		int connectedComponents();
	
		void DFS(int row, int col, map<pair<int, int>, bool> &visited);

		void separationEdges();
	
		void separationEdgesRecur(int next, bool visited[], int time[], int lowest[], int parent[]);

		
};

#endif
