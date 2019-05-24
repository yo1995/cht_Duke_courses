This test program can be used as a simple correctness test for the 
malloc implementation (with either of the 2 allocation policies). Note
that this test certainly does not *guarantee* that your code is
completely correct. But it is useful as a basic check. You are free
to use this to construct other test cases for your own checking.

When executed, the program will print the line "Test passed" or
"Test failed" depending on the outcome. The test malloc's 10 memory
regions of different sizes (and free's them after some period of 
time). After each malloc, the program writes some number of 
integer values into the region of memory, and then reads those 
values back and adds them into a running sum. If the resulting
sum is equal to the expected sum at the end of the test, then 
the "Test passed" message is shown.

To compile this program, you may work with the provided Makefile.
There are two variables that you will need to edit:

1) WDIR should point to the directory with your my_malloc.* code
and compiled library (libmymalloc.so).

2) MALLOC_VERSION can be changed to alter which of the *_malloc
and *_free functions are called by the test programs.  The valid
values are:
       "FF" - use first fit
       "BF" - use best fit

