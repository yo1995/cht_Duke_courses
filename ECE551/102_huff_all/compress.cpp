#include <iostream>
#include <stdio.h>
#include <stdint.h>
#include <map>
#include <queue>
#include <assert.h>
#include <functional>
#include <stdlib.h>
#include <fstream>
#include "readFreq.h"
#include "node.h"



void writeHeader(BitFileWriter * bfw, const std::map<unsigned,BitString> &theMap) {
  for (int i =0 ; i < 257; i++) {
    std::map<unsigned,BitString>::const_iterator it = theMap.find(i);
    if (it != theMap.end()) {
      bfw->writeByte(it->second.size());
      bfw->writeBitString(it->second);
    }
    else {
      bfw->writeByte(0);
    }
  }
}

void writeCompressedOutput(const char* inFile,
			   const char *outFile,
			   const std::map<unsigned,BitString> &theMap ){
  BitFileWriter bfw(outFile);
  writeHeader(&bfw,theMap);

	//WRITE YOUR CODE HERE!
	//open the input file for reading
	std::ifstream input;
	input.open(inFile);
	int c;
	if(input.fail()) {
		perror("Could not open file");
		exit(EXIT_FAILURE);
	}
	//You need to read the input file, lookup the characters in the map,
	//and write the proper bit string with the BitFileWriter
	std::map<unsigned,BitString>::const_iterator it;
	while((c = input.get()) != EOF) {
		it = theMap.find((unsigned)c);
		// character not found
		assert(it != theMap.end());
		bfw.writeBitString(it -> second);
	}
	//dont forget to lookup 256 for the EOF marker, and write it out.
	it = theMap.find(256);
	//BitFileWriter will close the output file in its destructor
	bfw.writeBitString(it->second);
	//but you probably need to close your input file.
	input.close();
}

int main(int argc, char ** argv) {
	if (argc != 3) {
		fprintf(stderr,"Usage: compress input output\n");
		return EXIT_FAILURE;
	}
	//WRITE YOUR CODE HERE
	//Implement main
	uint64_t * counts = readFrequencies(argv[1]);
	assert(counts != NULL);
	Node * tree = buildTree (counts);
	delete[] counts;
	std::map<unsigned,BitString> theMap;
	BitString empty;
	tree -> buildMap(empty, theMap);
	writeCompressedOutput(argv[1], argv[2], theMap);
	delete tree;
	return EXIT_SUCCESS;
	//hint 1: most of the work is already done. 
	//hint 2: you can look at the main from the previous tester for 90% of this
}
