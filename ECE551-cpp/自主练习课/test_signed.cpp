#include <stdio.h>
#include <stdlib.h>

int main(void) {
   int a = -1;
   unsigned int b = 0;
   if(a + b > 0) {  //did we get -1 or 4billion?
      printf("Unsigned %d\n", a + b);
   }
   else {
     printf("Signed %d\n", a + b);
   }
   return EXIT_SUCCESS;
}
