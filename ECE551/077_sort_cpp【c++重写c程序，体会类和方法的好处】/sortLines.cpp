#include <cstdio>
#include <string>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>


int main(int argc, char *argv[]) {
	std::ifstream input;
	std::string line;
	//declare a vertor of string to store the lines, so much easier than c!
	std::vector<std::string> data;
	size_t i = 0;
	size_t n_of_argc = 1;
	//first, with 0 arguments, read from cin
	if(argc == 1){
		//getline in cpp version
		while(!std::cin.eof()) {
			getline(std::cin, line);
			data.push_back(line);
		}
		//data.pop_back();
		//sort the data using algorithm
		sort(data.begin(), data.end());
		//print out the data to cout
		while(i < data.size()) {
			std::cout << data[i] << std::endl;
			i++;
		}
	}
	//second, with more than 0 arguments, loop through the parameters
	else{
		while(n_of_argc < argc) {
			input.open(argv[n_of_argc]);
			if(input.fail()) {
				perror("Could not open file");
				return EXIT_FAILURE;
			}
			//getline for files
			while(!input.eof()){
				getline(input, line);
				data.push_back(line);
			}
			//data.pop_back();
			//sort the data using algorithm
			sort(data.begin(), data.end());
			i = 0;
			while(i < data.size()){
				std::cout << data[i] << std::endl;
				i++;
			}
			
			data.clear();
			//closing the file
			input.close();
			//seems no need to check close status.
			n_of_argc++;
		}
	}
	
	return EXIT_SUCCESS;
}