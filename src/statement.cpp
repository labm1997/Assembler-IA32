#include "statement.hpp"
#include <iostream>

Program::Program(StatementList statements, SymbolTable symbolTable) : statements(statements), symbolTable(symbolTable) {};

string Program::getObjectCode(){
    string out;
    // !TODO: first passage algorithm to get label addresses

    for(auto it = this->statements.begin() ; it != this->statements.end() ; ++it){
        out += (*it)->getObjectCode();
    }
    return out;
}

DeclareStatement::DeclareStatement(Label *label, vector<int32_t> data) : data(data) {
    this->label = label;
};

BinaryInstruction::BinaryInstruction(Label *label, AccessSize accessSize, Expression *lhs, Expression *rhs) {
    this->label = label;
    this->indirectAccessSize = accessSize;
    this->lhs = lhs;
    this->rhs = rhs;
};

UnaryInstruction::UnaryInstruction(Label *label, AccessSize accessSize, Expression *exp) {
    this->label = label;
    this->indirectAccessSize = accessSize;
    this->exp = exp;
};

void Program::prettyPrinter(){
    for(auto it = this->statements.begin() ; it != this->statements.end() ; ++it){
        (*it)->prettyPrinter();
    }
}

void UnaryInstruction::prettyPrinter(){
    if(this->label)
        cout << this->label->getName() << ": ";

    cout << this->getName();

    if(!this->indirectAccessSize.empty())
        cout << " " << this->indirectAccessSize;

    if(this->exp){
        cout << " ";
        this->exp->prettyPrinter();
    }
    cout << endl;

}

void BinaryInstruction::prettyPrinter(){
    if(this->label)
        cout << this->label->getName() << ": ";

    cout << this->getName();

    if(!this->indirectAccessSize.empty())
        cout << " " << this->indirectAccessSize;

    if(this->lhs){
        cout << " ";
        this->lhs->prettyPrinter();
    }

    if(this->rhs){
        cout << ",";
        this->rhs->prettyPrinter();
    }

    cout << endl;

}

void DeclareStatement::prettyPrinter(){
    if(this->label){
        this->label->prettyPrinter();
        cout << ": ";
    }

    cout << "dd";

    for(auto it = this->data.begin() ; it != this->data.end() ; ++it){
        cout << " " << to_string((*it));
        if(it+1 != this->data.end()) cout << ",";
    }

    cout << endl;
}
