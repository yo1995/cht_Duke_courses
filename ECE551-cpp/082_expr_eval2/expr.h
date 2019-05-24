#ifndef __EXPR_H___
#define __EXPR_H___

#include <sstream>
#include <string>

//"(+ 4 3)" -> "(4 + 3)"

class Expression {
	public:
		virtual std::string toString() const = 0;  //abstract method, to handle the + in the input string expression
		virtual long evaluate() const = 0;  // to handle the number in the input string expression, first part needless
		virtual ~Expression() {} //destructor
};

// use a helper child class to handle the operators in the input string
class makeExpr : public Expression {
	protected:
		std::string myHandledString;
		long eval;
	public:
		makeExpr(char op, Expression * lhs, Expression * rhs){
			//e.g. return "(4 + 5)"
			myHandledString = '(' + lhs -> toString() + ' ' + op + ' ' + rhs -> toString() + ')';
		}
		virtual std::string toString() const{
			return myHandledString;
		}
		virtual long evaluate() const{
			return eval;
		}
};

class NumExpression : public Expression {
	//A constructor: NumExpression(long) 
	private:
		long numberProcessing;
	public:
		NumExpression(long n): numberProcessing(n) {}
		//std::string toString() const //actually implement it
		virtual std::string toString() const {
			std::stringstream s;
			std::string str;
			s << numberProcessing;
			str = s.str();
			s.clear();
			return str;
		}
		virtual long evaluate() const{
			return numberProcessing;
		}
		virtual ~NumExpression() {}
		
};

class PlusExpression : public makeExpr {
	//A constructor: PlusExpression(Expression * lhs, Expression * rhs)
	//take in two expressions and combine them with a + in the middle between them
	public:
		PlusExpression(Expression * lhs, Expression * rhs) : makeExpr('+', lhs, rhs) {
			delete lhs;
			delete rhs;
		}	
    //std::string toString() const //actually implement it
};

class MinusExpression : public makeExpr {
	//A constructor: MinusExpression(Expression * lhs, Expression * rhs)
	//take in two expressions and combine them with a + in the middle between them
	public:
		MinusExpression(Expression * lhs, Expression * rhs) : makeExpr('-', lhs, rhs) {
			delete lhs;
			delete rhs;
		}
    //std::string toString() const //actually implement it
};

class TimesExpression : public makeExpr {
	//A constructor: TimesExpression(Expression * lhs, Expression * rhs)
	//take in two expressions and combine them with a + in the middle between them
	public:
		TimesExpression(Expression * lhs, Expression * rhs) : makeExpr('*', lhs, rhs) {
			delete lhs;
			delete rhs;
		}
    //std::string toString() const //actually implement it
};

class DivExpression : public makeExpr {
	//A constructor: DivExpression(Expression * lhs, Expression * rhs)
	//take in two expressions and combine them with a + in the middle between them
	public:
		DivExpression(Expression * lhs, Expression * rhs) : makeExpr('/', lhs, rhs) {
			delete lhs;
			delete rhs;
		}
    //std::string toString() const //actually implement it
};

#endif
