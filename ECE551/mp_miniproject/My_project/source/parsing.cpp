#define MAXFLAG 1
#define MINFLAG -1
#include "parsing.h"
#include "helpers.h"

/*************************************************
Function: parsing
Description: to wrap-up the parsers for each step
Calls: 
Called By: numeric.cpp/main
Input: 1.read-in file content 2.mapped-function pairs 3.all valid commands.
Return: void
Others: 
*************************************************/

std::string check_command(std::string &input, const std::vector<std::string> &command_list) {
	unsigned i = 0;
	std::string::iterator it = input.begin();
	std::string::iterator it_first = it;
	while(i < command_list.size()) {
		if(input.find(command_list[i]) != std::string::npos) {
			// to check if there is something like "abcdefinexyz and eliminate it"
			std::string temp_define_string = get_next_valid_str(it);
			// remove spaces
			delete_spaces_and_tabs_on_head_and_tail(temp_define_string);
			if(temp_define_string.compare(command_list[i]) != 0) {
				std::stringstream err_msg1;
				err_msg1 << "Error: In input: >> " << input << " << have strange " << command_list[i] << "!";
				throw err_msg1.str();
			}
			std::cout << "Try to define input: >> " << input << " <<." << std::endl;
			input.erase(it_first, it);
			return command_list[i];
		}
		i++;
	}
	if(i == command_list.size()) {
		std::stringstream err_msg2;
		err_msg2 << "Error: In input: >> " << input << " << command not found!" << std::endl;
		throw err_msg2.str();
	}
	return NULL;
}

void parsing(const std::string &input, std::map<std::string, function*> &function_map, const std::vector<std::string> &command_list) {
	std::string temp_compare;
	std::string temp_input;
	temp_input = input;
	try {
		temp_compare = check_command(temp_input, command_list);
	}
	catch(std::string err_msg) {
         std::cerr << err_msg << std::endl;
         return;
    }
	std::cout << "Command is: >> " << temp_compare << " <<." << std::endl;
	if(!temp_compare.compare(0, 6, "define")) {
		parsing_define(temp_input, function_map);
	}
	else if(!temp_compare.compare(0, 8, "evaluate")) {
		parsing_eval(temp_input, function_map);
	}
	else if(!temp_compare.compare(0, 6, "numint")) {
		parsing_numint(temp_input, function_map);
	}
	else if(!temp_compare.compare(0, 5, "mcint")) {
		parsing_mcint(temp_input, function_map);
	}
	else if(!temp_compare.compare(0, 3, "max")) {
		parsing_gradient(temp_input, function_map, MAXFLAG);
	}
	else if(!temp_compare.compare(0, 3, "min")) {
		parsing_gradient(temp_input, function_map, MINFLAG);
	}
	else {
		std::cerr << "Unknown command: can only take in 6 pre-defined types of commands." << std::endl;
	}
}