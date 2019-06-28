#include "expression.hpp"

ContentOf::ContentOf(Expression *exp) : exp(exp) {};

Register::Register(string registerName) : registerName(registerName) {};

Integer::Integer(long int value) : value(value) {};

Label::Label(string label) : label(label) {};

string Register::getName(){
    return this->registerName;
}
