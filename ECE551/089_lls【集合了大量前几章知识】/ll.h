#ifndef _LL_H_
#define _LL_H_
#include <cstdlib>
#include <exception>
#include <assert.h>

//YOUR CODE GOES HERE
template<typename T>
class LinkedList {
	private:
		class Node {
			public:
				T data;
				Node* prev;
				Node* next;
				// to ensure things won't go wrong even if parameters aren't correct number
				Node() : next(NULL), prev(NULL) {}
				Node(const T &d) : data(d), prev(NULL), next(NULL) {}
				Node(const T &d, Node* p) : data(d), prev(p), next(NULL) {}
				Node(const T &d, Node* p, Node* n) : data(d), prev(p), next(n) {}
		};
		Node* head;
		Node* tail;
		int size; 
	public:
		// default constructor
		LinkedList() : head(NULL), tail(NULL), size(0) {}
		
		// copy constructor, we do not want to change the original one
		LinkedList(const LinkedList &rhs) {
			size = 0;
			head = NULL;
			tail = NULL;
			// we can either iterate over size or iterate towards the tail pointer
			int i = 0;
			while (i < rhs.size) {
				addBack(rhs[i]);
				i++;
			}
		}
		// operator=
		LinkedList<T>& operator=(LinkedList<T> &rhs) {
			if (this != &rhs) {
				destroy();
				int i = 0;
				while (i < rhs.size) {
					addBack(rhs[i]);
					i++;
				}
			}
			return *this;
		}
		// add front
		void addFront(const T &item) {
			Node* front = new Node(item, NULL, head);
			if (head != NULL) {
				head -> prev = front;
			}
			head = front;
			if (tail == NULL) {
				tail = head;
			}
			size++;
		}
		// addBack
		void addBack(const T &item) {
			Node* back = new Node(item, tail, NULL);
			if (tail != NULL) {
				tail -> next = back;
			}
			tail = back;
			if (head == NULL) {
				head = tail;
			}
			size++;
		}
		
		// remove
		bool remove(const T &item) {
			Node* search = head;
			if (tail == NULL) {
				return false;
			}
			// the item could be on head, tail or between them
			while (search != NULL) {
				if (search -> data == item) {
					if (head == tail) {
						head = NULL;
						tail = NULL;
						size--;
						delete search;
						return true;
					}
					else if (search == head) {
						head = head -> next;
						search -> next -> prev = NULL;						
						size--;
						delete search;
						return true;
					}
					else if (search == tail) {
						tail = tail -> prev;
						search -> prev -> next = NULL;
						size--;
						delete search;
						return true;
					}
					else {
						search -> prev -> next = search -> next;
						search -> next -> prev = search -> prev;
						size--;
						delete search;
						return true;
					}
				}
				else {
					// continue the scan
					search = search -> next;
				}
			}
			// otherwise item not found, return false
			return false;
		}
		
		//find
		int find(const T &item) const {
			Node* search = head;
			int i = 0;
			while(search != NULL) {
				if(search -> data == item) {
					return i;
				}
				search = search -> next;
				i++;
			}
			return -1;
		}
		
		// it's an int type rather than size_t 
		int getSize() const {
			return size;
		}
		
		// gives a reference to the data, same implementations
		// if needed, also add a const before int index
		T& operator[](int index) {
			Node* refdata = head;
			// over the range of size
			if ((index < 0) || (index >= size)) {
				throw std::exception();
			}
			else {
				int i = 0;
				while (i < index) {
					refdata = refdata -> next;
					i++;
				}
				return refdata -> data;
			}
		}
		
		const T& operator[](int index) const {
			Node* refdata = head;
			// over the range of size
			if ((index < 0) || (index >= size)) {
				throw std::exception();
			}
			else {
				int i = 0;
				while (i < index) {
					refdata = refdata -> next;
					i++;
				}
				return refdata -> data;
			}
		}
		
		// generic destroy for both = and destructor
		void destroy() {
			// though NEVER MAKE ASSUMPTIONS, I assume the ADT is well constructed
			// so that I can use brute-force to SMASH IT DOWN!!!
			Node* temp = NULL;
			while (head != NULL) {
				temp = head -> next;
				delete head;
				head = temp;
			}
			size = 0;
			tail = NULL;
		}
		
		// destructor
		~LinkedList() {
			destroy();
		}
};

#endif
