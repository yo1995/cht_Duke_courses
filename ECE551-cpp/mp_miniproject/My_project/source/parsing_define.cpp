#include <cmath>
#include "parsing_define.h"
#include "helpers.h"

/*****************************************************************************
Copyright: 2017, Ting
File name: parsing_define.cpp
Description: Parse define command only. Store the expressions into the function_map
Author: Ting
Version: 1.0
Date: 11.29
History: 11.29 - 1,
*****************************************************************************/

// to seperate ids from left hand side(id id id) of define input line
std::string seperate_id(std::string input){
    std::size_t first_id = input.find_first_of('(', 0);
    std::size_t last_id = input.find_first_of(')', 0);
	if((first_id == std::string::npos) || (last_id == std::string::npos)) {
		std::string err_msg1 = "Error: Invalid ID!";
		throw err_msg1;
	}
    std::size_t length = last_id - first_id + 1;
    std::string id_list = input.substr(first_id, length);
	// remove spaces
	delete_spaces_and_tabs_on_head_and_tail(id_list);
	//  std::cout << "ID list is: " << id_list << '\n';
	if(id_list.empty()){
		std::string err_msg2 = "Error: No id is detected in input!";
		throw err_msg2;
	}
    return id_list;
}

// to seperate variables from right hand side(expression) of define input line
std::string seperate_expression(std::string &input) {
	// think about the evil case (f x = (+ x 2)), whether this handle is good or not
	// the next equal sign after the first ), which strictly comply with the example
	std::size_t first_id = input.find_first_of(')',0);
	// variable name might contain equal sign
    std::size_t start_of_expression = input.find('=', first_id);
	if((first_id == std::string::npos) || (start_of_expression == std::string::npos)) {
		std::string err_msg1 = "Error: Invalid expression!";
		throw err_msg1;
	}
    std::string expression_list = input.substr(start_of_expression + 1);
    delete_spaces_and_tabs_on_head_and_tail(expression_list);
	if(expression_list.empty()){
		std::string err_msg2 = "Error: No expression is detected in input!";
		throw err_msg2;
	}
    return expression_list;
}

// read the id_list and store the variables into vector of pairs, which could be later put into map
// admit that it is not a good function name
void get_variables_and_enlist(std::string::iterator &it, std::vector<var_pair> &var_pair_temp) {
    std::string temp_string;
	var_pair temp_pair;
	while(*it != ')') {
		temp_string = get_next_valid_str(it);
		if(temp_string.empty()){
			std::string err_msg1 = "Error: No argument!";
			throw err_msg1;
		}
		temp_pair = make_pair(temp_string, 0);
		var_pair_temp.push_back(temp_pair);
	}
}

//!!CITE!! this part is from 083_expr_eval3, slightly altered to meet the needs
Expression* makeExpr(std::string &op, Expression* lhs, Expression* rhs) {
	if(!op.compare(0, 1, "+")) {
		return new plusExpression(lhs,rhs);
	}
	else if(!op.compare(0, 1, "-")) {
		return new minusExpression(lhs,rhs);
	}
	else if(!op.compare(0, 1, "*")) {
		return new timesExpression(lhs,rhs);
	}
	else if(!op.compare(0, 1, "/")) {
		return new divExpression(lhs,rhs);
	}
	else if(!op.compare(0, 1, "%")) {
		return new modExpression(lhs,rhs);
	}
	else if(!op.compare(0, 3, "pow")) {
		return new powExpression(lhs,rhs);
	}
	else {
		std::cerr << "Error: Impossible op: " << op << "!" << std::endl;
		return NULL;
	}
}

// similar to makeExpr, just change the number of arg passed in
Expression* makeFunc(std::string &fname, Expression* var) {
	if(!fname.compare(0, 3, "sin")) {
		return new sinFunction(var);
	}
	else if(!fname.compare(0, 3, "cos")) {
		return new cosFunction(var);
	}
	else if(!fname.compare(0, 4, "sqrt")) {
		return new sqrtFunction(var);
	}
	else if(!fname.compare(0, 3, "log")) {
		return new logFunction(var);
	}
	else {
		std::cerr << "Error: Impossible function name: "<< fname << "!" << std::endl;
		return NULL;
	}
}

//!!CITE!! this part is from 083_expr_eval3, hugely altered to meet the needs
Expression* parse_expression(std::string::iterator &it, std::map<std::string, function*> &function_map, function* f) {
	skip_spaces(it);
	if(*it == '\0'){
		std::cerr << "Error: End of line found mid expression!" << std::endl;
		return NULL;
	}
	else if(*it == '(') {
		// (op E E)
		++it;
		return parse_operator(it, function_map, f);
	}
	else if(isdigit(*it) || *it=='+' || *it=='-' || *it=='.') {
		// there are several valid way of writing a number. e.g. +1, -3, .75, 0.02
		// but also could have evil vars like +AB, +-@#$, etc. so let us see if it is a number
		std::string::iterator temp = it;
		double value;
		std::string temp_string;
		value = my_strtod(it);
		if(std::isnan(value)) {
			std::cout << "Invalid number format! Try if it's a str variable." << std::endl;
			temp_string = get_next_valid_str(temp);
			if(f -> isVar(temp_string)) {
				std::cout << "Notice: Wow! It is actually a str var!" << std::endl;
				return new strExpr(temp_string);
			}
			else {
				std::cerr << "Error: The string" << temp_string << "is neither a number nor a existing variable!" << std::endl;
				return NULL;
			}
		}
		else {
			return new numExpr(value);
		}
	}
	else {
		// it should be a variable now
		skip_spaces(it);
		if(*it == ')') {
			// only one variable, give a NULL to rhs in parse_operator to continue
			return NULL;
		}
		std::string temp_string = get_next_valid_str(it);
		if(f -> isVar(temp_string)) {
			return new strExpr(temp_string);
		}
		else{
			std::cerr << "Error: The variable string " << temp_string << " is not defined in definition!" << std::endl;
			return NULL;
		}
	}
}

//!!CITE!! this part is from 083_expr_eval3, hugely altered to meet the needs
Expression* parse_operator(std::string::iterator &it, std::map<std::string, function*> &function_map, function *f) {
	skip_spaces(it);
	std::string op = get_next_valid_str(it);
	std::cout << "The operator is: " << op << "." << std::endl;
	// to see if the operator is in the map, if true, ask user whether to renew it
	if(is_function(op, function_map)) {
		std::map<std::string, function*>::iterator map_it = function_map.find(op);
		// found a existing function! call it.
		std::vector<Expression *> args;
		while(*it != ')') {
			Expression *expr = parse_expression(it, function_map, f);
			if(expr != NULL) {
				args.push_back(expr);
			}
		}
		if(args.size() != (*map_it).second -> varSize()) {
			if(args.size() > 0) {
				size_t i = 0;
				while(i < args.size()) {
				   delete args[i];
				   ++it;
				   i++;
				}
				std::cerr << "Error: Wrong usage(argument amount) of function "<< op << ", will abort parsing!" << std::endl;
				return NULL;
			}
			else {
				return NULL;
			}
		}
		++it;
		return new fnameExpr(op, (*map_it).second, args);
	}
	if(!is_valid_operator(op)) {
		std::cerr << "Error: Operator: " << op << " wrong format or not found!" << std::endl;
		return NULL;
	}
	// operators can also be variable names. test case was designed
	// evil cases... design some cases later to test it 
	if(f -> isVar(op) && (*(it+1) == ')')){
		return new strExpr(op);
	}
	
	//!!!CITE the following logic is pretty similar to 083_expr_eval3
	//! this part could be hacked! eg. operator +-*//
	std::string valid_operators ("+-*/% pow");
	std::string valid_fnames("sin cos sqrt log");
	Expression *lhs = NULL; 
	Expression *rhs = NULL;
	lhs = parse_expression(it, function_map, f);
	if(lhs == NULL) {
		return NULL;
	}
	if(valid_operators.find(op) != std::string::npos) {
		rhs = parse_expression(it, function_map, f);
	}
	if(rhs == NULL && ((valid_fnames.find(op)) == std::string::npos)) {
		delete lhs;
		return NULL;
	}
	skip_spaces(it);
	if(*it == ')') {
		++it;
		if(valid_operators.find(op) != std::string::npos) {
			return makeExpr(op, lhs, rhs);
		}
		else if(valid_fnames.find(op) != std::string::npos) {
			return makeFunc(op, lhs);
		}
		else{
			std::cout << "Just impossible, I verified beforehand." << std::endl;
		}
	}
	std::cerr << "Error: Expect ) but found " << *it << std::endl;
	delete lhs;
	delete rhs;
	return NULL;
}

// the wrapper of define line parsers
// some tasks: 1. devide the line into pieces 2. parse left and right respectively 3. parse operators as well as variables, and then store them
void parsing_define(std::string input, std::map<std::string, function*> &function_map) {
	// do some sanity check to avoid waste of resources
	if(input.find('=') == std::string::npos) {
		std::cerr << "Error: No equal sign in input!" << std::endl;
		return;
	}
	if(!parenthes_mismatch(input)) {
		std::cerr << "Error: Parenthes amount mismatch!" << std::endl;
		return;
	}
	std::string id_list;
	std::string expression_list;
	try {
		id_list = seperate_id(input);
		expression_list = seperate_expression(input);
	}
	catch(std::string err_msg) {
         std::cerr << err_msg << std::endl;
         return;
    }
	std::string::iterator id_it = id_list.begin();
	std::string fname;
	skip_spaces(id_it); 
	if((*id_it == '(')){
		++id_it;
		fname = get_next_valid_str(id_it);
		std::cout << "Name of function: " << fname << "." << std::endl;
		// move after the leading '('
		++id_it;
	}
	
	if(is_function(fname, function_map)) {
		std::cerr << "Error: Function name " << fname << " already exists. Abort re-definition!" << std::endl;
		return;
	}
	
	std::vector<var_pair> var_pair_temp;
	// first, let us deal with the id_list
	try {
		get_variables_and_enlist(id_it, var_pair_temp);
	}
	catch(std::string err_msg) {
         std::cerr << err_msg << std::endl;
         return;
    }
	// second, let us parse the right hand side expression into a working function
	function* f = new function(var_pair_temp);
	Expression* expr;
	std::string::iterator expr_it = expression_list.begin();
	expr = parse_expression(expr_it, function_map, f);
	if(expr == NULL) {
		std::cerr << "Error: Invalid expression! No expression parsed." << std::endl;
		delete f;
		return;
	}
	f -> addExpression(expr);
	//!!CITE http://www.cplusplus.com/reference/map/map/insert/
	if(function_map.insert(std::pair<std::string, function*> (fname, f)).second == true) {
		std::cout << "Successfully defined " << id_list << " = " << f -> toString() << "!" << std::endl;
		return;
	}
	else {
		// actually, this is not very frequent.
		delete f;
    }
}
