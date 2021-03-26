#ifndef MINHEAP_H
#define MINHEAP_H

#include "TreeNode.h"
#include <vector>

using namespace std;

class MinHeap
{
	private:
		int size;
		vector<TreeNode *> node;
	public:
		MinHeap();
		TreeNode * removeMin(); //returns root of heap
		void insert(TreeNode * val); //adds element to heap
		int getSize(); //returns size of heap
		~MinHeap();
		int iparent(int i){
			return (i-1)/2;
		}
		int ileft(int i){
			return i*2+1;
		}
		int iright(int i){
			return i*2+2;
		}
		/*void swap(int child, int parent){
			while(child > 0){
				if(tn[child]->getFrequency() >= tn[parent]->getFrequency()){
					break;
				}
				TreeNode * tmp = tn[child];
				tn[child] = tn[parent];
				tn[parent] = tmp;
				//child = parent;
				//parent = iparent(child);
			}
		}*/
};

#endif
