#include <iostream>
#include <string>
#include <list>
#include <map>
#include "graph.h"

#define INFI 1000000

//typedef pair<string, string> EMK;
//typedef double EMV;
//typedef map<EMK, EMV>;

/*
	Graph class for use in Project 5.
	
	This provided code does not include any specific data structure
	for storing a graph, only a list of methods that will be called 
	from main. Based on the project handout and the lecture slides,
	select a method for storing the graph and implement it here.

*/

// Constructor for Graph 
Graph::Graph(int Vertices, int routes)
{
	numCities = Vertices;
	numRoutes = routes;
	/*adj = new int* [numCities];
	for(int i = 0; i < numCities; i++){
		adj[i] = new int[numCities];
	}*/
	//adj = new map<pair<string, string>, double>;
}

void Graph::addRoute(string source, string destination, double weight){
	int r = arrayLocation(source);
	int c = arrayLocation(destination);
	//adj[r].push_back(make_pair(c, weight));
	//adj[c].push_back(make_pair(r, weight));
	adj[make_pair(source, destination)] = weight;
}

void Graph::analyseGraph(){
	int cc = 0; //connected Components
	string se; //separation edges
	cc = connectedComponents();
	cout << cc << endl;
	//separationEdges();
	//cout << se << endl;
}

void Graph::findCost(const string &source, const string &destination){

}

void Graph::eulerianTour(int e){

}

int Graph::arrayLocation(string val){
	int i = 0;
	int found = 0;
	for(i = 0; i < cities.size(); i++){
		if(cities[i] == val){
			found = 1;
			break;
		}
	}
	if(found != 1){
		cities.push_back(val);
		i++;
	}
	return i;
}

int Graph::connectedComponents(){
	int total = 0;
	map<pair<int, int>, bool> visited;
	for(int i = 0; i < numCities; i++){
		for(int j = 0; j < numCities; j++){
			visited[make_pair(i, j)] = false;
		}
	}

	for(int i = 0; i < numCities; i++){
		for(int j = 0; j < numCities; j++){
			if(visited.at(make_pair(i,j)) == false){
				DFS(i, j, visited);
				++total;
			}
		}
	}
	return total;
}

void Graph::DFS(int row, int col, map<pair<int, int>, bool> &visited){
	visited.at(make_pair(row, col)) = true;
	
	map<pair<string, string>, double>::iterator it = adj.begin();
	//for(it = adj[row].begin(); it != adj[row].end(); ++it){
	//	DFS(*it, visited);
	//}
	while(it != adj.end()){
		if(!visited.at(make_pair(arrayLocation(it->first.first), arrayLocation(it->first.second)))){
			DFS(arrayLocation(it->first.first), arrayLocation(it->first.second), visited);
		}
		++it;
	}
}

void Graph::separationEdges(){
	/*bool *visited = new bool[numCities];
	int *time = new int[numCities];
	int *lowest = new int[numCities];
	int *parent = new int[numCities];

	for(int i = 0; i < numCities; i++){
		parent[i] = -1;
		visited[i] = false;
	}
	for(int i = 0; i < numCities; i++){
		if(visited[i] == false){
			separationEdgesRecur(i, visited, time, lowest, parent);
		}
	}*/
}
/*
void Graph::separationEdgesRecur(int next, bool visited[], int time[], int lowest[], int parent[]){
	static int t = 0;
	visited[next] = true;
	time[next] = lowest[next] = ++t;

	list<int>::iterator it;
	for(it = adj[next].begin(); it != adj[next].end(); ++it){
		int cur = *it;
		if(visited[cur] == false){
			parent[cur] = cur;
			separationEdgesRecur(cur, visited, time, lowest, parent);
			lowest[next] = min(lowest[next], lowest[cur]);
			if(lowest[cur] > time[next]){
				cout << cities[next] << " " << cities[cur] << endl;
			}
		}else if(cur != parent[next]){
			lowest[next] = min(lowest[next], time[cur]);
		}
	}
}*/
