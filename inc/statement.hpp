#ifndef HEADER_STATEMENT
#define HEADER_STATEMENT

#include <string>
#include <list>
#include <fstream>
#include <map>

#include "expression.hpp"

using namespace std;

typedef map<string, Label *> SymbolTable;

class Statement {
	protected:
	Label *label;
	list<Expression *> arguments;

	public:
	virtual string getObjectCode() = 0;
};

typedef list<Statement *> StatementList;

class Instruction : public Statement {
	virtual string getObjectCode() = 0;
};

class Program {
	private:
	// List of program statements
	StatementList statements;

	// Hash map to labels
	SymbolTable symbolTable;

	public:
	Program(StatementList, SymbolTable);

};

class BinaryInstruction : public Instruction {
	public:
	BinaryInstruction(Label *, Expression *, Expression *);
};

class AddInstruction : public BinaryInstruction {
	public:
	// Inherits BinaryExpression constructor
	using BinaryInstruction::BinaryInstruction;
	string getObjectCode();
};

class MovInstruction : public Instruction {

};


#endif
