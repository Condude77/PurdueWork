#include <iostream>
#include "TreeNode.h"

#include <string>


TreeNode::TreeNode(unsigned char v, unsigned int freq)
{
	this->val = v;
	this->frequency = freq;
	left = NULL;
	right = NULL;
}

unsigned int TreeNode::getFrequency()
{
	return frequency;
}
unsigned char TreeNode::getVal()
{
	return val;
}

bool TreeNode::isLeafNode()
{
	if(this->left == NULL && this->right == NULL){
		return true;
	}else{
		return false;
	}
}

void TreeNode::join(TreeNode * left, TreeNode * right)
{
	this->right = right;
	this->left = left;
}

TreeNode * TreeNode::getLeft()
{
	return this->left;
}

TreeNode * TreeNode::getRight()
{
	return this->right;
}

void TreeNode::setFrequency(unsigned int freq)
{
	this->frequency = freq;
}

void TreeNode::setVal(unsigned char v)
{
	this->val = v;
}

TreeNode::~TreeNode(){
	delete left;
	delete right;
}
