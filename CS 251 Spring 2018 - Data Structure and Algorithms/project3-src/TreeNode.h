#ifndef TREENODE_H
#define TREENODE_H

#include <string>

using namespace std;

class TreeNode
{
	private:
		TreeNode * left;
		TreeNode * right;
		unsigned int frequency;
		unsigned char val;
	public:
		
		//TreeNode();
		TreeNode(unsigned char val, unsigned int frequency);

		//returns true if this node is a leaf node, returns false otherwise
		bool isLeafNode();

		//puts left as the left child and right as the right child of this node
		void join(TreeNode * left, TreeNode * right);
		
		TreeNode * getLeft(); //returns the left child
			
		TreeNode * getRight(); //returns the right child

		unsigned int getFrequency(); //returns frequency member variable
	
		unsigned char getVal(); //returns val member variable
		
		void setFrequency(unsigned int frequency);
		
		void setVal(unsigned char val);
		
		~TreeNode();
};


#endif
