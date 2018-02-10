#ifndef __PARSING_DEFINE_H__
#define __PARSING_DEFINE_H__

#include <cstdlib>
#include <vector>
#include <string>
#include <map>
#include "function.h"

std::string seperate_id(std::string input);
std::string seperate_expression(std::string &input);
void get_variables_and_enlist(std::string::iterator &it, std::vector<var_pair> &var_pair_temp);
Expression* makeExpr(std::string &op, Expression* lhs, Expression* rhs);
Expression* makeFunc(std::string &fname, Expression* var);
Expression* parse_operator(std::string::iterator &it, std::map<std::string, function*> &function_map, function *f);
Expression* parse_expression(std::string::iterator &it, std::map<std::string, function*> &function_map, function* f);
void parsing_define(std::string input, std::map<std::string, function*> &function_map);

#endif
