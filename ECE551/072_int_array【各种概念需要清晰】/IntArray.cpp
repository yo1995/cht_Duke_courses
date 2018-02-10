#include "IntArray.h"
#include <assert.h>
#include <ostream>

IntArray::IntArray() {
	data = NULL;
	numElements = 0;
}

IntArray::IntArray(int n) {
	numElements = n;
	data = new int[n];
}

IntArray::IntArray(const IntArray & rhs) {
	numElements = rhs.numElements;
	data = new int[numElements];
	int i = 0;
	while(i < numElements){
		data[i] = rhs.data[i];
		i++;
	}
}

IntArray::~IntArray() {
	//attention the usage of [] for array
	delete[] data;
}

IntArray & IntArray::operator=(const IntArray & rhs) {
	//should delete the former created space for data so it will not be lost
	delete[] data;
	numElements = rhs.numElements;
	data = new int[numElements];
	int i = 0;
	while(i < numElements){
		data[i] = rhs.data[i];
		i++;
	}
	return *this;
}

//the normal behavior of [] should return data[i] itself
const int & IntArray::operator[](int index) const {
	assert((index >= 0) && (index < numElements));
	return data[index];
}

int & IntArray::operator[](int index) {
	assert((index >= 0) && (index < numElements));
	return data[index];
}

int IntArray::size() const {
	return numElements;
}

bool IntArray::operator==(const IntArray & rhs) const {
	int i = 0;
	if (numElements != rhs.numElements) {
		return false;
	}
	else {
		while (i < numElements) {
			if (data[i] != rhs.data[i]) {
				return false;
			}
			i++;
		}
		return true;
	}
}

bool IntArray::operator!=(const IntArray & rhs) const {
	if (*this == rhs) {
		return false; //NOT not equal to
	}
	else {
		return true; //IS not equal to
	}
}

std::ostream & operator<<(std::ostream & s, const IntArray & rhs) {
	int i = 0;
	s << '{';
	//tricky at last comma
	while (i < rhs.size() - 1) {
		s << rhs[i] << ", ";
		i++;
	}
	if (rhs.size() > 0) {
		s << rhs[rhs.size() - 1]; //use of i will cause empty array fault
	}
	s << '}';
	return s;
}
