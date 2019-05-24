#ifndef __PARSING_NUMINT_H__
#define __PARSING_NUMINT_H__

#include <cstdlib>
#include <vector>
#include <string>
#include <map>
#include "function.h"

double division_of_dimension(std::pair<double, double> &range, double step);
double numint(function *f, std::vector<double, double> &range_list, double step);
void parsing_numint(std::string input, std::map<std::string, function*> &function_map);

#endif
