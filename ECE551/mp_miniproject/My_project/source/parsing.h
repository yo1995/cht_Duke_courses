#ifndef __PARSING_H__
#define __PARSING_H__

#include <string>
#include <map>
#include "function.h"
#include "helpers.h"
#include "parsing_define.h"
#include "parsing_eval.h"
#include "parsing_numint.h"
#include "parsing_mcint.h"
#include "parsing_gradient.h"

/*****************************************************************************
Copyright: 2017, Ting
File name: parsing.h
Description: The wrapper parsing header to invoke each step's parser respectively.
Author: Ting
Version: 1.0
Date: 11.26
History: 11.26 - 1.0,
*****************************************************************************/

std::string check_command(std::string &input, const std::vector<std::string> &command_list);
void parsing(const std::string &input, std::map<std::string, function*> &function_map, const std::vector<std::string> &command_list);

#endif
