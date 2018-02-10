#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void strfill(char * p, size_t n, char c) {
  for (size_t i = 0; i < n; i++) {
    p[i] = c;
  }
  p[n] = '\0';
}
char * rexpand(const char * str, size_t * counts) {
  if (*str == '\0') {
    return strdup("");
  }
  char * temp = rexpand(str+1, counts+1);
  size_t len = strlen(temp);
  size_t newsize = len + *counts +1;
  temp = realloc(temp,  newsize * sizeof(*temp));
  strfill(temp + len, *counts, *str);
  return temp;
}

int main(){
	char a[] = "abcde\n";
	char *b = NULL;
	size_t c = 5;
	size_t *d;
	d = &c;
	b = rexpand(a, d);
	puts(b);
	printf("%lu \n", *d);
}
