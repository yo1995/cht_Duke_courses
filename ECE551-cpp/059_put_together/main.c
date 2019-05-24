#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kv.h"
#include "counts.h"
#include "outname.h"

//this one is IMPORTANT
void getline_helper(FILE *f, kvarray_t * kvPairs, counts_t *count) {
	char *line = NULL; 
	char *line_copy = NULL;
	size_t sz = 0;
	//char ** data = NULL;
	int length_line = 0;
	while(getline(&line, &sz, f) >= 0) {
		line_copy = line;
		length_line = strlen(line_copy);
		//directly cut the line
		line_copy[length_line - 1] = '\0';
		//printf("the trunc-ed str is %s", line_copy);
		addCount(count, lookupValue(kvPairs, line_copy));
		line_copy = NULL;
		free(line);
		line=NULL;
		sz = 0;
	}
	free(line);
	line = NULL;
	//return data;		
}

counts_t * countFile(const char * filename, kvarray_t * kvPairs) {
  //WRITE ME
	//pre-def
	counts_t *count = createCounts();
	//open file part
	FILE *f = fopen(filename, "r");
	if (f == NULL) {
		perror("Could not open file");
		printf("FileName: %s\n", filename);
		return NULL;
	} 
	//we could either use getline or fgets, but fgets need to judge the length
	getline_helper(f, kvPairs, count);
	if (fclose(f) != 0) {
		perror("Failed to close the countFile input file!");
		return NULL;
	}
	f = NULL;
	return count;
}

int main(int argc, char ** argv) {
  //WRITE ME (plus add appropriate error checking!)
	//make sure no invalid input
	if(argc <= 2) {
		fprintf(stderr,"Please provide the right filename!\n");
		return EXIT_FAILURE;
	}
	int i = 2;
	FILE *f = NULL;
	//read the key/value pairs from the file named by argv[1] (call the result kv)
	kvarray_t *kv = readKVs(argv[1]);
	counts_t *c = NULL;
	char *outFileName = NULL;
	//count from 2 to argc (call the number you count i)
	while(i < argc) {
		//count the values that appear in the file named by argv[i], using kv as the key/value pair
		//   (call this result c)
		c = countFile(argv[i], kv);
		//compute the output file name from argv[i] (call this outName)
		outFileName = computeOutputFileName(argv[i]);
		//open the file named by outName (call that f)
		f = fopen(outFileName, "w+");
		//print the counts from c into the FILE f
		printCounts(c, f);
		//close f
		if (fclose(f) != 0) {
			perror("Failed to close the printCounts output file!");
			return EXIT_FAILURE;
		}
		//free the memory for outName and c
		freeCounts(c);
		free(outFileName);
		outFileName = NULL;
		f = NULL;
		i++;
	}
	//free the memory for kv
	freeKVs(kv);
	return EXIT_SUCCESS;
}
