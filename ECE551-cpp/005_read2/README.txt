 1. Create a file "answer.txt".
 2. Execute the following code by hand 
   (as you learned in Chapter 2), and write
   the output in your "answer.txt" file

int anotherFunction(int a, int b) {
  int answer = 42;
  int x = 0;
  printf("In anotherFunction(%d,%d)\n",a,b);
  while (b > a) {
    printf("a is %d, b is %d\n", a, b);
    answer = answer + (b - a);
    b -= x;
    a += x / 2;
    x++;
  }
  return answer;
}


int someFunction(int x, int y) {
  int a = x + y;
  if (x < y) {
    for (int i = 0; i < x; i++) {
      printf("In the loop with i = %d, a = %d\n", i, a);
      a = a + x;
    }
  }
  else {
    y = anotherFunction(y,a+4);
  }
  return a * y;
}

int main(void) {
  int x = 2;
  int a = someFunction(x,3);
  printf("a = %d\n", a);
  printf("x = %d\n", x);
  int b = someFunction(3,x);
  printf("b = %d\n", b);
  printf("x = %d\n", x);
  return 0;
}


//end of code

3. Submit your answer.txt file for grading


 


