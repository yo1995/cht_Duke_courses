CFLAGS=-Wall -Werror -std=c++11 -pedantic
OBJS=code.o
PROGRAM=code

$(PROGRAM): $(OBJS)
	g++ $(CFLAGS) -o $@ $(OBJS)

%.o: code.c code.h
	g++ -c $(CFLAGS) $<

clean:
	rm -f $(OBJS) $(PROGRAM) *~