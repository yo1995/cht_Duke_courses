CC=gcc
CFLAGS=-O3 -fPIC
MALLOC_VERSION=FF
WDIR=..

all: equal_size_allocs small_range_rand_allocs large_range_rand_allocs

equal_size_allocs: equal_size_allocs.c
	$(CC) $(CFLAGS) -I$(WDIR) -L$(WDIR) -D$(MALLOC_VERSION) -Wl,-rpath=$(WDIR) -o $@ equal_size_allocs.c -lmymalloc -lrt

small_range_rand_allocs: small_range_rand_allocs.c
	$(CC) $(CFLAGS) -I$(WDIR) -L$(WDIR) -D$(MALLOC_VERSION) -Wl,-rpath=$(WDIR) -o $@ small_range_rand_allocs.c -lmymalloc -lrt

large_range_rand_allocs: large_range_rand_allocs.c
	$(CC) $(CFLAGS) -I$(WDIR) -L$(WDIR) -D$(MALLOC_VERSION) -Wl,-rpath=$(WDIR) -o $@ large_range_rand_allocs.c -lmymalloc -lrt

clean:
	rm -f *~ *.o equal_size_allocs small_range_rand_allocs large_range_rand_allocs

clobber:
	rm -f *~ *.o
