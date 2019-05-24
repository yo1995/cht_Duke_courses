#include "IntMatrix.h"

IntMatrix::IntMatrix(){
	numRows = 0;
	numColumns = 0;
	rows = NULL;
}

IntMatrix::IntMatrix(int r, int c) {
	numRows = r;
	numColumns = c;
	rows = new IntArray*[r];
	int i = 0;
	while (i < r) {
		rows[i] = new IntArray(c);
		i++;
	}
}

IntMatrix::IntMatrix(const IntMatrix & rhs)  {
	numRows = rhs.numRows;
	numColumns = rhs.numColumns;
	rows = new IntArray*[numRows];
	int i = 0;
	while (i < numRows) {
		rows[i] = new IntArray(numColumns);
		*rows[i] = *rhs.rows[i]; //pay attention to the notation
		i++;
	}
}

IntMatrix::~IntMatrix() {
	int i = 0;
	while (i < numRows) {
		delete rows[i];
		i++;
	}
	delete[] rows;
}

IntMatrix &IntMatrix::operator=(const IntMatrix & rhs) {
	//first to delete the prior rows.
	int i = 0;
	while (i < numRows) {
		delete rows[i];
		i++;
	}
	delete[] rows;
	//complete
	numRows = rhs.numRows;
	numColumns = rhs.numColumns;
	rows = new IntArray*[numRows];
	i = 0;
	while (i < numRows) {
		rows[i] = new IntArray(numColumns);
		*rows[i] = *rhs.rows[i];
		i++;
	}
	return *this;
}

int IntMatrix::getRows() const {
	return numRows;
}

int IntMatrix::getColumns() const {
	return numColumns;
}

const IntArray & IntMatrix::operator[](int index) const {
	assert((index >=0 ) && (index< numRows));
	return *rows[index];
}

IntArray & IntMatrix::operator[](int index){
	assert((index >=0 ) && (index< numRows));
	return *rows[index];
}

bool IntMatrix::operator==(const IntMatrix & rhs) const {
	if ((numRows != rhs.numRows) || (numColumns != rhs.numColumns)) {
		return false;
	}
	else {
		int i = 0;
		while(i < numRows) {
			if (*rows[i] != *rhs.rows[i]) {
				return false; //pay attention to the notation
			}
			i++;
		}
		return true;
	}
}

IntMatrix IntMatrix::operator+(const IntMatrix & rhs) const {
	assert((numRows == rhs.numRows) && (numColumns == rhs.numColumns));
	IntMatrix sumMat(numRows,numColumns);
	int i = 0;
	int j = 0;
	while(i < numRows) {
		while (j < numColumns) {
			(*sumMat.rows[i])[j] = (*rows[i])[j] + (*rhs.rows[i])[j]; //pay attention to the notation
			j++;
		}
		j = 0;
		i++;
	}
	return sumMat;
}

std::ostream & operator<<(std::ostream & s, const IntMatrix & rhs) {
	s << "[ ";
	int i = 0;
	while (i < rhs.getRows() - 1) {
		s << rhs[i] << ",\n";
		i++;
	}
	if (rhs.getRows() > 0) {
		s << rhs[rhs.getRows() - 1] << " ]"; //print the single line
	}
	else {
		s << " ]"; //empty Matrix
	}
	return s;
}
