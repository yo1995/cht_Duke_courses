#include "bstset.h"
#include "set.h"
#include <iostream>
#include <cstdlib>

int main(void) {
	BstSet<int> set;
	// using the answer of 091_traversals - 3
	set.add(2);
	set.add(4);
	set.add(1);
	set.add(7);
	set.add(3);
	set.add(9);
	set.add(-1);
	set.add(88);
	set.add(17);
	set.add(-23);
	set.add(16);
	set.add(44);
	set.add(77);
	
	set.remove(2);
	set.remove(16);
	set.remove(-1);
	
	std::cout << "mid_order: " << std::endl;
	set.inorder();
	std::cout << "value contain 1?:" << set.contains(1) << std::endl;
	
	//std::cout<<"value of 1 is:"<<set.lookup(9)<< std::endl; 
	
	return EXIT_SUCCESS;
}
