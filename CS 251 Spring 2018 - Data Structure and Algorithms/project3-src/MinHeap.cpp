#include <iostream>
#include <climits>
#include <algorithm>
#include "MinHeap.h"



//implement the methods in MinHeap.h

MinHeap::MinHeap()
{
	this->size = 0;
}

void MinHeap::insert(TreeNode * val)
{
	//vector <TreeNode *> node(tn);
	//cout << "insert val: " << val->getVal() << endl;
	//cout << "insert fre: " << val->getFrequency() << endl;
	//cout << "size: " << node.size() << endl;
	node.push_back(val);			//added to vector
	
	int child = node.size()-1;
	int parent = iparent(child);
	while(child > 0){
		//swap(child, parent);
		if(node[child]->getFrequency() >= node[parent]->getFrequency()){
			break;
		}
		TreeNode * tmp = node[child];
		node[child] = node[parent];
		node[parent] = tmp;
		child = parent;
		parent = iparent(child);
	}
}

int MinHeap::getSize()
{
	return node.size();
}

TreeNode * MinHeap::removeMin()
{
	if(node.size() == 0){
		return NULL;
	}
	int n = node.size();
	TreeNode * minkey = node[0];
	n--;
	node[0] = node[n];
	node.pop_back();
	if(n == 0){
		return minkey;
	}

	int parent = 0;
	int left = ileft(parent);
	int right = iright(parent);
	while(left < n){
		int min = left;
		if(right < n && node[right]->getFrequency() < node[left]->getFrequency()){
			min = right;
		}
		//swap(parent, min);
		if(node[parent]->getFrequency() <= node[min]->getFrequency()){
			break;
		}
		TreeNode * tmp = node[min];
		node[min] = node[parent];
		node[parent] = tmp;
		parent = min;
		left = ileft(parent);
		right = iright(parent);
	}
	return minkey;
}

MinHeap::~MinHeap(){
	node.clear();
	size = 0;
}
