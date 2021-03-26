#ifndef HUFFTREE_H
#define HUFFTREE_H

#include "MinHeap.h"
#include "TreeNode.h"
#include <string>

#define MAXCODELENGTH 256
#define BYTESIZE 8

using namespace std;

class HuffTree
{
	private:
		//Add class members and methods	
		MinHeap * minHeap;
		TreeNode * root;
		string codes[MAXCODELENGTH];
	public:
		HuffTree();

		//build a hiffman  tree  give a minheap
		void buildTree(MinHeap * mh);

		//recursive code
		//traverses through whole tree and generates codes
		void recursiveCodes(TreeNode * tn, string code);

		//generate codes by traversing the huffman tree
		//and store them in an internal data structure (array of strings for example)
		void generateCodes();

		//returns root of the tree
		TreeNode * getRoot();

		//returns huffman code from  the ascii code
		string getCharCode(int c);

		~HuffTree();
		
};


#endif
