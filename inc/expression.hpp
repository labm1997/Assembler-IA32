#ifndef HEADER_EXPRESSION
#define HEADER_EXPRESSION

#include <string>

using namespace std;

class Expression {
	public:
	virtual void prettyPrinter() = 0;
};

class AtomicExpression : public Expression {
	public:
	virtual void prettyPrinter() = 0;
};

// Defines class for Expression+Expression on AT&T IA32 Assembly
class AddExpression : public Expression {
	AtomicExpression *lhs;
	AtomicExpression *rhs;
	public:
	AddExpression(AtomicExpression *, AtomicExpression *);
	void prettyPrinter();
};

// Defines class for [Expression] on AT&T IA32 Assembly
class ContentOf : public Expression {
	private:
	Expression *exp;
	public:
	ContentOf(Expression *);
	void prettyPrinter();
};

// Defines class for a label that points to some memory address
class Label : public AtomicExpression {
	private:
	string label;

	public:
	Label(string);
	long int getAddress();
	string getName();
	void prettyPrinter();
};

// Defines class for an integer
class Integer : public AtomicExpression {
	private:
	long int value;

	public:
	Integer(long int);
	void prettyPrinter();
};

// Defines class for a register
class Register : public AtomicExpression {
	private:
	string registerName;

	public:
	Register(string);
	string getName();
	void prettyPrinter();
};

#endif
