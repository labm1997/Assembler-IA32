#include "expression.hpp"
#include "statement.hpp"

#include <iostream>

ContentOfLabel::ContentOfLabel(Label *label, Integer *offset) : label(label), offset(offset) {};
ContentOfRegister::ContentOfRegister(Register *reg, Integer *offset) : reg(reg), offset(offset) {};

Register::Register(string registerName) : registerName(registerName) {};

Integer::Integer(int32_t value) : value(value) {};

Label::Label(string label) : label(label) {};
LabelAdd::LabelAdd(Label *label, Integer *offset) : label(label), offset(offset) {};

string Register::getName(){
    return this->registerName;
}

string Label::getName(){
    return this->label;
}

uint32_t Label::getAddress(){
    if(this->getSection() == "text") return this->address + START_TEXT_ADDRESS;
    if(this->getSection() == "data") return this->address + START_DATA_ADDRESS;
    return 0;
}

void Label::setAddress(uint32_t address){
    this->address = address;
}

void LabelAdd::prettyPrinter(){
    if(this->label != nullptr) this->label->prettyPrinter();
    if(this->offset != nullptr) {
        cout << "+";
        this->offset->prettyPrinter();
    }
}

void ContentOfLabel::prettyPrinter(){
    cout << "[";
    if(this->label != nullptr) this->label->prettyPrinter();
    if(this->offset != nullptr) {
        cout << "+";
        this->offset->prettyPrinter();
    }
    cout << "]";
}

void ContentOfRegister::prettyPrinter(){
    cout << "[";
    if(this->reg != nullptr) this->reg->prettyPrinter();
    if(this->offset != nullptr) {
        cout << "+";
        this->offset->prettyPrinter();
    }
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
