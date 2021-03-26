#ifndef NODE_H
#define NODE_H

#include <iostream>
using namespace std;

enum color {RED, BLACK};

template <typename Key, typename Element>
class Node {
private:
	// Add class members here
	color col;
	Key key;
	Element element;
	Node<Key, Element> * parent;
	Node<Key, Element> * left;
	Node<Key, Element> * right;
public:
	// Implement each of these methods
	Node() {
		parent = NULL;
		left = NULL;
		right = NULL;
		key;
		element;
		col = RED;
	}

	// These implementations are only to get the skeleton
	// code to compile. Ignore the warnings they generate
	// until you replace their implementations with your own
	const Key& getKey() const { return key; }
	const Element& getElement() const { return element; }
	Element& getElement() { return element; }

	color getColor() const { return col; }
	Node* getParent() { return parent; }
	Node* getLeft() { return left; }
	Node* getRight() { return right; }

	void setKey(const Key& k) {
		key = k;
	}
	void setElement(const Element& e) {
		element = e;
	}
	void setColor(color c) {
		col = c;
	}
	void setLeft(Node * x){
		left = x;
	}
	void setRight(Node * x){
		right = x;
	}
	void setParent(Node * x){
		parent = x;
	}
};



// The following functions have been provided for you, and do not
// need to be modified:


// Output stream operator overload -- writes the contents of a
// Node to the specified output stream. Note that there must be
// a similar operator overload for Key and Element types in
// order for this to compile.
// Example usage:
//		Node<K,E> n;
//		cout << n << endl;
template <typename Key, typename Element>
ostream& operator<<(ostream& out, const Node<Key, Element>& n) {
	out << n.getKey() << endl;
	out << n.getElement() << endl;
	return out;
}

// Input stream operator overload -- reads the contents of a
// Node from the specified input stream. Note that there must be
// a similar operator overload for Key and Element types in order
// for this to compile.
// Example usage:
//		Node<K,E> n;
//		cin >> n;
template <typename Key, typename Element>
istream& operator>>(istream& in, Node<Key, Element>& n) {
	// Clear existing contents, if any
	n = Node<Key, Element>();

	Key k;
	in >> k;
	Element e;
	in >> e;
	n.setKey(k);
	n.setElement(e);

	return in;
}

#endif
