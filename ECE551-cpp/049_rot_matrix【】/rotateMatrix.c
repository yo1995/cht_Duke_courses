#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>



int rotateMatrix(FILE * f){
	//isascii();
	char matrix[12][12] = {0}; //it can return any possible character value read from the file
	int i = 0;
	int j = 0;
	char temp2;
	char * ptr;
	// how to check if the matrix exceed in width
	//reading the matrix
	while (!feof(f)){ 
		fgets(matrix[i], 12, f);
		if (feof(f) && i>9){
			break; //test EOF ugly ugly so ugly TAT
		}
		ptr = strchr(matrix[i],'\n');
		//first time forgot to test if it's too long or too short in rangs
		if (i < 10) {
			if (ptr - matrix[i] != 10){
				return EXIT_FAILURE;
			}
		}
		else {
			return EXIT_FAILURE; //when i = 10 the file pointer should reach EOF
		}
		i++;
	}
	//first flip updown
	for (i = 0; i < 5; i++){
		for (j = 0; j < 10; j++){
			temp2 = matrix[i][j];
			matrix[i][j] = matrix[9-i][j];
			matrix[9-i][j] = temp2;
		}
	}
	//then flip diagonal
	for (i = 0; i < 9 ; i++){
		for (j = i + 1; j < 10; j++){
			temp2 = matrix[i][j];
			matrix[i][j] = matrix[j][i];
			matrix[j][i] = temp2;
		}
	}
	// after writing this I was told that I can just read and write into the matrix other than read first and then rotate.
	// print to stdout
	for (i = 0; i< 10; i++){
		for (j = 0; j< 10; j++){
			printf("%c",matrix[i][j]);
		}
		printf("\n");
	}
	return EXIT_SUCCESS;
}


int main(int argc, char ** argv) {
	if (argc != 2) {
		fprintf(stderr,"Wrong inputFileName!\n");
		return EXIT_FAILURE;
	}
	FILE * f = fopen(argv[1], "r");
	if (f == NULL) {
		//perror("Could not open file, either non-exist or wrong!");
		fprintf(stderr,"Could not open file, either non-exist or wrong!\n");
		return EXIT_FAILURE;
	}
	int r;
	r = rotateMatrix(f);
	if (r == EXIT_FAILURE){
		//printf("error in rotate\n");
		fprintf(stderr,"Invalid input: Line length is not valid\n");
		return EXIT_FAILURE;
	}
	
	if (fclose(f) != 0) {
		//perror("Failed to close the input file!");
		fprintf(stderr,"Failed to close the input file!\n");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
