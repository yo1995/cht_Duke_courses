#include <stdio.h>

struct _xyz_t {
  int x;
  int y;
  int * z;
};

int main(){
  struct _xyz_t * x1;
  struct _xyz_t * x2;
  //x1 is initialized in here somewhere
  //..
  x2 = x1;
  printf("%d",sizeof(x2));
	
}
  
