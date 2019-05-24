#include <cstdio>
#include <cstdlib>
#include "function.h"

// we can either use iteration or recursion. iteration is easier to write.
int binarySearchForZero_helper(Function<int, int> * f, int low, int high) {
	int mid;
	int f_x;
	while (low <= high) {
		mid = (low + high) / 2;
		f_x = f -> invoke(mid);
		if (f_x == 0) {
			return mid;
		}
		else if (f_x < 0) {
			low = mid + 1;
		}
		else {
			high = mid - 1;
		}
	}
	
	if (mid > high) {
		return mid - 1;
	}
	return mid;
}


int binarySearchForZero(Function<int, int> * f, int low, int high) {
	// If f(x) is all positive, you should return low
	if(low == high) {
		return low;
	}
	if (f -> invoke(low) >= 0) {
		return low;
	}
	// Note that if f(x) is all negative, the answer is (high-1)
	if (f -> invoke(high - 1) <= 0) {
		return high - 1;
	}
	// it might be a case that all negative, we should return high - 1 though low = high
	
	// other cases, use binary search technique
	return binarySearchForZero_helper(f, low, high - 1);
}
