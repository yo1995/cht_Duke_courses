CFLAGS=-Wall -Werror -std=gnu++11 -pedantic -ggdb3
OBJS=dedup.o
PROGRAM=dedup

$(PROGRAM): dedup.cpp
	g++ $(CFLAGS) -o $(PROGRAM) dedup.cpp

clean:
	rm -f $(OBJS) $(PROGRAM) *~
