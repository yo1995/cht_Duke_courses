#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kv.h"

char ** readstrings(FILE *f, int *count)
{
	char **data=NULL; char *line=NULL; size_t sz=0;
	*count=0;
	while(getline(&line,&sz,f)>=0)
	{
		*count+=1;
		data=realloc(data,*count*sizeof(*data));
		data[*count-1]=line;
		line=NULL;
	}
	free(line);
	line=NULL;
	return data;
}
		

kvarray_t * readKVs(const char * fname) {
	if(fname==NULL)
		return NULL;
	FILE *input=fopen(fname, "r");
	if(input==NULL)
	{
		fprintf(stderr,"Could not open the file");
		return NULL;
	}
	int count=0;
	char **data=readstrings(input,&count);

        int j=0,key_length=0,value_length=0,flag=0;
	char *key_temp=NULL,*value_temp=NULL;	 

	kvarray_t *pair_array=malloc(sizeof(*pair_array));
	pair_array->length=count;
	pair_array->pairs=malloc(count*sizeof(*pair_array->pairs));

	for(int i=0;i<count;i++)
	{	
	  j=0;key_length=0;value_length=0;flag=0;key_temp=NULL;value_temp=NULL;
	  while(data[i][j]!='\n')
	  {
		if( (data[i][j]=='=')&&(flag==0) )
		{
			flag=1;
		}
		else if(flag==0)
		{
			key_length++;
		        key_temp=realloc(key_temp,key_length*sizeof(*key_temp));
			key_temp[key_length-1]=data[i][j];
		}
		else if(flag==1)
		{
			value_length++;
			value_temp=realloc(value_temp,value_length*sizeof(*value_temp));
			value_temp[value_length-1]=data[i][j];
		}
		j++;
	  }
	  key_temp=realloc(key_temp,(key_length+1)*sizeof(*key_temp));
	  key_temp[key_length]='\0';
	  value_temp=realloc(value_temp,(value_length+1)*sizeof(*value_temp));
	  value_temp[value_length]='\0';
          
	  pair_array->pairs[i].key=key_temp;
	  pair_array->pairs[i].value=value_temp;
	  free(data[i]);
	  data[i]=NULL;
	}
	free(data);
	fclose(input);  
	return pair_array;
}



void freeKVs(kvarray_t * pairs) {
  if(pairs!=NULL)
  {
	for(int i=0;i<pairs->length;i++)
	{
		free(pairs->pairs[i].key);
		free(pairs->pairs[i].value);
	}
	free(pairs->pairs);
        free(pairs);
  }	
}

void printKVs(kvarray_t * pairs) {
  if(pairs!=NULL)
  {
  	for(int i=0;i<pairs->length;i++)
	{
		printf("key = '%s' value = '%s'\n",pairs->pairs[i].key,pairs->pairs[i].value);
	}
  }
 
}

char * lookupValue(kvarray_t * pairs, const char * key) {
  if( (pairs!=NULL)&&(key!=NULL) )
  {
  	for(int i=0;i<pairs->length;i++)
	{
		if(strcmp(pairs->pairs[i].key,key)==0)
		{
			return pairs->pairs[i].value;
		}
	}
	return NULL;
  }
  else
	return NULL;
}
