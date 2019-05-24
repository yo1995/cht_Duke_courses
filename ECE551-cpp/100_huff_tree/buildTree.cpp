#include "node.h"

Node *  buildTree(uint64_t * counts) {
	//WRITE ME!
	priority_queue_t pq;
	int i = 0;
	while (i < 257) {
		if (counts[i] != 0) {
			pq.push(new Node(i, counts[i]));
		}
		i++;
	}
	if (pq.empty()) {
		return NULL;
	}
	// by definition in node.h, the passed-ins should be pointers
	Node* top0 = NULL;
	Node* top1 = NULL;
	while (pq.size() > 1) {
		top0 = pq.top();
		pq.pop();
		top1 = pq.top();
		pq.pop();
		pq.push(new Node(top0, top1));
	}
	return pq.top();
}
