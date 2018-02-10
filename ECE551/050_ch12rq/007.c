#include <stdio.h>
#include <stdlib.h>

int main(){
  int * p = malloc(sizeof(int));
  int * q = p;
  free(p);
  free(q);
  free(NULL);
} 

