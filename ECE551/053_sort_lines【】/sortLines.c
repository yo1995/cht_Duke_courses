#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//This function is used to figure out the ordering
//of the strings in qsort.  You do not need
//to modify it.
int stringOrder(const void * vp1, const void * vp2) {
	const char * const * p1 = vp1;
	const char * const * p2 = vp2;
	return strcmp(*p1, *p2);
}
//This function will sort and print data (whose length is count).
void sortData(char ** data, size_t count) {
	qsort(data, count, sizeof(char *), stringOrder);
}

//This part is extremely tricky, since I have forgotten to free all the memory occupied by data.
void print_and_free_all_of_the_lines(char ** data, int count){
	int i = 0;
	while(count > i){
		printf("%s", *(data + i));
		free(*(data + i));
		i++;
	}
	free(data); //free the pointer to pointer at last
}

// we can just loop for one round to both print and free the data lines.
/* void print_the_results(char ** data, int count){
	int i = 0;
	while(count > i){
		printf("%s", *(data + i));
		i++;
	}
} */

int main(int argc, char ** argv) {
	//WRITE YOUR CODE HERE!
	char * line = NULL;
	size_t sz = 0;
	int count = 0; //count the lines I've read from stdin
	int n_of_argc;
	FILE * f = NULL;
	//first, with 0 arguments, read from stdin
	char ** data = NULL;
	if (argc == 1) {
		while(getline(&line, &sz, stdin) >= 0){ //need no fclose
			count += 1;
			data = realloc(data, count * sizeof(char *));
			*(data + count - 1) = line;
			line = NULL; // so that next read can still be automatic
		}
		free(line); //free input line after use
		sortData(data, count);
		//line has been copied thus do not need to be freed iteratively
		print_and_free_all_of_the_lines(data, count);
	}
	//second, with more than 0 arguments, loop throngh the parameters
	else{
		n_of_argc = 1;
		while(n_of_argc < argc){
			//need to initialize each parameters before going into next level of loop
			line = NULL;
			sz = 0;
			count = 0;
			data = NULL;
			//again read the file
			f = fopen(argv[n_of_argc], "r");
			if (f == NULL) {
				perror("Could not open file");
				printf("FileName: %s\n", argv[n_of_argc]);
				return EXIT_FAILURE;
			}
			while(getline(&line, &sz, f) >= 0){
				count += 1;
				data = realloc(data, count * sizeof(char *));
				*(data + count - 1) = line;//we need to replicate the lines with data
				line = NULL;
			}
			free(line); //free input line after use
			sortData(data, count);
			print_and_free_all_of_the_lines(data, count);
			if (fclose(f) != 0) {
				perror("Failed to close the input file!");
				return EXIT_FAILURE;
			}
			n_of_argc++;
		}
	}
	return EXIT_SUCCESS;
}
