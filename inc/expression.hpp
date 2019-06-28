#ifndef HEADER_EXPRESSION
#define HEADER_EXPRESSION

#include <string>

using namespace std;

class Expression {

};

// Defines class for Expression+Expression on AT&T IA32 Assembly
class AddExpression : public Expression {

};

// Defines class for [Expression] on AT&T IA32 Assembly
class ContentOf : public Expression {
	private:
	Expression *exp;
	public:
	ContentOf(Expression *);
};

// Defines class for a label that points to some memory address
class Label : public Expression {
	private:
	string label;

	public:
	Label(string);
	long int getAddress();
};

// Defines class for an integer
class Integer : public Expression {
	private:
	long int value;

	public:
	Integer(long int);
};

// Defines class for a register
class Register : public Expression {
	private:
	string registerName;

	public:
	Register(string);
	string getName();
};

#endif
