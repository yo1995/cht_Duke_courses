#ifndef __BSTSET_H__
#define __BSTSET_H__
#include "set.h"
#include "cstdlib"
#include <iostream>

template<typename T>
class BstSet : public Set<T> {
	private:
		class Node {
			public:
			T key;
			Node *left;
			Node *right;
			Node(const T &k) : key(k), left(NULL), right(NULL) {}
			Node(const T &k, Node* l, Node* r) : key(k), left(l), right(r) {}
		};
		Node *root;
		
	public:
		BstSet() : root(NULL) {}
		// again, xinlei! heart tired! coeur fatigue! corazon cansado!
		Node* copy(const Node* node) {
			if (node == NULL) {
				return NULL;
			}
			Node* newRoot = new Node(node -> key);
			// recursively copy the whole tree into the new one
			if (node -> left != NULL) {
				newRoot -> left = copy(node -> left);
			}
			if (node -> right != NULL) {
				newRoot -> right = copy(node -> right);
			}
			return newRoot;
		}
		
		// copy constructor
		// either assign NULL or not does not matter, since root will be assigned anyway
		BstSet(const BstSet<T> &rhs) : root(NULL) {
			root = copy(rhs.root);
		}
		
		// assignment operator
		BstSet<T> & operator=(const BstSet<T> & rhs) {
			if (this != &rhs) {
				destroy(root);
				root = copy(rhs.root);
			}
			return *this;
		}
		
		virtual void add(const T & key) {
			Node** adder = &root;
			while (*adder != NULL) {
				if (key < (*adder) -> key) {
					adder = &(*adder) -> left;
				}
				else if (key > (*adder) -> key) {
					adder = &(*adder) -> right;
				}
				// If you add a key which already exists, you should replace its value. and also return
				else {
					return;
				}
			}
			// finally, add the new node into the tree
			*adder = new Node(key);
		}
		
   
		virtual bool contains(const T & key) const {
			Node* checker = root;
			while (checker != NULL) {
				if (key < checker -> key) {
					checker = checker -> left;
				}
				else if (key > checker->key) {
					checker = checker -> right;
				}
				else {
					return true;
				}
			}
			// for last 094 it was throw, but this time it is bool return value
			return false;
		}
		
		
		
		Node* remove_helper(Node* node, const T & key) {
			if (node == NULL) {
				return node;
			}
			else if (key < node -> key) {
				node -> left = remove_helper(node -> left, key);
			}
			else if (key > node -> key) {
				node -> right = remove_helper(node -> right, key);
			}
			else {
				if ((node -> right == NULL) && (node -> left == NULL)) {
					delete node;
					node = NULL;
				}
				else if (node -> left == NULL) {
					Node* temp = node;
					node = node -> right;
					delete temp;
				}
				else if (node -> right == NULL) {
					Node* temp = node;
					node = node -> left;
					delete temp;
				}
				else {
					// select the left-most right child (right once, then all the way left)
					Node* temp_r = node -> right;
					// use a iteration to find left most
					while (temp_r -> left != NULL) {
						temp_r = temp_r -> left;
					}
					node -> key = temp_r -> key;
					node -> right = remove_helper(node -> right, temp_r -> key);
				}
			}
			return node;
		}
		
		virtual void remove(const T & key) {
			root = remove_helper(root, key);
		}
		
		// write a destroy to help destructor
		void destroy(Node* n) {
			if (n != NULL) {
				destroy(n -> left);
				destroy(n -> right);
				delete n;
			}
		}
		
		// for personal testcases, print out the tree order
		void inorder() {
			print_mid_order(root);
		}
		
		void print_mid_order(Node* n) {
			if(n != NULL) {
				print_mid_order(n -> left);
				std::cout << n -> key << " ";
				print_mid_order(n -> right);
			}
		}
		
		virtual ~BstSet<T>() {
			destroy(root);
		}
};
#endif