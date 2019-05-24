#ifndef __BSTMAP_H__
#define __BSTMAP_H__

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include "map.h"

template<typename K, typename V>
class BstMap : public Map<K , V> {
	private:
		class Node {
			public:
				K key;
				V value;
				Node* left;
				Node* right;
				Node(const K &k, const V & v) : key(k), value(v), left(NULL), right(NULL) {}
				Node(const K &k, const V & v, Node* p, Node* n) : key(k), value(v), left(p), right(n) {}
		};
		Node* root;
	public:
		BstMap() : root(NULL) {}
		
		// no description in README to ADD A COPY CONSTRUCTOR!!! F**k
		Node* copy(const Node* node) {
			if (node == NULL) {
				return NULL;
			}
			Node* newRoot = new Node(node -> key, node -> value);
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
		BstMap(const BstMap<K,V> &rhs) : root(NULL) {
			root = copy(rhs.root);
		}
		
		// assignment operator
		BstMap<K,V> & operator=(const BstMap<K,V> & rhs) {
			if (this != &rhs) {
				destroy(root);
				root = copy(rhs.root);
			}
			return *this;
		}
		
		
		virtual void add(const K & key, const V & value) {
			Node** adder = &root;
			Node* element = new Node(key, value, NULL, NULL);
			while (*adder != NULL) {
				if (key < (*adder) -> key) {
					adder = &(*adder) -> left;
				}
				else if (key > (*adder) -> key) {
					adder = &(*adder) -> right;
				}
				// If you add a key which already exists, you should replace its value. and also return
				else {
					(*adder) -> value = value;
					delete element;
					return;
				}
			}
			// finally, add the new node into the tree
			*adder = element;
		}
		
		virtual const V & lookup(const K& key) const throw (std::invalid_argument) {
			Node* lookuper = root;
			while (lookuper != NULL) {
				if (key < lookuper -> key) {
					lookuper = lookuper -> left;
				}
				else if (key > lookuper -> key) {
					lookuper = lookuper -> right;
				}
				else {
					return lookuper -> value;
				}
			}
			throw std::invalid_argument("The key is not found.");
		}
		
		// we need to get access of the tree's child, so construct a helper function
		// MAYBE we can learn later how to use iteration to delete a node??? 
		Node* remove_helper(Node* node, const K& key) {
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
					node -> value = temp_r -> value;
					node -> key = temp_r -> key;
					node -> right = remove_helper(node -> right, temp_r -> key);
				}
			}
			return node;
		}
		
		virtual void remove(const K& key) {
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
		
		virtual ~BstMap<K,V>() {
			destroy(root);
		}
};

#endif