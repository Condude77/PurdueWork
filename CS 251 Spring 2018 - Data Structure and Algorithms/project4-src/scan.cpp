#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include "SimpleHTMLParser.h"
#include "RBTree.h"
using namespace std;

// Output stream operator overload for node elements
ostream& operator<<(ostream& out, const vector<pair<string, int>>& e);

int main(int argc, char** argv) {
	// Implement your scan code here
	if(argc != 3){
		return 0;
	}
	ifstream in(argv[1]);
	ofstream out(argv[2]);
	int no;
	in >> no; 
	if(no <= 0){
		return 0;
	}
	SimpleHTMLParser * htmlp = new SimpleHTMLParser();
	string url;
	RBTree<string, vector<pair<string, int>>> tree;
	while(in >> url){
		//parse
		//string url = urlList.back();
		//cout << url;
		htmlp->parse(url);
		//get doc
		string txt = htmlp->getText();
		//process doc
		istringstream iss(txt);
		string word;
		while(iss >> word){
			//search word in RBTree
			if(!tree.search(word)){
				vector <pair<string, int>> tmp;
				tmp.push_back(make_pair(word, 1));
				tree.insert(url, tmp);
			}else{
				vector <pair<string, int>>& tmp = tree.search(word)->getElement();
				if(tree.search(word)->getKey() == url){
					tmp.back().second++;
					cout << "check1: " << tmp.back().second << endl;
				}else{
					vector<pair<string, int>> t;
					t.push_back(make_pair(url, 1));
					tree.insert(word, t);
				}
			}
		}
	}
	//print RBTree to output file
	out << tree;
	//out << endl;
}


// This function overloads the output stream operator for Node
// elements. It has been provided to you and does not need to be
// altered.
ostream& operator<<(ostream& out, const vector<pair<string, int>>& e) {
	// Write the number of elements in the vector
	out << e.size() << endl;
	for (auto& uf : e)
		// Write each member of each pair the vector stores
		out << uf.first << " " << uf.second << endl;
	return out;
}
