CC= gcc
CFLAGS = -pedantic -std=gnu99 -Wall -Werror 
test-power: power.o test-power.o
	$(CC) -o test-power power.o test-power.o
power.0: power.c
	$(CC) $(CFLAGS) -c power.c
test-power.o :test-power.c
	$(CC) $(CFLAGS) -c test-power.c