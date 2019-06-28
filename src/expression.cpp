#include "expression.hpp"

#include <iostream>

ContentOf::ContentOf(Expression *exp) : exp(exp) {};

Register::Register(string registerName) : registerName(registerName) {};

Integer::Integer(long int value) : value(value) {};

Label::Label(string label) : label(label) {};

string Register::getName(){
    return this->registerName;
}

string Label::getName(){
    return this->label;
}

void ContentOf::prettyPrinter(){
    cout << "[";
    this->exp->prettyPrinter();
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
