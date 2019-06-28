#include "statement.hpp"

Program::Program(StatementList statements, SymbolTable symbolTable) : statements(statements), symbolTable(symbolTable) {};

BinaryInstruction::BinaryInstruction(Label *label, Expression *lhs, Expression *rhs) {
    this->arguments.push_back(lhs);
    this->arguments.push_back(rhs);
    this->label = label;
};

string AddInstruction::getObjectCode(){
	return "";
}
