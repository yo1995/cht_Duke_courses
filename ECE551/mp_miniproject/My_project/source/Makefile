CFLAGS=-Wall -Werror -std=gnu++11 -pedantic -ggdb3

numerics: numeric.cpp
	g++ -o numerics $(CFLAGS) numeric.cpp parsing.cpp parsing_define.cpp parsing_eval.cpp parsing_gradient.cpp parsing_mcint.cpp parsing_numint.cpp helpers.cpp

clean:
	rm -f *~ numerics
