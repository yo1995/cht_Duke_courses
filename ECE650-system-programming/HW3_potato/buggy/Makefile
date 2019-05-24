CC=gcc
CFLAGS=-O3 -fPIC -ggdb3 -Wall -Werror -pedantic

all: ringmaster player

ringmaster: ringmaster.o utils.o
	$(CC) $(CFLAGS) -o $@ ringmaster.o utils.o

player: player.o utils.o
	$(CC) $(CFLAGS) -o $@ player.o utils.o

ringmaster.o:	ringmaster.c potato.h

player.o:		player.c potato.h

utils.o:		utils.c potato.h

.PHONY: clean clobber

clean:
	rm -f *.o ringmaster player *~

clobber:
	rm -f *~ *.o
