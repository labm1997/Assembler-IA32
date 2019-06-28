#ifndef HEADER_PARSER
#define HEADER_PARSER

#include "statement.hpp"
#include "expression.hpp"

class Parser {
	private:
	static Instruction *matchInstruction(string, SymbolTable&);
	static ContentOf *matchContentOf(string, SymbolTable&);
	static Expression *matchExpression(string, SymbolTable&);
	static Register *matchRegister(string);
	static Integer *matchInteger(string);
	static Label *matchLabel(string, SymbolTable&);

	public:
	static Program* parser(ifstream *);
};

#endif
