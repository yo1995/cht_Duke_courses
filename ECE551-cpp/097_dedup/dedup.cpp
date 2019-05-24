#include<dirent.h>
#include<stdio.h>
#include<iostream>
#include<fstream>
#include<cstdlib>
#include<vector>
#include<string>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<functional>
#include<unordered_map>

// to calculate the hash value for each file, and store them in a unsorted map
size_t hash_helper(std::ifstream &input, std::string filepath) {
	std::string line;
	size_t hashval;
	std::string input_data;
	std::hash<std::string> str_hash;
	while(!input.eof()) {
		getline(input, line);
		input_data.append(line);
	}
	hashval = str_hash(input_data);
	return hashval;
}

// to compare the hash value of corresponding files, insert the pair and output the script
void file_compare_delete_script_helper(std::pair<size_t, std::string> curr_pair, std::unordered_map<size_t, std::string> &myhashmap) {
	// to find the corresponding hash Key in the unordered_map
	std::unordered_map<size_t, std::string>::iterator it = myhashmap.find(curr_pair.first); // also get<0>(curr_pair)
	if (it == myhashmap.end()) {
		myhashmap.insert(curr_pair);
	}
	else {
		std::cout<< "#Removing " << curr_pair.second << " (duplicate of " << it -> second << ")." << std::endl;
		std::cout<< "rm " << curr_pair.second << std::endl;
	}
}

// to recursively walk through the whole directory without . and ..
void dir_recursor(const std::string &dir, std::unordered_map<size_t, std::string> &myhashmap) {
	DIR* curr_dir;
	struct dirent* ent;
	// since we use the C-lib dirent, we need to obey the string fashion in C format. Ref AoP Ch.16
	if ((curr_dir = opendir(dir.c_str())) == 0) { // if I new a string myself and c_str it, it returns a pointer which could cause dangling pointer problem. be careful!
		perror("Could not open directory!");
		closedir(curr_dir);
		exit(EXIT_FAILURE);
	}
	else {
		while ((ent = readdir(curr_dir)) != NULL) {
			std::string file_name(ent -> d_name);
			if (file_name == "." || file_name == "..") {
				continue;
			}
			else if (ent -> d_type == DT_DIR) {
				std::string next_dir = dir + "/" + file_name;
				dir_recursor(next_dir, myhashmap);
			}
			else if (ent -> d_type == DT_REG) {
				std::string file = dir + "/" + file_name;
				std::ifstream input(file.c_str());
				if (input.good()) {
					size_t curr_hash;
					curr_hash = hash_helper(input, file);
					std::pair<size_t, std::string> curr_pair(curr_hash, file);
					file_compare_delete_script_helper(curr_pair, myhashmap);
				}
				else {
					std::cout << "Can not open the file!" << std::endl;
					exit(EXIT_FAILURE);
				}
				
			}
			else {
				std::cerr << "Unknown file type. " << file_name << " will be omitted." <<std::endl;
			}
		}
	}
	closedir(curr_dir);
}

// to end the drought
int main(int argc, char **argv) {
	if (argc < 2) {
		perror("Usage: ./dir (the directory to be dedup-ed). Please re-run the program!");	
		return EXIT_FAILURE;
	}
	else {
		std::unordered_map<size_t, std::string> my_hash_map;
		int i = 1;
		std::cout << "#!/bin/bash" << std::endl;
		while (i < argc) {
			const std::string dir = argv[i];
			dir_recursor(dir, my_hash_map);
			i++;
		}
	}
	return EXIT_SUCCESS;
}
