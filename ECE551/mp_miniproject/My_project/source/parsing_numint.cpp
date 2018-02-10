#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <sstream>
#include "parsing_numint.h"
#include "helpers.h"
#include "function.h"

/*****************************************************************************
Copyright: 2017, Ting
File name: parsing_numint.cpp
Description: Parse and calculate the numerical integral of a certain function
Author: Ting
Version: 1.0
Date: 11.30
History: 11.30 - 1,
*****************************************************************************/
// each dimension can be divided into how many intervals
double division_of_dimension(std::pair<double, double> &range, double step) {
	double temp;
	temp = range.second - range.first;
	// get rid of the NAN problem
	if (temp < step) {
		std::stringstream err_msg1;
		err_msg1 << "Error: Step size too large! Some dimensions cannot be divided.";
		throw err_msg1.str();
	}
	return temp / step;
}

double numint(function *f, std::vector<std::pair<double, double> > &range_list, double step) {
	std::vector<double> divisions_of_each_dimension;
	std::vector<double> midpoints_of_each_dimension;
	std::vector<double> temp;
	double amount_of_midpoints = 1;
	double temp_value = 0;
	unsigned i = 0;
	unsigned size_of_range_list = range_list.size();
	double result = 0;
	while(i < size_of_range_list) {
		try {
			temp_value = division_of_dimension(range_list[i], step);
		}
		catch(std::string err_msg) {
			throw err_msg;
		}
		amount_of_midpoints *= temp_value;
		divisions_of_each_dimension.push_back(amount_of_midpoints);
		i++;
	}
	if(amount_of_midpoints > pow(10, 9)) {
		std::cout << "Notice: Wow! The integral has huge range. Are you sure about to proceed?" << std::endl;
		if(!choice()) {
			std::stringstream err_msg1;
			err_msg1 << "Warning: The integral is too heavy to handle!";
			throw err_msg1.str();
		}
	}
	// to find the first midpoint for each dimension, which we start from and iterate along
	i = 0;
	while(i < size_of_range_list) {
		midpoints_of_each_dimension.push_back(range_list[i].first + step / 2);
		i++;
	}
	i = 0;
	unsigned j = 0;
	amount_of_midpoints = ceil(amount_of_midpoints);
	temp = midpoints_of_each_dimension;
	// this is really of low efficiency, but I'm confused to find better ways
	// we need to approach the midpoint on n-D area from n-1 directions. so it is (n-1)! 
	while(i <  (unsigned)amount_of_midpoints) {
		j = 0;
		while(j < size_of_range_list) {
			if(!(i % (unsigned)(amount_of_midpoints / divisions_of_each_dimension[j]))) {
				midpoints_of_each_dimension[j] += step;
				if(midpoints_of_each_dimension[j] > range_list[j].second){
					midpoints_of_each_dimension[j] = temp[j];
				}
			}
			j++;
		}
		f -> setAllValue(midpoints_of_each_dimension);
		result += f -> evaluate();
		i++;
	}
	/* the final result equals to the n-D area of variables times each height of prism and sum them up,
	which also equals to the sum-up of times.*/
	return pow(step, size_of_range_list) * result;
}

// wrapper
void parsing_numint(std::string input, std::map<std::string, function*> &function_map) {
	// numint f 0.001 -1 1 4 5
	double step;
	double result;
	std::vector<std::pair<double, double> > range_list;
	std::string::iterator it = input.begin();
	std::string fname = get_next_valid_str(it);
	if(!is_function(fname, function_map)) {
		std::cerr << "Error: Function "<< fname << " definition is not found!" << std::endl;
		return;
	}
	step = my_strtod(it);
	if(std::isnan(step)) {
		std::cerr << "Error: Step number invalid, will abort parsing!" << std::endl;
		return;
	}
    try {
		// xmin xmax ymin ymax
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
	try {
		result = numint((*map_it).second, range_list, step);
	}
	catch(std::string err_msg) {
         std::cerr << err_msg << std::endl;
         return;
    }
	std::cout << "The result of integral " << fname << " = " << result << "." << std::endl;
}
