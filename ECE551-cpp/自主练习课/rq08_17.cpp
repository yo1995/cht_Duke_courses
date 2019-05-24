#include <stdio.h>

int main(){
	double d = 3.14159;
	printf("%d\n", &d);
    double * p;
	p = (double*) &d;
    *p = 42;
    printf("%f\n", d);
    printf("%d\n", &d);
    printf("%d\n", p);
}
    
