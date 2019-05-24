#include <random> // in c++11, change makefile cflags then
#include "parsing_mcint.h"
#include "helpers.h"

/*****************************************************************************
Copyright: 2017, Ting
File name: parsing_mcint.cpp
Description: Parse and calculate the Monte Carlo integral of certain function
Author: Ting
Version: 1.0
Date: 11.30
History: 11.30
*****************************************************************************/

// according to the equation provided in numerics.txt, need to do this
// calculate every length of bounds
double max_minus_min(std::vector<std::pair<double, double> > &range_list) {
	double temp;
	double result = 1;
	unsigned i = 0;
	while(i < range_list.size()) {
		temp = range_list[i].second - range_list[i].first;
		result *= temp;
		i++;
	}
	return result;
}

//!! Do not forget seed!! otherwise the rng will not change during invoke.
std::vector<double> uniform_rv_vector_generator(std::vector<std::pair<double, double> > &range_list) {
	std::default_random_engine generator(std::random_device{}());
	std::vector<double> random_vector;
	unsigned i = 0;
	double temp;
	while(i < range_list.size()) {
		std::uniform_real_distribution<double> uni_dist(range_list[i].first, range_list[i].second);
		temp = uni_dist(generator);
		random_vector.push_back(temp);
		i++;
	}
	return random_vector;
}

//!!!CITE partially from http://www.cplusplus.com/reference/random/uniform_real_distribution/
double mcint(function *f, std::vector<std::pair<double, double> > &range_list, double trials) {
	double result;
	double max_min;
	double fsum = 0;
	std::vector<double> uni_dist;
	unsigned i = 0;
	while(i < trials) {
		// a uniform r.v. in the range of each variable, repeat, say 500 times.
		uni_dist = uniform_rv_vector_generator(range_list);
		f -> setAllValue(uni_dist);
		fsum += f -> evaluate();
		i++;
	}
	max_min = max_minus_min(range_list);
	result = (1/trials) * max_min * fsum;
	return result;
}

// wrapper
void parsing_mcint(std::string input, std::map<std::string, function*> &function_map) {
	double trials;
	double result;
	std::vector<std::pair<double, double> > range_list;
	std::string::iterator it = input.begin();
	std::string fname = get_next_valid_str(it);
	if(!is_function(fname, function_map)) {
		std::cerr << "Error: Function "<< fname << " definition is not found!" << std::endl;
		return;
	}
	trials = my_strtod(it);
	if(trials < 1 || std::isnan(trials)) {
		std::cerr << "Error: Trials number invalid, will abort parsing!" << std::endl;
		return;
	}
	try {
		get_integral_bounds(it, range_list);
	}
	catch(std::string err_msg) {
         std::cerr << err_msg << std::endl;
         return;
    }
	function_map::iterator map_it = function_map.find(fname);
	if(range_list.size() != (*map_it).second -> varSize()) {
		std::cerr << "Error: Wrong usage(bounds amount) of function "<< fname << ", will abort parsing!" << std::endl;
		return;
	}
	result = mcint((*map_it).second, range_list, trials);
	/* catch(std::string err_msg) {
         std::cerr << err_msg << std::endl;
         return;
    } */
	std::cout << "The result of Monte Carlo integral " << fname << " = " << result << "." << std::endl;
}