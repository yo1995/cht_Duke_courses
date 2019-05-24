#ifndef __PARSING_EVAL_H__
#define __PARSING_EVAL_H__

#include "function.h"

std::string seperate_evaluation(std::string &input);
double evaluate(std::string::iterator &it, std::map<std::string, function*> &function_map, function* f, std::string fname);
double parse_eval_funtion(std::string::iterator &it, std::map<std::string, function*> &function_map);
void parsing_eval(std::string input, std::map<std::string, function*> &function_map);

#endif
