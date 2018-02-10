#include <cmath>
#include <algorithm>    // std::count, CITE from cpp/ref
#include "helpers.h"

void skip_spaces(std::string::iterator &it) {
	while(*it == ' ' || *it == '\t') {
		++it;
	}
}

void delete_spaces(std::string &str, std::string::iterator &it) {
	while(*it == ' ' || *it == '\t') {
		str.erase(it);
	}
}

void delete_spaces_and_tabs_on_head_and_tail(std::string &str) {
	if(str.empty()) {
		return;
	}
	str.erase(0, str.find_first_not_of(" \t\n\r"));
	str.erase(str.find_last_not_of(" \t\n\r") + 1);
}

bool parenthes_mismatch(std::string input) {
	int lnum = std::count (input.begin(), input.end(), '(');
	int rnum = std::count (input.begin(), input.end(), ')');
	if(lnum == rnum) {
		return true;
	}
	else {
		return false;
	}
}

std::string get_next_valid_str(std::string::iterator &it) {
	skip_spaces(it);
	std::string str;
	str.clear();
	while(*it != ' ' && *it != '\t') {
		if((*it == '(') || (*it == '\0') || *it == ')') {
			return str;
		}
		str += *it;
		++it;
	}
	return str;
}

double my_strtod(std::string::iterator &it) {
	std::string value_string;
	value_string = get_next_valid_str(it);
	char* end;
	double value;
	value = strtod(value_string.c_str(), &end);
	// std::cout << "value string: " << value_string << std::endl;
	if(*end == ' ' || *end == '\0' || *end == '\t') {
		return value;
	}
	else{
		// defined in <cmath>, included in function.h
		return NAN;
	}
}

bool is_valid_operator(std::string &op) {
	std::string valid_operators("+-*/% sin cos sqrt log pow");
	std::size_t found = valid_operators.find(op);
	return (found != std::string::npos);
}

bool is_function(std::string &str, std::map<std::string, function*> &function_map) {
	return (function_map.find(str) != function_map.end());
}

void get_integral_bounds(std::string::iterator &it, std::vector<std::pair<double, double> > &range_list) {
	std::cout << "Bounds will be parsed in a fashion of min max pairs." << std::endl;
	while(*it != '\0') {
		double lower;
		double upper;
		lower = my_strtod(it);
		upper = my_strtod(it);
		if(std::isnan(lower) || std::isnan(upper) || lower >= upper || *(it-1) == '\0') {
			std::string err_msg1 = "Error: Invalid input for bounds! Check order, validity and amount of args.";
			throw err_msg1;
		}
		range_list.push_back(std::make_pair(lower, upper));
   }
}

bool choice() {
	char ch;
	std::cout << "Please choose whether to continue? (Y/N)" << std::endl;
	std::cin >> ch;
	if(ch == 'Y' || ch == 'y') {
		return true;
	}
	else if(ch == 'N' || ch == 'n') {
		return false;
	}
	else {
		std::cout << "Invalid char. Please choose again.";
		return choice();
	}
}
