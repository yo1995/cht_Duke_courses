#include <stdio.h>
#include <stdlib.h>



int * initArray(int * array, int n){
  for (int i = 0; i < n; i++){
    array[i] = i;
  }
  return array;
}

int * aFunction(int n) {
  int myArray[n]={0};
  
  int * ptr = initArray(myArray, n-1);
  int total = 0;
  for (int i = 0; i < n-1; i++) {
    printf("%d\n", myArray[i]);
    total += ptr[i]; //*ptr[i]
  }
  printf("Total = %d\n", total);
  return myArray;
}

int main (void) {
  int * ptr = aFunction(8);
  printf("%d\n", ptr[0]);
  return EXIT_SUCCESS;
}
