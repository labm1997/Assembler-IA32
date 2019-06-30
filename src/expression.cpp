#include "expression.hpp"

#include <iostream>

AddExpression::AddExpression(AtomicExpression *lhs, AtomicExpression *rhs) : lhs(lhs), rhs(rhs) {};

ContentOf::ContentOf(Expression *exp) : exp(exp) {};

Register::Register(string registerName) : registerName(registerName) {};

Integer::Integer(int32_t value) : value(value) {};

Label::Label(string label) : label(label) {};

string Register::getName(){
    return this->registerName;
}

string Label::getName(){
    return this->label;
}

uint32_t Label::getAddress(){
    return this->address;
}

void Label::setAddress(uint32_t address){
    this->address = address;
}

void AddExpression::prettyPrinter(){
    if(this->lhs != nullptr) this->lhs->prettyPrinter();
    cout << "+";
    if(this->rhs != nullptr) this->rhs->prettyPrinter();
}

void ContentOf::prettyPrinter(){
    cout << "[";
    if(this->exp != nullptr) this->exp->prettyPrinter();
    cout << "]";
}

void Register::prettyPrinter(){
    cout << this->registerName;
}

void Integer::prettyPrinter(){
    cout << to_string(this->value);
}

void Label::prettyPrinter(){
    cout << this->label;
}
