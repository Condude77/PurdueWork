#ifndef PROJ4_RBTREE_H
#define PROJ4_RBTREE_H

#include "Node.h"
#include <iostream>
#include <queue>
using namespace std;

template <typename Key, typename Element>
class RBTree {
private:
	// Add class members here
	Node<Key, Element> * root;
	size_t s;
public:
	// Implement each of these methods
    RBTree() {root = NULL; s = 0; }
    void insert(const Key& k, const Element& e);
    Node<Key, Element>* search(const Key& k);
    void del(const Key& k);
    void inorder(ostream& out) const;
    void levelOrder(ostream& out) const;
	size_t size() const;
	Node<Key, Element>* getRoot();
    void rotateLeft(Node<Key, Element> * x);
    void rotateRight(Node<Key, Element> * x);
    void replace(Node<Key, Element> * dest, Node<Key, Element> * src);
    void inorderRecursive(ostream& out, Node<Key, Element> * node) const;
    void levelOrderRecursive(ostream& out, Node<Key, Element> * node) const;
};


// Function to insert a new node with given data
template <typename Key, typename Element>
void RBTree<Key, Element>::insert(const Key& k, const Element& e) {
	Node<Key, Element> * node;
	Node<Key, Element> * p;
	Node<Key, Element> * tmp;
	p = NULL;
	node = root;
	while(node){
		p = node;
		if(k < node->getKey()){
			node = node->getLeft();
		}else{
			node = node->getRight();
		}
	}
	if(!p){
		tmp = root = new Node<Key, Element>;
		tmp->setKey(k);
		tmp->setElement(e);
		tmp->setColor(BLACK);
		tmp->setParent(NULL);
		tmp->setLeft(NULL);
		tmp->setRight(NULL);
	}else{
		tmp = new Node<Key, Element>;
		tmp->setKey(k);
		tmp->setElement(e);
		tmp->setColor(RED);
		tmp->setParent(p);
		tmp->setLeft(NULL);
		tmp->setRight(NULL);
		
		if(tmp->getKey() < p->getKey()){
			p->setLeft(tmp);
		}else{
			p->setRight(tmp);
		}
	}
	
	Node<Key, Element> * uncle;
	bool side;
	while(tmp->getParent() && tmp->getParent()->getColor() == RED){
		if((side = (tmp->getParent() == tmp->getParent()->getParent()->getLeft()))){
			uncle = tmp->getParent()->getParent()->getRight();
		}else{
			uncle = tmp->getParent()->getParent()->getLeft();
		}
		if(uncle && uncle->getColor() == RED){
			tmp->getParent()->setColor(BLACK);
			uncle->setColor(BLACK);
			tmp->getParent()->getParent()->setColor(RED);
			tmp = tmp->getParent()->getParent();
		}else{
			if(tmp == (side ? tmp->getParent()->getRight() : tmp->getParent()->getLeft())){
				tmp = tmp->getParent();
				side ? rotateLeft(tmp) : rotateRight(tmp);
			}
			tmp->getParent()->setColor(BLACK);
			tmp->getParent()->getParent()->setColor(RED);
			side ? rotateRight(tmp->getParent()->getParent()) : rotateLeft(tmp->getParent()->getParent());
		}
	}
	root->setColor(BLACK);
	s++;
}

// Function to search a node with given data
template <typename Key, typename Element>
Node<Key, Element>* RBTree<Key, Element>::search(const Key& k) {
	Node<Key, Element> * node = root;
	while(node){
		if(node->getKey() > k){
			node = node->getLeft();
		}else if(node->getKey() < k){
			node = node->getRight();
		}else{
			return node;
		}
	}
}

// Function to delete a node with given data
template <typename Key, typename Element>
void RBTree<Key, Element>::del(const Key& k) {
	Node<Key, Element> * node = root;
	while(node){
		if(node->getKey() > k){
			node = node->getLeft();
		}else if(node->getKey() < k){
			node = node->getRight();
		}else{
			break;
		}
	}
	if(node->getKey() != k || !node){
		return;
	}
	color og;
	Node<Key, Element> * child;
	Node<Key, Element> * old;
	
	//check if there is a child and then replace child with node
	if(!node->getLeft()){
		child = node->getRight();
		replace(node, child);
	}else if(!node->getRight()){
		child = node->getLeft();
		replace(node, child);
	}else{
		old = node->getRight();
		//get min value in tree
		while(old->getLeft()){
			old = old->getLeft();
		}
		og = old->getColor();
		child = old->getRight();
		if(old->getParent() == node){
			child->setParent(node);
		}else{
			replace(old, old->getRight());
			old->setRight(node->getRight());
			old->getRight()->setParent(old);
		}
		replace(node, old);
		old->setLeft(node->getLeft());
		old->getLeft()->setParent(old);
		old->setColor(node->getColor());
	}
	// special delete cases
	if(og == BLACK){
		bool side;
		Node<Key, Element> * sib;
		while(old->getColor() == BLACK && old != root){
			if((side = (old == old->getParent()->getLeft()))){
				sib = old->getParent()->getRight();
			}else{
				sib = old->getParent()->getLeft();
			}
			if(sib->getColor() == RED){
				sib->setColor(BLACK);
				old->getParent()->setColor(RED);
				side ? rotateLeft(old->getParent()) : rotateRight(old->getParent());
				sib = side ? old->getParent()->getRight() : old->getParent()->getLeft();
			}
			if(sib->getRight()->getColor() == BLACK && sib->getLeft()->getColor() == BLACK){
				sib->setColor(RED);
				old = old->getParent();
			}else{
				if(sib->getRight()->getColor() == BLACK || sib->getLeft()->getColor() == BLACK){
					sib->setColor(RED);
					if(side){
						sib->getLeft()->setColor(BLACK);
						rotateRight(sib);
						sib = old->getParent()->getRight();
					}else{
						sib->getRight()->setColor(BLACK);
						rotateLeft(sib);
						sib = old->getParent()->getLeft();
					}
				}
				sib->setColor(old->getParent()->getColor());
				old->getParent()->setColor(BLACK);
				if(side){
					sib->getLeft()->setColor(BLACK);
					rotateLeft(old->getParent());
				}else{
					sib->getRight()->setColor(BLACK);
					rotateRight(old->getParent());
				}
				old = root;
			}
		}
	}
	s--;
}

// Function to do inorder traversal
template <typename Key, typename Element>
void RBTree<Key, Element>::inorder(ostream& out) const {inorderRecursive(out, root);}

// Function to perform level-order traversal
template <typename Key, typename Element>
void RBTree<Key, Element>::levelOrder(ostream& out) const {levelOrderRecursive(out, root);}

// Function to return the number of nodes in the RBTree
template <typename Key, typename Element>
size_t RBTree<Key, Element>::size() const { return s; }

// Function to return a pointer to the root of the RBTree
template <typename Key, typename Element>
Node<Key, Element>* RBTree<Key, Element>::getRoot() { return root; }


// The following functions have been provided for you, and do not
// need to be modified:


// Output stream operator overload -- writes the contents of the
// entire RBTree to the specified output stream, using an in-order
// traversal. Note that there must be a similar operator overload
// for Key and Element types in order for this to compile.
// Example usage:
//		RBTree<K,E> rbt;
//		cout << rbt << endl;
template <typename Key, typename Element>
ostream& operator<<(ostream& out, const RBTree<Key, Element>& rbtree) {
	out << rbtree.size() << endl;
	rbtree.inorder(out);
	return out;
}

// Input stream operator overload -- reads the contents of an RBTree
// from the specified input stream, inserting each Node as it is
// read. Note that there must be a similar operator overload for Key
// and Element types in order for this to compile.
// Example usage:
//		RBTree<K,E> rbt;
//		cin >> rbt;
template <typename Key, typename Element>
istream& operator>>(istream& in, RBTree<Key, Element>& rbtree) {
	// Clear existing contents, if any
	rbtree = RBTree<Key, Element>();

	// Read in the number of elements to read
	size_t size;
	in >> size;
	for (size_t i = 0; i < size; i++) {
		// Read in each node as it appears in the stream
		Node<Key, Element> n;
		in >> n;
		// Insert the node into the RBTree
		rbtree.insert(n.getKey(), n.getElement());
	}

	return in;
}

// Rotate tree node left
template <typename Key, typename Element>
void RBTree<Key, Element>::rotateLeft(Node<Key, Element> * x){
	Node<Key, Element> * node;
	node = x->getRight();
	x->setRight(node->getLeft());
	if(node->getLeft()){
		node->getLeft()->setParent(x);
	}
	node->setParent(x->getParent());
	node->setLeft(x);
	if(!x->getParent()){
		root = node;
	}else if(x == x->getParent()->getLeft()){
		x->getParent()->setLeft(node);
	}else{
		x->getParent()->setRight(node);
	}
	x->setParent(node);
}
// Rotate tree node right
template <typename Key, typename Element>
void RBTree<Key, Element>::rotateRight(Node<Key, Element> * x){
	Node<Key, Element> * node;
	node = x->getLeft();
	x->setLeft(node->getRight());
	if(node->getRight()){
		node->getRight()->setParent(x);
	}
	node->setParent(x->getParent());
	node->setRight(x);
	if(!x->getParent()){
		root = node;
	}else if(x == x->getParent()->getRight()){
		x->getParent()->setRight(node);
	}else{
		x->getParent()->setLeft(node);
	}
	x->setParent(node);
}
//replaces the dest node with the src node
template <typename Key, typename Element>
void RBTree<Key, Element>::replace(Node<Key, Element> * dest, Node<Key, Element> * src){
	if(dest->getParent() == NULL){
		root = src;
	}else if(dest == dest->getParent()->getLeft()){
		dest->getParent()->setLeft(src);
	}else{
		dest->getParent()->setRight(src);
	}
	if(src){
		src->setParent(dest->getParent());
	}
}
//recursive helper for inorder method
template <typename Key, typename Element>
void RBTree<Key, Element>::inorderRecursive(ostream& out, Node<Key, Element> * node) const {
	if(node == NULL){
		return;
	}
	inorderRecursive(out, node->getLeft());
	out << node->getKey() << endl;
	out << node->getElement() << endl;
	out << endl;
	inorderRecursive(out, node->getRight());
}
//recursve helper for levelOrder method
template <typename Key, typename Element>
void RBTree<Key, Element>::levelOrderRecursive(ostream& out, Node<Key, Element> * node) const {
	if(node == NULL){
		return;
	}
	queue<Node<Key, Element> *> q;
	q.push(node);
	while(!q.empty()){
		Node<Key, Element> * tmp = q.front();
		out << tmp->getKey() << endl;
		out << tmp->getElement() << endl;
		out << endl;
		q.pop();
		if(tmp->getLeft() != NULL){
			q.push(tmp->getLeft());
		}
		if(tmp->getRight() != NULL){
			q.push(tmp->getRight());
		}
	}
}
#endif //PROJ4_RBTREE_H
