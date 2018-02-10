#include <stdio.h>


int main(){
	int * array = malloc(sizeof(*array));
	free(array);
	//free(&array[0]);//µÈ¼Û
}

