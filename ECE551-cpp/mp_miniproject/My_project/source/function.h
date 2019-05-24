#ifndef __FUNCTION_H__
#define __FUNCTION_H__

#include <cstdio>
#include <cstdlib>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <map>
#include <cmath>

/*****************************************************************************
Copyright: 2017, Ting
File name: function.h
Description: Provide the basic data structure to store the expressions and evaluate the functions
Author: Ting
Version: 1.0
Date: 11.27
History: 11.27 - 1.0,
*****************************************************************************/

typedef std::pair<std::string, double> var_pair;

class Expression {
	protected:
		double value;
	public:
		Expression(): value(0) {}
		Expression(double v): value(v) {}
		virtual std::string toString() const = 0;  //abstract method
		virtual double evaluate() = 0;  // to handle the number in the input string expression
		virtual double evaluate(std::vector<var_pair> &values) const = 0; // to handle a passed-in function
		virtual ~Expression() {} //destructor
};

// the data structure to store the function expression
class function: public Expression {
	private:
		std::vector<var_pair> var_list;
		Expression* expr;
	public:
		function(): expr(NULL) {}
		function(std::vector<var_pair> &v): var_list(v), expr(NULL) {}
		function(std::vector<var_pair> &v, Expression* e): var_list(v), expr(e) {}
		virtual std::string toString() const {
			return expr -> toString();
		}
		virtual double evaluate() {
			return expr -> evaluate(var_list);
		}
		virtual double evaluate(std::vector<var_pair> &v) const {
			// already detected in functions.
			// assert(v.size() == var_list.size()); // use test case to check if correct
			return expr -> evaluate(v);
		}
		// to set the value for the whole list, since it needs extra work to assign each one by one
		void setAllValue(std::vector<double> v) {
			// already detected in functions.
			// assert(v.size() == var_list.size()); // should be same length
			unsigned i = 0; // cannot use a single iterator to do two vectors.
			while(i < var_list.size()) {
				var_list[i].second = v[i];	
				i++;
			}
		}
		// to print all the variables and there values into cout to check them
		void printVarList() const {
			if(!var_list.empty()) {
				std::vector<var_pair>::const_iterator it = var_list.begin();
				while(it != var_list.end()) {
					std::cout << " " << (*it).first << " = ";
					std::cout << (*it).second;
					++it;
				}
			}
		}
		// to check if a symbol is an existing variable in the list
		bool isVar(const std::string &var) const {
			if(!var_list.empty()) {
				std::vector<var_pair>::const_iterator it = var_list.begin();
				while(it != var_list.end()) {
					if((*it).first == var) {
						return true;
					}
					++it;
				}
				return false;
			}
			return false;
		}
		// to return the size of the list
		unsigned varSize() const {
			return var_list.size();
		}
		// to get the value of a existing variable name
		double getVarValue(const std::string &var) const {
			if(!var_list.empty()) {
				std::vector<var_pair>::const_iterator it = var_list.begin();
				while(it != var_list.end()) {
					if((*it).first == var) {
						return (*it).second;
					}
					++it;
				}
				std::cout << "Could not get value for variable " << var << ". Not initialized."<< std::endl;
			}
		}
		// to get the expression
		Expression* getExpression() {
			return expr;
		}
		std::vector<var_pair> getVarList() {
			return var_list;
		}
		void addExpression(Expression *e) {
			expr = e;
		}
		virtual ~function() {
			delete expr;
		}
};
// the basic function_map that I use afterwards.
typedef std::map<std::string, function*> function_map;

// furthermore, we need 3 classes to handle user-defined functions: fname and variables(number/ string)

class fnameExpr: public Expression {
	private:
		std::string fname;
		function* fbody;
		std::vector<Expression*> args;
	public:
		fnameExpr(std::string &n, function *f, std::vector<Expression*> &e): fname(n), fbody(f), args(e) {}
		// in order to print it in a pretty way, we have to sacrifice some tidiness
		virtual std::string toString() const {
			std::stringstream fexpr;
			fexpr << fname;
			if(!args.empty()) {
				fexpr << '(';
				unsigned i = 0;
				while(i < args.size() - 1) {
					fexpr << args[i] -> toString();
					fexpr << ',';
					i++;
				}
				fexpr << args[i] -> toString() << ')';
			}
			return fexpr.str();
		}
		virtual double evaluate() {
			return 0;
		}
		// there comes the complex part! somehow recursively handle the passed-in function expression
		virtual double evaluate(std::vector<var_pair> &v) const {
			unsigned i = 0;
			double temp;
			std::vector<double> temp_vec;
			while(i < args.size()) {
				temp = args[i] -> evaluate(v);
				temp_vec.push_back(temp);
				i++;
			}
			fbody -> setAllValue(temp_vec);
			return fbody -> evaluate();
		}
		// destructor
		virtual ~fnameExpr(){
			std::vector<Expression*>::iterator it = args.begin();
			while(it != args.end()) {
				delete *it;
				++it;
			}
		}
};

class numExpr: public Expression {
	public:
		numExpr(double v): Expression(v) {}
		std::string toString() const {
			std::stringstream num_str;
			num_str << value;
			return num_str.str();
		}
		virtual double evaluate() {
			// should be right, also if return 0
			return value; 
		}
		virtual double evaluate(std::vector<var_pair> &v) const {
			return value;
		}
		virtual ~numExpr() {}
};

class strExpr: public Expression {
	private:
		std::string str_var;
	public:
		strExpr(std::string n): str_var(n) {}
		virtual std::string toString() const {
			return str_var;
		}
		virtual double evaluate() {
			return value;
		}
		virtual double evaluate(std::vector<var_pair> & v) const {
			std::vector<var_pair>::iterator it = v.begin();
			while(it != v.end()) {
				if((*it).first == str_var) {
					return (*it).second;      
				}
				++it;
			}
			// already handled in functions
			// if the error handling here is correct??
			// std::cerr<< "Did not find name in value vector.\n";
			return value;
		}
		virtual ~strExpr() {}
};

//!!CITE!! this part is from 083_expr_eval3, slightly altered to meet the needs
class makeExpr: public Expression {
	protected:
		char op_temp;
		Expression* left;
		Expression* right;
	public:
	// not sure if an operator reload should be written here
		makeExpr(char op, Expression* lhs, Expression* rhs): op_temp(op), left(lhs), right(rhs) {}
		virtual std::string toString() const {
			std::stringstream myHandledString;
			myHandledString << '(' << left -> toString() << ' ' << op_temp << ' ' + right -> toString() << ')';
			return myHandledString.str();
		}
		virtual double evaluate() {
			switch(op_temp) {
				case '+':
					return left -> evaluate() + right -> evaluate();
					break;
				case '-':
					return left -> evaluate() - right -> evaluate(); 
					break;
				case '*':
					return left -> evaluate() * right -> evaluate(); 
					break;
				case '/':
					return left -> evaluate() / right -> evaluate();
					break;
				case '%':
					return fmod(left -> evaluate(), right -> evaluate());
					break;
				default:
					std::cerr << "Impossible operator char: " << op_temp << "\n" << std::endl;
					return 0;
					break;
			}
		}
		virtual double evaluate(std::vector<var_pair> & v) const {
			switch(op_temp) {
				case '+':
					return (left -> evaluate(v) + right -> evaluate(v));
					break;
				case '-':
					return (left -> evaluate(v) - right -> evaluate(v)); 
					break;
				case '*':
					return (left -> evaluate(v) * right -> evaluate(v)); 
					break;
				case '/':
					return (left -> evaluate(v) / right -> evaluate(v)); 
					break;
				case '%':
					return (fmod(left -> evaluate(v), right -> evaluate(v))); 
					break;
				default:
					std::cerr << "Impossible operator char: " << op_temp << "\n" << std::endl;
					return 0;
					break;
			}
		}
		virtual ~makeExpr() {
			delete left;
			delete right;
		}
};

// this section is to adapt the built-in functions
class makeFunc: public Expression {
	protected:
		std::string name_temp;
		Expression* var_temp;
	public:
		makeFunc(std::string fname, Expression* var): name_temp(fname), var_temp(var) {}
		virtual std::string toString() const {
			std::stringstream myHandledString;
			myHandledString << name_temp << '(' << var_temp -> toString() << ')';
			return myHandledString.str();
		}
		virtual double evaluate() {
			if(!name_temp.compare(0, 3, "sin")) {
				return sin(var_temp -> evaluate());
			}
			else if(!name_temp.compare(0, 3, "cos")) {
				return cos(var_temp -> evaluate());
			}
			else if(!name_temp.compare(0, 4, "sqrt")) {
				return sqrt(var_temp -> evaluate());
			}
			else if(!name_temp.compare(0, 3, "log")) {
				return log(var_temp -> evaluate());
			}
			else {
				std::cerr << "Impossible function name: "<< name_temp << "\n" << std::endl;
				return 0;
			}
		}
		virtual double evaluate(std::vector<var_pair> & v) const {
			if(!name_temp.compare(0, 3, "sin")) {
				return sin(var_temp -> evaluate(v));
			}
			else if(!name_temp.compare(0, 3, "cos")) {
				return cos(var_temp -> evaluate(v));
			}
			else if(!name_temp.compare(0, 4, "sqrt")) {
				return sqrt(var_temp -> evaluate(v));
			}
			else if(!name_temp.compare(0, 3, "log")) {
				return log(var_temp -> evaluate(v));
			}
			else {
				std::cerr << "Impossible function name: "<< name_temp << "\n" << std::endl;
				return 0;
			}
		}
		virtual ~makeFunc() {
			delete var_temp;
		}
};

// first, we handle the operators
class plusExpression : public makeExpr{
	public:
		plusExpression(Expression* lhs, Expression* rhs) : makeExpr('+', lhs, rhs) {}
};

class minusExpression : public makeExpr{
	public:
		minusExpression(Expression* lhs, Expression* rhs) : makeExpr('-', lhs, rhs) {}
};

class timesExpression : public makeExpr{
	public:
		timesExpression(Expression* lhs, Expression* rhs) : makeExpr('*', lhs, rhs) {}
};

class divExpression : public makeExpr{
	public:
		divExpression(Expression* lhs, Expression* rhs) : makeExpr('/', lhs, rhs) {}
};

class modExpression : public makeExpr{
	public:
		modExpression(Expression* lhs, Expression* rhs) : makeExpr('%', lhs, rhs) {}
};

// next, we goes through the unary functions
class sinFunction : public makeFunc{
	public:
		sinFunction(Expression* var) : makeFunc("sin", var) {}
};

class cosFunction : public makeFunc{
	public:
		cosFunction(Expression* var) : makeFunc("cos", var) {}
};

class sqrtFunction : public makeFunc{
	public:
		sqrtFunction(Expression* var) : makeFunc("sqrt", var) {}
};

class logFunction : public makeFunc{
	public:
		logFunction(Expression* var) : makeFunc("log", var) {}
};

// finally, we have to handle this particular function which takes two parameters
class powExpression : public Expression{
	private:
		Expression* lhs;
		Expression* rhs;
	public:
		virtual std::string toString() const {
			std::stringstream myHandledString;
			myHandledString << "pow(" << lhs -> toString() << ',' << rhs -> toString() << ')';
			return myHandledString.str();
		}
		virtual double evaluate() {
			return pow(lhs -> evaluate(), rhs -> evaluate());
		}
		// this part is confusing, try to rethink it tomorrow
		virtual double evaluate(std::vector<var_pair> &v) const {
			return pow(lhs -> evaluate(v), rhs -> evaluate(v));
		}
		powExpression(Expression *l, Expression *r): lhs(l), rhs(r) {}
		virtual ~powExpression() {
			delete lhs;
			delete rhs;
		}
};

#endif
