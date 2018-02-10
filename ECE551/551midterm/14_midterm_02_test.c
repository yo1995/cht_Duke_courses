#include <stdio.h>
#include <stdlib.h>
int main (void) {
int a = 3;
int b = 4;
int data[3] = {6, 8, 12};
int * ptrs[3] = {data, &a, &b};
ptrs[0]++;
*ptrs[1] += *ptrs[2] ;
ptrs[0][1]--;
int i = 0;
for (i = 0; i < 3; i++) {
printf("*ptrs[%d] = %d\n", i, *ptrs[i]);
}
while(data[1] < data[2]) {
for (i = 0; i < 3; i++) {
*ptrs[i] += (i+2);
}
}
printf("data[1] = %d\n", data[1]);
printf("a = %d\n", a);
printf("b = %d\n", b);
return EXIT_SUCCESS;
}

