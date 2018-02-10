#ifndef __PARSING_MCINT_H__
#define __PARSING_MCINT_H__

#include <cstdlib>
#include <vector>
#include <string>
#include <map>
#include "function.h"

double max_minus_min(std::vector<std::pair<double, double> > &range_list);
double mcint(function *f, std::vector<std::pair<double, double> > &range_list, double trials);
void parsing_mcint(std::string input, std::map<std::string, function*> &function_map);

#endif
