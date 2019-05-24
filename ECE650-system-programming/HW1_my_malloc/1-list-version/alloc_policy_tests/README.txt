This set of 3 programs are intended for performance study of the 
three malloc allocation policies: first fit, best fit, worst fit.

When executed, each program will produce the following output to
summarize performance along 2 dimensions: 1) Execution time of 
the program (to perform a large series of malloc and free ops) 
and 2) Fragmentation, which is the amount of free space divided
by the total dynamic memory size (i.e. the aggregate amount of
memory that has been sbrk'ed).

Execution Time = XX.XX seconds
Fragmentation  = 0.XXXX

To compile these programs, you may work with the provided Makefile.
There are two variables that you will need to edit:

1) WDIR should point to the directory with your my_malloc.* code
and compiled library (libmymalloc.so).

2) MALLOC_VERSION can be changed to alter which of the *_malloc
and *_free functions are called by the test programs.  The valid
values are:
       "FF" - use first fit
       "BF" - use best fit

By running these 3 programs across your 2 allocation policy 
implementations, you will be able to study performance for the
purposes of your assignment writeup for this part. Try to think 
about whether the performance metrics you measure make sense
in relation to the characteristic of the test program.

The following describes the malloc & free patterns of the 3 tests:

1) equal_size_allocs
This program uses the same number of bytes (128) in all of its
malloc calls. The program streams through memory by malloc'ing
128B chunks, and then later free'ing the chunks (in the same order
as they were malloc'ed).

2) small_range_rand_allocs
This program works with allocations of random size, ranging from
128 - 512 bytes (in 32B increments). The program first malloc's
a large number of these random regions. Then the program alternates
free'ing a random selection of 50 of these allocated regions, and 
malloc'ing 50 more regions with a random size from 128 - 512 bytes.

3) large_range_rand_allocs
This program works with allocations of random size, ranging from
32 - 64K bytes (in 32B increments). The program first malloc's
a large number of these random regions. Then the program alternates
free'ing a random selection of 50 of these allocated regions, and 
malloc'ing 50 more regions with a random size from 32 - 64K bytes.

Note that at the top of each test case .c file, you will see a 
#define NUM_ITERS variable. If needed, you may adjust this variable
to make the timed program run longer (if it runs too short and you 
are seeing execution time variability), or shorter (if the speed
of your implementation is such that the execution takes so long 
that it is a burden.  I would expect the default values should 
result in runs of no more than a minute or two.
