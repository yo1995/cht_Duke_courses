#include <stdio.h>

int main(){
	int a = 3;
	int *b;
	b = & a;
	
	int **c;
	c = & b;
	int ***d;
	d = & c;
	
	printf("%d\n", a);
	printf("%d\n", *b);
	printf("%d\n", *c);
	printf("%d\n", ***d);
}
