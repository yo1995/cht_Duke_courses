#include <stdio.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include "readFreq.h"

void printSym(std::ostream & s, unsigned sym) {
	if (sym > 256 ) {
		s << "INV";
	}
	else if (sym == 256) {
		s << "EOF";
	}
	else if (isprint(sym)) {
		char c = sym;
		s << "'" << c << "'";
	}
	else {
		std::streamsize w = s.width(3);
		s << std::hex << sym << std::dec;
		s.width(w);
	}
}

uint64_t* readFrequencies(const char * fname) {
	//WRITE ME!
	std::ifstream input;
	int c;
	input.open(fname);
	if(input.fail()) {
		perror("Could not open file");
		exit(EXIT_FAILURE);
	}
	uint64_t* freq_array = new uint64_t[257]();
	while((c = input.get()) != EOF) {
		freq_array[c]++;
	}
	freq_array[256]++;
	input.close();
	return freq_array;
}
