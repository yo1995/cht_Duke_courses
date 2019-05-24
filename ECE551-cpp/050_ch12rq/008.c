#include <stdio.h>
#include <stdlib.h>

int main(){
	int xcount=0;
	char * ptr = malloc(50 * sizeof(char));
	if(fgets(ptr, 50, stdin)!= NULL) {
   		while(*ptr != '\0') {
      	if (*ptr == 'x') {
         	xcount++;
      	}
    	ptr++;
   		}
 	}
	free(ptr);
} 

