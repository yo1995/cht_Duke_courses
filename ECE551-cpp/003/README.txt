This is your second "reading quiz."  As with other reading quizzes,
you should read Chapter 2, watch the videos in it, and make sure you understand
the material in it (which may require re-reading all or part of it,
asking questions, etc), BEFORE attempting this quiz.  This quiz will check
your basic understanding of the material before you move on to other,
more in-depth exercises.

As always, place your answers in "answers.txt" (one per line), and
submit as usual.

1. What does the term "semantics" mean in the context of programming? B

A. The rules for constructing "grammatically correct" programs.
B. The rules describing the meaning of syntactically correct code.
C. The rules for fixing problems with broken code.
D. The rules describing how to divide code into functions.

2. Consider the following piece of code: 

int f(int x, int y) {
   temp = 3 *x + y *y;
   return (temp - x) / 2;
}

Which of the following best describes the syntax error in this piece
of code?  A

A. It makes use of an undeclared variable.
B. It makes incorrect use of parenthesis.
C. (temp - x) / 2 is not a valid expression.
D. The parameters to f are declared improperly.

3.  What is an "lvalue"?  D

A. A value that can be placed into a box.
B. A value that is elevated through the evaluation of an expression.
C. A value that can be the left operand of an operator.
D. None of the above
左值应该是命名一个box的，而非上述所有功能。

4. What symbol ends statements in C? D

A. A period
B. A comma
C. A colon
D. A semicolon

5. What does "scope" mean? B

A. The types of values that a variable can hold.
B. The region of code in which a variable is visible.
C. The kinds of problem that a function can solve.
D. The amount of domain knowledge required to write a particular program.

6. What does the "%d" format specifier mean for printf? A

A. Print an integer in base 10
B. Print arbitrary data in its native format
C. Print the current date
D. Print a string

7.  What is the difference between printing a value and returning
    a value? B

A. Printing a value leaves the current function, while returning
   a value does not.
B. Printing a value gives it to the user, while returning a value
   gives it to other code for further computation.
C. Printing a value only works on strings, while returning a value
   only works on integers.
D. None of the above.

8. What are the semantics of "break"? C

A. It causes the program to crash.
B. It exits the current function, destroying its frame.
C. It moves the execution arrow just outside of the current loop or switch.
D. None of the above.


9. If C did not have the keyword "for" but you wanted to write
   something where a for-loop were the natural choice, what could
   you use instead? B

A. if
B. while
C. return
D. break

10. Suppose you have the line of code

  int a = f(x,y);

Which of the following best describes how you determine what
value to put in the box for "a"? D

A. (1) Look at f to see what mathematical function it is.
   (2) Work out the math for x and y.
   (3) Your answer goes in the box for "a".
B. (1) Create a frame for f, copying the values of x and y
       into the boxes named for its parameters.
   (2) Move the execution arrow into f, and execute code line by line.
   (3) When your execution arrow reaches a statement of the form
        printf("%d", num)
       the value that it prints is what goes into the box for "a".
C. (1) Look at f to see where it has a statement of the form
          a = expression
   (2) Figure out what value that expression has
   (3) The value you came up with is what goes in the box for "a".
D. (1) Create a frame for f, copying the values of x and y
       into the boxes named for its parameters.
   (2) Move the execution arrow into f, and execute code line by line.
   (3) When your execution arrow reaches a statement of the form
   return expression;
       the value of that expression is what you will end up
       putting in the box for "a" (after you destroy the frame,
       and return the execution arrow to the call site).

