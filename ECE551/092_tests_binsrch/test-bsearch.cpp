#include <cstdio>
#include <cstdlib>
#include <math.h>
#include "function.h"

// this one returns a result of largest x
int binarySearchForZero(Function<int,int> *f, int low, int high);

template<typename T>
class SinFunction : public Function<int, int> {
	public:
		virtual int invoke(int arg){
			return 10000000 * (sin(arg/100000.0) - 0.5);
		}
};

template<typename T>
class LinearFunction : public Function<int, int> {
	public:
		virtual int invoke(int arg){
			//this is the easiest way to do it.
			return arg;
		}
};

class CountedIntFn : public Function<int,int>{
	protected:
		unsigned remaining;
		Function<int,int> * f;
		const char * mesg;
	public:
		CountedIntFn(unsigned n, Function<int,int> * fn, const char * m): remaining(n), f(fn), mesg(m) {}
		// invoke n times 
		virtual int invoke(int arg) {
			if (remaining == 0) {
				fprintf(stderr,"Too many function invocations in %s\n", mesg);
				exit(EXIT_FAILURE);
			}
			remaining--;
			return f->invoke(arg);
		}
};

void check(Function<int,int> * f, int low, int high, int expected_ans, const char * mesg) {
	// check remaining should not be greater than the expected_ans
	int invoke_times = high > low ? log(high - low) / log(2) + 1 : 1;
	CountedIntFn *f2 = new CountedIntFn(invoke_times, f, mesg);
	int answer = binarySearchForZero(f2, low, high);
	
	if (answer != expected_ans) {
		fprintf(stderr, "%s", mesg);
		exit(EXIT_FAILURE);
	}
}


int main() {
	// think about what to pass into the check function
	SinFunction<int> * sf = new SinFunction<int>();
	LinearFunction<int> * lf = new LinearFunction<int>();
	check(sf, 0, 150000, 52359, "0 - sin test case. 0, 150000, 52359\n");
	check(lf, -1, 1, 0, "1 test case. 0, 8, 3\n");
	check(lf, 0, 8, 0, "2 test case. 0, 8, 3\n");
	check(lf, 3, 8, 3, "3 test case. 3, 8, 3\n");
	check(lf, 3, 3, 3, "4 test case. 3, 3, 3\n");
	check(lf, 2, 3, 2, "5 test case. 2, 3, 3\n");
	check(lf, 3, 2, 3, "6 test case. 3, 2, 3\n");
	check(lf, 0, 0, 0, "7 test case. 0, 0, 0\n");
	check(lf, -4, -3, -4, "8 test case. 0, 0, 0\n");
	check(lf, -99999999, -1, -2, "9 test case. 0, 0, 0\n");
	check(lf, -1, 10000000, 0, "10 test case. 0, 0, 0\n");
	check(lf, 1, 10000000, 1, "11 test case. 0, 0, 0\n");
	check(lf, -99999999, 10000000, 0, "12 test case. 0, 0, 0\n");
	check(lf, -1, -1, -1, "13 test case. 0, 0, 0\n");
}