#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "counts.h"

counts_t * createCounts(void) {
  //WRITE ME
	counts_t *count_structure = malloc(sizeof(*count_structure));
	count_structure -> count = 0;
	count_structure -> unknown_count = 0;
	count_structure -> one_count = NULL;
	return count_structure;
}

void addCount(counts_t * c, const char * name) {
  //WRITE ME
	int i;
	int flag = 0;;
	if (name == NULL) {
		c -> unknown_count ++;
	}
	else {
		i = 0;
		while(i < (c -> count)) {
			//first to compare if the name is already in the list
			if(!strcmp(c -> one_count[i].string, name)){
				flag = 1;
				break;
			}
			i++;
		}
		//should think carefully about the loop and the condition
		//wrongly put this assign into loop before
		if (flag) {
			c -> one_count[i].times++;
			flag = 0;
		}
		else {
			//then add new items
			c -> one_count = realloc(c -> one_count, (c -> count + 1) * sizeof(*c -> one_count));
			c -> one_count[c -> count].string = malloc(strlen(name) * sizeof(c -> one_count[c -> count].string) );
			strcpy(c -> one_count[c -> count].string, name);
			c -> one_count[c -> count].times = 1;
			c -> count ++;
		}
	}
}

void printCounts(counts_t * c, FILE * outFile) {
  //WRITE ME
	int i = 0;
	while(i < (c -> count)) {
		fprintf(outFile, "%s: %d\n", c -> one_count[i].string, c -> one_count[i].times);
		i++;
	}
	if(c->unknown_count!=0){
		fprintf(outFile, "<unknown> : %d\n", c->unknown_count);
	}
}

void freeCounts(counts_t * c) {
  //WRITE ME
	int i = 0;
	while(i < (c -> count)) {
		free(c -> one_count[i].string);
		i++;
	}
	free(c -> one_count);
	free(c);
}
