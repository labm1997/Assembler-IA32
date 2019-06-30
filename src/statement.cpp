#include "statement.hpp"
#include <iostream>

Program::Program(StatementList statements, SymbolTable symbolTable) : statements(statements), symbolTable(symbolTable) {};

string Program::getObjectCode(){
    string out;

    // First passage algorithm
    this->firstPassage();

    for(auto it = symbolTable.begin() ; it != symbolTable.end() ; ++it){
        if((*it).second == nullptr) continue;
        cout << (*it).second->getName() << ": " << (*it).second->getAddress() << endl;
    }

    return out;
}

void Program::firstPassage(){
    uint32_t textPosition = 0;
    uint32_t dataPosition = 0;
    for(auto it = this->statements.begin(); it != this->statements.end() ; ++it){
        if((*it) == nullptr) continue;

        if((*it)->getSection() == "text"){
            if((*it)->getLabel() != nullptr){
                (*it)->getLabel()->setAddress(textPosition);
                textPosition += (*it)->size();
            }
        }
        else if ((*it)->getSection() == "data"){
            if((*it)->getLabel() != nullptr){
                (*it)->getLabel()->setAddress(dataPosition);
                dataPosition += (*it)->size();
            }
        }
        else {
            cout << "Unsupported section " << (*it)->getSection() << endl;
        }
    }
}

DeclareStatement::DeclareStatement(Label *label, vector<int32_t> data) : data(data) {
    this->label = label;
}

BinaryInstruction::BinaryInstruction(Label *label, AccessSize accessSize, Expression *lhs, Expression *rhs) {
    this->label = label;
    this->indirectAccessSize = accessSize;
    this->lhs = lhs;
    this->rhs = rhs;
}

UnaryInstruction::UnaryInstruction(Label *label, AccessSize accessSize, Expression *exp) {
    this->label = label;
    this->indirectAccessSize = accessSize;
    this->exp = exp;
}

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

uint32_t DeclareStatement::size(){
    return this->data.size() * 4;
}

uint32_t AddInstruction::size(){
    if(this->lhs->isType(REGISTER) && this->rhs->isType(CONTENTOF)){
        return 6 * 4;
    }
    else {
        cout << "Unsupported format for " << this->getName() << endl;
    }
    return 0;
}

uint32_t SubInstruction::size(){
    if(this->lhs->isType(REGISTER) && this->rhs->isType(CONTENTOF)){
        return 6 * 4;
    }
    else {
        cout << "Unsupported format for " << this->getName() << endl;
    }
    return 0;
}

uint32_t CmpInstruction::size(){
    if(this->lhs->isType(REGISTER) && this->rhs->isType(INTEGER)){
        return 3 * 4;
    }
    else {
        cout << "Unsupported format for " << this->getName() << endl;
    }
    return 0;
}

uint32_t MovInstruction::size(){
    if(this->lhs->isType(REGISTER) && this->rhs->isType(CONTENTOF)){
        if(((Register *)this->lhs)->getName() == "eax")
            return 5 * 4;
        else if(((Register *)this->lhs)->getName() == "ebx")
            return 6 * 4;
    }
    else if(this->lhs->isType(CONTENTOF) && this->rhs->isType(REGISTER)){
        if(((Register *)this->rhs)->getName() == "eax")
            return 5 * 4;
        else if(((Register *)this->rhs)->getName() == "ebx")
            return 6 * 4;
    }
    else if(this->lhs->isType(REGISTER) && this->rhs->isType(INTEGER)){
        return 5 * 4;
    }
    else {
        cout << "Unsupported format for " << this->getName() << endl;
    }
    return 0;
}

uint32_t MultInstruction::size(){
    if(this->exp->isType(CONTENTOF)){
        return 6 * 4;
    }
    else {
        cout << "Unsupported format for " << this->getName() << endl;
    }
    return 0;
}

uint32_t DivInstruction::size(){
    if(this->exp->isType(CONTENTOF)){
        return 6 * 4;
    }
    else {
        cout << "Unsupported format for " << this->getName() << endl;
    }
    return 0;
}

uint32_t JmpInstruction::size(){
    if(this->exp->isType(LABEL)){
        // !FIXME: Relative jump, it will only work if address to jump is not far
        return 2 * 4;
    }
    else {
        cout << "Unsupported format for " << this->getName() << endl;
    }
    return 0;
}

uint32_t JlInstruction::size(){
    if(this->exp->isType(LABEL)){
        return 2 * 4;
    }
    else {
        cout << "Unsupported format for " << this->getName() << endl;
    }
    return 0;
}

uint32_t JgInstruction::size(){
    if(this->exp->isType(LABEL)){
        return 2 * 4;
    }
    else {
        cout << "Unsupported format for " << this->getName() << endl;
    }
    return 0;
}

uint32_t JeInstruction::size(){
    if(this->exp->isType(LABEL)){
        return 2 * 4;
    }
    else {
        cout << "Unsupported format for " << this->getName() << endl;
    }
    return 0;
}

uint32_t PushInstruction::size(){
    if(this->exp->isType(LABEL)){
        return 5 * 4;
    }
    else if(this->exp->isType(CONTENTOF)){
        return 6 * 4;
    }
    else {
        cout << "Unsupported format for " << this->getName() << endl;
    }
    return 0;
}

uint32_t CallInstruction::size(){
    if(this->exp->isType(LABEL)){
        return 5 * 4;
    }
    else {
        cout << "Unsupported format for " << this->getName() << endl;
    }
    return 0;
}

uint32_t IntInstruction::size(){
    if(this->exp->isType(INTEGER)){
        return 2 * 4;
    }
    else {
        cout << "Unsupported format for " << this->getName() << endl;
    }
    return 0;
}
