#include <stdio.h>

int main(void) {
	int x = 5;
	double d = 2.4;
	char c = 'a';
	
	printf("%d\n" , x - 2);
	printf("%f\n" , x + d);
	printf("%d\n" , x / 2);
	printf("%f\n" , d + x / 3);
	printf("%d\n" , c + x);
	printf("%d\n" , x + 0xFFFFFFFF);
	printf("%d\n" , x + 0x7FFFFFFF);
	printf("%f\n" , d + c);
	return 0;
}
