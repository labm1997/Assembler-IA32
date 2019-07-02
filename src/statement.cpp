#include "statement.hpp"
#include <iostream>

uint32_t tc = 0;
uint32_t dc = 0;

Program::Program(StatementList statements, SymbolTable symbolTable) : statements(statements), symbolTable(symbolTable) {};

string Program::getObjectCode(){

    // First passage algorithm
    this->firstPassage();

    // Print labels and its addresses
    for(auto it = symbolTable.begin() ; it != symbolTable.end() ; ++it){
        if((*it).second == nullptr) continue;
        cout << (*it).second->getName() << ": " << (*it).second->getAddress() << endl;
    }

    ObjectCode machineCode = this->secondPassage();

    for(auto it = machineCode.getText()->begin() ; it != machineCode.getText()->end() ; ++it){
        cout << hex << (int)(*it) << " ";
    }

    //cout << machineCode.getText();
    //cout << machineCode.getData();

    return "";
}

void Program::firstPassage(){
    uint32_t textPosition = 0;
    uint32_t dataPosition = 0;
    for(auto it = this->statements.begin(); it != this->statements.end() ; ++it){
        if((*it) == nullptr) continue;

        if((*it)->getSection() == "text"){
            if((*it)->getLabel() != nullptr){
                (*it)->getLabel()->setAddress(textPosition);
            }
            textPosition += (*it)->size();
        }
        else if ((*it)->getSection() == "data"){
            if((*it)->getLabel() != nullptr){
                (*it)->getLabel()->setAddress(dataPosition);
            }
            dataPosition += (*it)->size();
        }
        else {
            cout << "Unsupported section " << (*it)->getSection() << endl;
        }
    }
}

ObjectCode Program::secondPassage(){
    MachineCode *text = new MachineCode;
    MachineCode *data = new MachineCode;

    for(auto it = this->statements.begin(); it != this->statements.end() ; ++it){
        if((*it) == nullptr) continue;

        MachineCode code = (*it)->machineCode();

        if((*it)->getSection() == "text"){
            text->insert(text->end(), code.begin(), code.end());
        }
        else if ((*it)->getSection() == "data"){
            data->insert(data->end(), code.begin(), code.end());
        }
        else {
            cout << "Unsupported section " << (*it)->getSection() << endl;
        }
    }

    return ObjectCode(text, data);
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
    tc = 0;
    dc = 0;
    for(auto it = this->statements.begin() ; it != this->statements.end() ; ++it){
        (*it)->prettyPrinter();
    }
}

void UnaryInstruction::prettyPrinter(){
    cout << hex << tc << "\t";
    tc += this->size();

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
    cout << hex << tc << "\t";
    tc += this->size();

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
    cout << hex << dc << "\t";
    dc += this->size();

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
