#include <stdio.h>

int main(){
 int data[] = {2,5,7,9};
 int * p = &data[1];
 p++;
 printf("%d", *p);
}
