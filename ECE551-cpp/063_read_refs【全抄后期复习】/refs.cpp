#include <stdio.h>
#include <stdlib.h>

int & f(int * ptr, int & r) {
  r ++;
  ptr[3] += r;
  return ptr[1];
}

int main(void) {
  int data[] = {6, 7, 8, 9,10};
  int & x = data[1];
  x = data[2];
  int * p = &x;
  f(p, data[3]) = 47;
  for (int i = 0; i < 5; i++) {
    printf("data[%d] = %d\n", i, data[i]);
  }
  printf("x = %d\n", x);
  return EXIT_SUCCESS;
}

//第一步：变成6 8 8 9 10
//第二步：传进去p 和 9的ref
//第三步：9++ >>> 10，10+10 = 20
//第四步：6 8 47 10 20