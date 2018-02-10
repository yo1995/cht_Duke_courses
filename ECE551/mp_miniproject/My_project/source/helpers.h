#ifndef __HELPERS_H__
#define __HELPERS_H__

#include <cmath>
#include "function.h"

typedef std::pair<std::string, double> var_pair;

void skip_spaces(std::string::iterator &it);
void delete_spaces(std::string &str, std::string::iterator &it);
void delete_spaces_and_tabs_on_head_and_tail(std::string &str);
bool parenthes_mismatch(std::string input);
std::string get_next_valid_str(std::string::iterator &it);
double my_strtod(std::string::iterator &it);
bool is_valid_operator(std::string &op);
bool is_function(std::string &str, std::map<std::string, function*> &function_map);
void get_integral_bounds(std::string::iterator &it, std::vector<std::pair<double, double> > &range_list);
bool choice();

#endif
