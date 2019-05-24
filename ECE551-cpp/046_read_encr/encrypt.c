#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

void encrypt(FILE * f, int key){
  int c;
  while ((c = fgetc(f)) != EOF) {
    if (isalpha(c)) { //判断是否为英文字母 
      c = tolower(c);
      c -= 'a'; //转换为对应字母的1-26的数值 
      c += key; 
      c %= 26; //循环移位 
      c += 'a';
    }
    printf("%c", c);
  }
}

int main(int argc, char ** argv) { //检测输入合法性 
  if (argc != 3) {
    fprintf(stderr,"Usage: encrypt key inputFileName\n");
    return EXIT_FAILURE;
  }
  int key = atoi(argv[1]);
  if (key == 0) {
    fprintf(stderr,"Invalid key (%s): must be a non-zero integer\n", argv[1]);
    return EXIT_FAILURE;
  }
  FILE * f = fopen(argv[2], "r");
  if (f == NULL) {
    perror("Could not open file");
    return EXIT_FAILURE;
  }
  encrypt(f,key);
  if (fclose(f) != 0) {
    perror("Failed to close the input file!");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
