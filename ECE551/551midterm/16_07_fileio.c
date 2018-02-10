#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char ** argv) {
  FILE *f= fopen(argv[1],"r");
  int size = strlen(argv[2]);
  int value[size] = {0};
  int c;
  while ((c = fgetc(f))!= EOF) {
    for (int i = 0; i < size; ++i) {
      if (c == argv[2][i]) {
        value[i]++;
      }
    }
  }
  for (int j = 0; j < size; ++j) {
    printf("%c : %d\n",argv[2][j],value[j]);
  }
  fclose(f);
  return 0;
}