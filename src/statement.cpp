#include "statement.hpp"
#include <iostream>

Program::Program(StatementList statements, SymbolTable symbolTable) : statements(statements), symbolTable(symbolTable) {};

BinaryInstruction::BinaryInstruction(Label *label, AccessSize accessSize, Expression *lhs, Expression *rhs) {
    this->label = label;
    this->indirectAccessSize = accessSize;
    this->arguments.push_back(lhs);
    this->arguments.push_back(rhs);
};

string AddInstruction::getObjectCode(){
	return "";
}

string MovInstruction::getObjectCode(){
	return "";
}

void Program::prettyPrinter(){
    for(auto it = this->statements.begin() ; it != this->statements.end() ; ++it){
        (*it)->prettyPrinter();
    }
}

void BinaryInstruction::prettyPrinter(){
    if(this->label)
        cout << this->label->getName() << ": ";

    cout << this->getName();

    if(!this->indirectAccessSize.empty())
        cout << " " << this->indirectAccessSize;

    if(this->arguments[0]){
        cout << " ";
        this->arguments[0]->prettyPrinter();
    }

    if(this->arguments[1]){
        cout << ",";
        this->arguments[1]->prettyPrinter();
    }

    cout << endl;

}
