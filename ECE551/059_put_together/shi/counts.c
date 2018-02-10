#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "counts.h"
counts_t * createCounts(void) {
  //WRITE ME
  counts_t * cur = malloc(sizeof(*cur));
  cur->array = NULL;
  cur->size = 0;
  cur->uSize = 0;
  return cur;
}
void addCount(counts_t * c, const char * name) {
  //WRITE ME
  if (c == NULL) {
    exit(EXIT_FAILURE);
  }
  if (name == NULL) {
    c->uSize++;
    return;
  }
  for (int i = 0; i < c->size; i++) {
    if (strcmp(c->array[i].string, name) == 0) {
      c->array[i].time++;
      return;
    }
  }
  c->size++;
  c->array = realloc(c->array, (c->size)*sizeof(*c->array));
  c->array[c->size-1].string = strdup(name);
  c->array[c->size-1].time = 1;
}
void printCounts(counts_t * c, FILE * outFile) {
  //WRITE ME
  if (c == NULL || outFile == NULL) {
    exit(EXIT_FAILURE);
  }
  for (int i = 0; i < c->size; i++) {
    fprintf(outFile, "%s: %d\n",c->array[i].string, c->array[i].time);
  }
  if (c->uSize != 0) {
    fprintf(outFile, "<unknown> : %d\n", c->uSize);
  }
  if (fclose(outFile) != 0) {
    exit(EXIT_FAILURE);
  }
}

void freeCounts(counts_t * c) {
  //WRITE ME
  for (int i = 0; i < c->size; i++) {
    free(c->array[i].string);
  }
  free(c->array);
  free(c);
}
