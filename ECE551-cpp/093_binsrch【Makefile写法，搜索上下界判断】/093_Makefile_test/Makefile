test-bsearch: search.o test-bsearch.o 
	g++ -o test-bsearch -ggdb3 -Wall -Werror -pedantic -std=gnu++98 search.o test-bsearch.o
	
search.o: search.cpp
	g++ -c -ggdb3 -Wall -Werror -pedantic -std=gnu++98 search.cpp

test-bsearch.o: test-bsearch.cpp
	g++ -c -ggdb3 -Wall -Werror -pedantic -std=gnu++98 test-bsearch.cpp

.PHONY: clean
clean:
	rm -f test-bsearch *.o *.c~ *.h~