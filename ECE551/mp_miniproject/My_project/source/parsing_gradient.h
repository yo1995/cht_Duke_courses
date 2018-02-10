#ifndef __PARSING_GRADIENT_H__
#define __PARSING_GRADIENT_H__

#include <cstdlib>
#include <vector>
#include <string>
#include <map>
#include "function.h"

void get_start_point(std::string::iterator &it, std::vector<double> &start_point, function* f);
std::vector<double> vector_add_minus(std::vector<double> &lhs, std::vector<double> &rhs, int flag);
std::vector<double> vector_scaling(std::vector<double> &v, double gamma);
double directional_diff(function *f, std::vector<double> &v, unsigned i);
double euc_dist(std::vector<double> &v);
double gradient(function *f, double gamma, double convergedDistance, std::vector<double> &start_point, double step_limit, int flag);
void parsing_gradient(std::string input, std::map<std::string, function*> &function_map, int pos_neg_flag);

#endif
