#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include <map>
#include "parsing_eval.h"
#include "helpers.h"

/*****************************************************************************
Copyright: 2017, Ting
File name: parsing_eval.cpp
Description: Parse evaluate command only. Call the functions stored in the map and evaluate them, to yield function value given variable values.
Author: Ting
Version: 1.0
Date: 11.29
History: 11.29 - 1,
*****************************************************************************/

// similar to parsing_define, left hand side parsing. Thought about combining those two but do not think necessary
std::string seperate_evaluation(std::string &input) {
    std::size_t first_id = input.find_first_of('(', 0);
    std::size_t last_id = input.find_first_of(')', 0);
	if((first_id == std::string::npos) || (last_id == std::string::npos)) {
		std::string err_msg1 = "Error: Invalid format in evaluation!";
		throw err_msg1;
	}
    std::size_t length = last_id - first_id + 1;
    std::string id_list = input.substr (first_id, length);
	delete_spaces_and_tabs_on_head_and_tail(id_list);
	if(id_list.empty()){
		std::string err_msg2 = "Error: Nothing is detected in evaluation!";
		throw err_msg2;
	}
    return id_list;
}

// the important function that calls the f -> evaluate to really get the numerical answer
double evaluate(std::string::iterator &it, std::map<std::string, function*> &function_map, function* f, std::string fname) {
	std::vector<double> result_list;
	double result;
	while(*it != ')') {
		skip_spaces(it);
		if(*it == '(') {
			// e.g. (f (g 1 2) (g 3 4) (h 5 6 -7))
			// sort of recursive call
			result = parse_eval_funtion(it, function_map);
			++it;
			if(std::isnan(result)) {
				return NAN;
			}
			else {
				result_list.push_back(result);
			}
		}
		else if(isdigit(*it) || *it=='+' || *it=='-' || *it=='.') {
			std::string::iterator temp = it;
			double value;
			value = my_strtod(it);
			if(std::isnan(value)) {
				std::cerr<< "Error: number " << value << " in input is not valid double number!" << std::endl;
				return NAN;
			}
			else {
				result_list.push_back(value);
			}
		}
		else {
			std::cerr << "Error: Evaluate command " << fname << " must have an expression with no variables!" << std::endl;
			return NAN;
		}
	}
	if(f -> varSize() != result_list.size()) {
		std::cout << "Error: Wrong usage(argument amount) of function "<< fname << ", will abort evaluating!\n";
		return NAN;
	}
	else {
		// sqrt(-1) and 0/0 goes to this part, design test cases!
		f -> setAllValue(result_list);
		return f -> evaluate();
	}
}

// to see whether the function already exists
// non-exist functions cannot be evaluated!
double parse_eval_funtion(std::string::iterator &it, std::map<std::string, function*> &function_map) {
	double result;
	if(*it == '(') {
		++it;
		std::string fname = get_next_valid_str(it);
		if(is_function(fname, function_map)) {
			std::map<std::string, function*>::iterator map_it = function_map.find(fname);
			// using corresponding function fname to evaluate, function*
			result = evaluate(it, function_map, (*map_it).second, fname);
			return result;
		}
		else{
			std::cout << "Error: Function "<< fname << " definition is not found!" << std::endl;
			return NAN;
		}
	}
	else {
		std::cerr << "Error: Invalid expression! No expression parsed." << std::endl;
		return NAN;
	}
}

// still, a small wrapper function
void parsing_eval(std::string input, std::map<std::string, function*> &function_map) {
	std::string evaluation;
	if(!parenthes_mismatch(input)) {
		std::cerr << "Error: Parenthes amount mismatch!" << std::endl;
		return;
	}
	try {
		evaluation = seperate_evaluation(input);
	}
	catch(std::string err_msg) {
         std::cerr << err_msg << std::endl;
         return;
    }
	std::string::iterator it = evaluation.begin();
	double result;
	result = parse_eval_funtion(it, function_map);
	if(std::isnan(result)){
		//std::cerr << "Error: Result is not a number. Check if 0/0, sqrt(-1), no definition or other faults!" << std::endl;
		std::cerr << "Error: Failed to evaluate!" << evaluation << std::endl;
		return;
	}
	else {
		std::cout << evaluation << " = " << result << std::endl;
	}
}
