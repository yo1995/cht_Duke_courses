#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

//find the largest element in the array
int arrayMax(int * array, int n) {
  int * a = array;
  int * s = array; //static pointer
  int i; 
  for (i = 0 ; i < n-1 ; i++){
  		if (*a < *(array+i+1)){
  			a = array+i+1;
		}	
  }
  return (a-s);
}


int counting_decipher(FILE * f){
  int c;
  int alphabet_array[26]={0};
  while ((c = fgetc(f)) != EOF) {
    	if (isalpha(c)) { 
			c = tolower(c);
			c -= 'a';
			alphabet_array[c] += 1;
    	}
	}
	int the_key_to_decipher;
	the_key_to_decipher = arrayMax(alphabet_array,26);
	int key;
	key = the_key_to_decipher + 'a' - 'e';
	if (key < 0){
		key += 26;
	}
	return key;
}


int main(int argc, char ** argv) {
	if (argc != 2) {
		fprintf(stderr,"No inputFileName!\n");
		return EXIT_FAILURE;
	}
	int key;
	FILE * f = fopen(argv[1], "r");
	if (f == NULL) {
		perror("Could not open file");
		return EXIT_FAILURE;
	}
	key = counting_decipher(f);
	printf("%d\n",key);
	if (fclose(f) != 0) {
		perror("Failed to close the input file!");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

