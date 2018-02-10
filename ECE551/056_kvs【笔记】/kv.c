#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kv.h"



kvarray_t * readKVs(const char * fname) {
	//WRITE ME
	//open the file
	FILE * f;
	f = fopen(fname, "r");
	if (f == NULL) {
		perror("Could not open file");
		printf("FileName: %s\n", fname);
		//????WHETHER SHOULD USE NULL OR EXIT????
		return NULL;
		//exit(EXIT_FAILURE);
	}
	//read lines of the text
	char *line = NULL;
	//copy of pointer line
	char *line_temp = NULL;
	size_t sz = 0;
	int count = 0;
	int i;
	int j;
	//mark the equal sign in the line
	int flag_for_key_value = 0;
	char *key_temp = NULL;
	char *value_temp = NULL;
	kvarray_t *data = malloc(sizeof(*data));
	data -> pairs = NULL;
	//in the loop, first split the line into key and value
	//then store them into data
	while(getline(&line, &sz, f) >= 0){
		count++;
		i = 0; 
		j = 0;
		
		data -> pairs = realloc(data -> pairs, count * sizeof(*data -> pairs));
		line_temp = line;
		//printf("Test to see initial line_temp %s\n",line_temp);
		//line_temp/line is not a pointer?
		while(*line_temp != '\n'){
			if((*line_temp == '=') && (flag_for_key_value == 0)){	
				flag_for_key_value = 1;
			}
			else if(flag_for_key_value == 0){
				i++;
				key_temp = realloc(key_temp,i * sizeof(*key_temp));
				*(key_temp + i - 1) = line_temp[0];
			}
			else if(flag_for_key_value == 1){
				j++;
				value_temp = realloc(value_temp,j * sizeof(*value_temp));
				*(value_temp + j - 1) = line_temp[0];
			}
			//printf("Test to see line_temp %s",line_temp);
			//printf("flag = %d\n", flag_for_key_value);
			line_temp++;
		}
		//printf("if reached here");
		key_temp = realloc(key_temp, (i + 1) * sizeof(*key_temp));
		*(key_temp + i) = '\0';
		value_temp = realloc(value_temp, (j + 1) * sizeof(*value_temp));
		*(value_temp + j) = '\0';
		
		data -> pairs[count - 1].key = key_temp;
		data -> pairs[count - 1].value = value_temp;
		//re-initialize all the temp pointers
		//extremely tricky case here. line need to be free-d
		free(line);
		line = NULL;
		//why key_temp need no freeing? - data -> pairs copied it
		line_temp = NULL;
		key_temp = NULL;
		value_temp = NULL;
		flag_for_key_value = 0;
	}
	free(line);
	free(line_temp);
	free(key_temp);
	free(value_temp);
	if (fclose(f) != 0) {
		perror("Failed to close the input file!");
		return NULL;
		//return EXIT_FAILURE;
	}
	//the length of lines/arrays
	data -> length = count;
	return data;
}

void freeKVs(kvarray_t * pairs) {
  //WRITE ME
	int i = 0;
	//check if we have valid input
	if (pairs != NULL){
		while (i < pairs -> length){
			free(pairs -> pairs[i].key);
			free(pairs -> pairs[i].value);
			i++;
		}
		free(pairs -> pairs);
		free(pairs);
		//all free-d up! nice~
	}
}

void printKVs(kvarray_t * pairs) {
  //WRITE ME
	int i = 0;
	if (pairs != NULL){
		while(i < pairs -> length){
			printf("key = '%s' value = '%s'\n", pairs -> pairs[i].key, pairs -> pairs[i].value);
			i++;
		}
	}
}

char * lookupValue(kvarray_t * pairs, const char * key) {
  //WRITE ME
	int i = 0;
	if ((pairs != NULL) && (key != NULL)){
		while(i < pairs -> length){
			if(!strcmp(pairs -> pairs[i].key, key)){
				return pairs->pairs[i].value;
			}
			i++;
		}
		return NULL;
	}
	else{
		return NULL;
	}
}
