#ifndef __T_MATRIX_H___
#define __T_MATRIX_H___

#include <assert.h>
#include <iostream>
#include <vector>
#include <cstdlib>

//YOUR CODE GOES HERE!
template<typename T>
class Matrix {
	private:
		int numRows;
		int numColumns;
		std::vector< std::vector<T> > rows; //mind the white space
	public:
		Matrix() {
			numRows = 0;
			numColumns = 0;
			rows.clear();
		}
		
		Matrix(int r, int c) {
			numRows = r;
			numColumns = c;
			rows.resize(r);
			int i = 0;
			while (i < r) {
				rows[i].resize(c);
				i++;
			}
		}
		
		Matrix(const Matrix & rhs) {
			numRows = rhs.numRows;
			numColumns = rhs.numColumns;
			rows.resize(numRows);
			int i = 0;
			while (i < numRows) {
				rows[i].resize(numColumns);
				rows[i] = rhs.rows[i]; //pay attention to the notation
				i++;
			}
		}
		
		~Matrix() {
			int i = 0;
			while (i < numRows) {
				rows[i].clear();
				i++;
			}
			rows.clear();
		}
		
		Matrix & operator=(const Matrix & rhs) {
			//first to delete the prior rows.
			int i = 0;
			while (i < numRows) {
				rows[i].clear();
				i++;
			}
			rows.clear();
			//complete
			numRows = rhs.numRows;
			numColumns = rhs.numColumns;
			rows.resize(numRows);
			i = 0;
			while (i < numRows) {
				rows[i].resize(numColumns);
				rows[i] = rhs.rows[i]; //pay attention to the notation
				i++;
			}
			return *this;
		}
		
		int getRows() const {
			return numRows;
		}
		
		int getColumns() const {
			return numColumns;
		}
		
		const std::vector<T> & operator[](int index) const {
			assert((index >=0 ) && (index< numRows));
			return rows[index];
		}
		
		std::vector<T> & operator[](int index) {
			assert((index >=0 ) && (index< numRows));
			return rows[index];
		}
		
		bool operator==(const Matrix & rhs) const {
			if ((numRows != rhs.numRows) || (numColumns != rhs.numColumns)) {
				return false;
			}
			else {
				int i = 0;
				while(i < numRows) {
					if (rows[i] != rhs.rows[i]) {
						return false; //pay attention to the notation
					}
					i++;
				}
				return true;
			}
		}
		
		Matrix operator+(const Matrix & rhs) const {
			assert((numRows == rhs.numRows) && (numColumns == rhs.numColumns));
			Matrix sumMat(numRows,numColumns);
			int i = 0;
			int j = 0;
			while(i < numRows) {
				while (j < numColumns) {
					sumMat.rows[i][j] = rows[i][j] + rhs.rows[i][j]; //pay attention to the notation
					j++;
				}
				j = 0;
				i++;
			}
			return sumMat;
		}
};

//before overload << for Matrix, we still need the << for array...
template<typename T>
std::ostream & operator<<(std::ostream & s, const std::vector<T> & rhs) {
	unsigned int i = 0;
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

template<typename T>
std::ostream & operator<<(std::ostream & s, const Matrix<T> & rhs) {
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


#endif
