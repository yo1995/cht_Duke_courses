#include <cstdio>
#include <cstdlib>
#include <ostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include "helpers.h"
#include "parsing.h"
#include "function.h"

void destructMap(std::map<std::string, function*> &function_map){
	std::map<std::string, function*>::iterator it = function_map.begin();
	while(it != function_map.end()) {
		// to recycle the new functions in parsing process
		delete (*it).second;
		++it;
	}
}

int main(int argc, char **argv) {
	std::string input;
	std::map<std::string, function*> function_map;
	std::vector<std::string> command_list;
	// add all possible commands into a list and decide what command is in input
	command_list.push_back("define");
	command_list.push_back("evaluate");
	command_list.push_back("numint");
	command_list.push_back("mcint");
	command_list.push_back("max");
	command_list.push_back("min");
	
	/* for here, I believe that we can either use no second parameter as cin input or treat it as an error;
	also, we can take more than one filename parameter by simply iterating through the filenames.
	I'll provide both methods here with comments.*/
	
	// other version
	/*
	while(!getline(cin, input).eof()) {
		parsing(input, function_map, command_list);
    }
	*/
	if(argc < 2){
		std::cout << "Usage: ./numeric filename\n";
		return EXIT_FAILURE;
	}
	else if(argc == 2) {
		std::ifstream input_file;
		input_file.open(argv[1]);
		if(!input_file.good()) {
			std::cerr << "Could not open file! Exit.\n" << std::endl;
			return EXIT_FAILURE;
		}
		std::cout << "=== START READING ===\n" << std::endl;

		while(!std::getline(input_file, input).eof()) {
			// sanitize the white chars in input line for further uses
			delete_spaces_and_tabs_on_head_and_tail(input);
			if(input.empty()) {
				continue;
			}
			parsing(input, function_map, command_list);
			std::cout << "\n=== READING NEXT LINE ===\n" << std::endl;
		}
		input_file.close();
		std::cout << "=== PROCESS COMPLETE! ===" << std::endl;
		destructMap(function_map);
	}
	else {
		std::cerr << "Could only enter one filename! Exit.\n" << std::endl;
		return EXIT_FAILURE;
	}
	
	// other version
	/*
	else {
		int i = 1;
		while(i < argc) {
			std::ifstream input_file;
			input_file.open(argv[i]);
			std::cout << "--- Now reading file "<< argv[i] << "... ---\n" << std::endl;
			if(!input_file.good()) {
				std::cerr << "Could not open file!" << argv[i] << "\n" << std::endl;
			}
			while(!std::getline(input_file, input).eof()) {
				parsing(input, function_map, command_list);
			}
			input_file.close();
			destructMap(function_map);
			i++;
		}
	}
	*/
	return EXIT_SUCCESS;
}