#ifndef HEADER_STATEMENT
#define HEADER_STATEMENT

#include <string>
#include <list>
#include <vector>
#include <fstream>
#include <map>

#include "expression.hpp"

using namespace std;

typedef map<string, Label *> SymbolTable;

class Statement {
	protected:
	Label *label;
	vector<Expression *> arguments;

	public:
	virtual string getObjectCode() = 0;
	virtual void prettyPrinter() = 0;
};

typedef list<Statement *> StatementList;
typedef string AccessSize;

class Instruction : public Statement {
	protected:
	AccessSize indirectAccessSize;
	public:
	virtual string getObjectCode() = 0;
	virtual void prettyPrinter() = 0;
	virtual string getName() = 0;
};

class Program {
	private:
	// List of program statements
	StatementList statements;

	// Hash map to labels
	SymbolTable symbolTable;

	public:
	Program(StatementList, SymbolTable);
	void prettyPrinter();

};

class BinaryInstruction : public Instruction {
	public:
	BinaryInstruction(Label *, AccessSize, Expression *, Expression *);
	void prettyPrinter();
	virtual string getName() = 0;
};

class AddInstruction : public BinaryInstruction {
	public:
	// Inherits BinaryExpression constructor
	using BinaryInstruction::BinaryInstruction;
	string getObjectCode();
	string getName() { return "add"; };
};

class MovInstruction : public BinaryInstruction {
	public:
	// Inherits BinaryExpression constructor
	using BinaryInstruction::BinaryInstruction;
	string getObjectCode();
	string getName() { return "mov"; };
};


#endif
