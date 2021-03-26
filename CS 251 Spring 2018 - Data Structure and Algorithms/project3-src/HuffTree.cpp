#include <iostream>
#include "HuffTree.h"

HuffTree::HuffTree(){
	root = NULL;
}

void HuffTree::buildTree(MinHeap * mh){
	while(mh->getSize() > 1){
        	TreeNode * left = mh->removeMin();
                TreeNode * right = mh->removeMin();
                unsigned int tf = left->getFrequency() + right->getFrequency();
                TreeNode * parent = new TreeNode(' ', tf);
                parent->join(left, right);
                mh->insert(parent);
        }
        root = mh->removeMin();
}

void HuffTree::recursiveCodes(TreeNode * tn, string code){
	if(tn == NULL){
		return;
	}
	if(tn->isLeafNode()){
		codes[tn->getVal()] = code;
		//cout << (unsigned int) tn->getVal() << " " << (unsigned int) tn->getFrequency() << " " << code << endl;
	}
	recursiveCodes(tn->getLeft(), code + "1");
	recursiveCodes(tn->getRight(), code + "0");
}

void HuffTree::generateCodes(){
	recursiveCodes(root, "");
}

TreeNode * HuffTree:: getRoot(){
	return root;
}

string HuffTree::getCharCode(int c){
	return codes[c];
}

HuffTree::~HuffTree(){
	delete root;
}
