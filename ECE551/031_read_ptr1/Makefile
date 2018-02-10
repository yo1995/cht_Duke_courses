CC= gcc
CFLAGS = -pedantic -std=gnu99 -Wall -Werror
test: test.o
	$(CC) -o test test.o
test.o: test.c
	$(CC) $(CFLAGS) -c test.c