#include <stdio.h>

int main(void) {
  int a = 3;
  int b = 9;
  while (a <= b) {
     if (a % 2 == 1) {
        printf("a is %d\n", a);
     }
     else {
       printf("b is %d\n", b);
       for (int i = 0; i < b - a ; i++) {
          printf("a * i + b = %d\n", a * i + b);
       }
     }
     a++;
     b--;
  }  
  getchar();
  return 0;
  
}
