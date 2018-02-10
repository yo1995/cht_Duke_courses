#define MINDIFF 0.001 // give a designer defined step size to calculate diff 
#include "parsing_gradient.h"
#include "helpers.h"

/*****************************************************************************
Copyright: 2017, Ting
File name: parsing_gradient.cpp
Description: Parse and calculate the gradient of certain function. Good way of approximating an extremum
Author: Ting
Version: 1.0
Date: 11.26
History: 11.26 - 1,
*****************************************************************************/

// parse the start point vector in input
void get_start_point(std::string::iterator &it, std::vector<double> &start_point, function* f) {
	double point;
	unsigned least_amount;
	unsigned i = 0;
	least_amount = f -> varSize();
	while(i < least_amount) {
		point = my_strtod(it);
		if(std::isnan(point)) {
			std::stringstream err_msg;
			err_msg << "Error: Invalid start point value: NAN or reach the end!";
			throw err_msg.str();
		}
		start_point.push_back(point);
		i++;
	}
}

// vector linear operation, combined ascent and descent
std::vector<double> vector_add_minus(std::vector<double> &lhs, std::vector<double> &rhs, int flag) {
	std::vector<double> result;
	if(lhs.size() == rhs.size()) {
		double temp;
		unsigned i = 0;
		while(i < lhs.size()) {
			// the combination.
			temp = lhs[i] + flag * rhs[i];
			result.push_back(temp);
			i++;
		}
		return result;
	}
	std::stringstream err_msg;
	err_msg << "Error: Vectors to add/minus should be same dimension. Abort calculation!";
	throw err_msg.str();
}

// vector multiplied by a constant
std::vector<double> vector_scaling(std::vector<double> &v, double gamma) {
	double temp;
	std::vector<double> result;
	unsigned i = 0;
	while(i < v.size()) {
		temp = v[i] * gamma;
		result.push_back(temp);
		i++;
	}
	return result;
}

// directional differential calculation
double directional_diff(function *f, std::vector<double> &v, unsigned i) {
	std::vector<double> v_inc (v);
	v_inc[i] += MINDIFF;
	f -> setAllValue(v_inc);
	double fxh = f -> evaluate();
	f -> setAllValue(v);
	double fxl = f->evaluate();
	return (fxh - fxl) / MINDIFF;
}

// calculate the Euclidian distance between step = gradu.*gamma and zero
double euc_dist(std::vector<double> &v) {
	unsigned i = 0;
	double result = 0;
	while(i < v.size()) {
		result += pow(v[i], 2);
		i++;
	}
	return sqrt(result);
}

// might not fully understand the gradient function. The first iteration was sacrificed.
double gradient(function *f, double gamma, double convergedDistance, std::vector<double> &start_point, double step_limit, int flag) {
	double size_of_start_point = start_point.size();
	std::vector<double> start_copy = start_point;
	double count = 0;
	std::vector<double> pnew (size_of_start_point, 0);
	std::vector<double> gradu (size_of_start_point, 0);
	std::vector<double> step (size_of_start_point, convergedDistance + 1);
	// to calculate the diff for each variable, give the closest point to the saddle point, or give the best guess if not converge.
	try {
		while((euc_dist(step) > convergedDistance) && (count < step_limit)) {
			pnew = vector_add_minus(start_point, step ,flag);
			start_point = pnew;
			unsigned i = 0;
			while(i < size_of_start_point) {
				gradu[i] = directional_diff(f, start_point, i);
				i++;
			}
			step = vector_scaling(gradu, gamma);
			count++;
		}
	}
	catch(std::string err_msg) {
		throw err_msg;
	}
	f -> setAllValue(pnew);
	return f -> evaluate();
}

// wrapper
void parsing_gradient(std::string input, std::map<std::string, function*> &function_map, int pos_neg_flag) {
	double gamma;
	double convergedDistance;
	double step;
	double result;
	std::vector<double> start_point;
	std::string::iterator it = input.begin();
	std::string fname = get_next_valid_str(it);
	std::string remainder_string;
	if(!is_function(fname, function_map)) {
		std::cerr << "Error: Function "<< fname << " definition is not found!" << std::endl;
		return;
	}
	// sequentially parse each item in line according to the rules
	gamma = my_strtod(it);
	convergedDistance = my_strtod(it);
	if(std::isnan(convergedDistance) || std::isnan(gamma)) {
		std::cerr << "Error: gamma or convergedDistance is invalid, will abort parsing!" << std::endl;
		return;
	}
	function_map::iterator map_it = function_map.find(fname);
	try {
		get_start_point(it, start_point, (*map_it).second);
	}
	catch(std::string err_msg) {
         std::cerr << err_msg << std::endl;
         return;
    }
	step = my_strtod(it);
	remainder_string = get_next_valid_str(it);
	delete_spaces_and_tabs_on_head_and_tail(remainder_string);
	if(!remainder_string.empty()) {
		std::cerr << "Error: Input not good, will abort parsing!" << std::endl;
		return;
	}
	try {
		result = gradient((*map_it).second, gamma, convergedDistance, start_point, step, pos_neg_flag);
	}
	catch(std::string err_msg) {
         std::cerr << err_msg << std::endl;
         return;
    }
	std::cout << "The result of Gradient " << fname << " = " << result << "." << std::endl;
}
