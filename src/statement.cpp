#include "statement.hpp"
#include <iostream>
#include <elfio/elfio.hpp>

using namespace ELFIO;

Program::Program(StatementList statements, SymbolTable symbolTable) : statements(statements), symbolTable(symbolTable) {};

void Program::saveExecutable(){
    elfio writer;
    ObjectCode objectCode = this->getObjectCode();

    writer.create (ELFCLASS32, ELFDATA2LSB );

    writer.set_os_abi( ELFOSABI_LINUX );
    writer.set_type ( ET_EXEC );
    writer.set_machine( EM_386 );

    section *text_sec = writer.sections.add( ".text" );
    text_sec->set_type( SHT_PROGBITS );
    text_sec->set_flags( SHF_ALLOC | SHF_EXECINSTR );
    text_sec->set_addr_align ( 0x10 );
    text_sec->set_data( (char *)objectCode.getText()->data(), objectCode.getText()->size() );

    segment *text_seg = writer.segments.add();
    text_seg->set_type( PT_LOAD );
    text_seg->set_virtual_address( START_TEXT_ADDRESS );
    text_seg->set_physical_address( START_TEXT_ADDRESS );
    text_seg->set_flags( PF_X | PF_R );
    text_seg->set_align( 0x1000 );

    text_seg->add_section_index( text_sec->get_index(), text_sec->get_addr_align());

    section *data_sec = writer.sections.add( ".data" );
    data_sec->set_type( SHT_PROGBITS );
    data_sec->set_flags( SHF_ALLOC | SHF_WRITE );
    data_sec->set_addr_align( 0x4 );
    data_sec->set_data( (char *)objectCode.getData()->data(), objectCode.getData()->size() );

    segment *data_seg = writer.segments.add();
    data_seg->set_type( PT_LOAD );
    data_seg->set_virtual_address( START_DATA_ADDRESS );
    data_seg->set_physical_address( START_DATA_ADDRESS );
    data_seg->set_flags( PF_W | PF_R );
    data_seg->set_align( 0x1000 );

    data_seg->add_section_index( data_sec->get_index(), data_sec->get_addr_align());

    writer.set_entry( START_TEXT_ADDRESS );

    writer.save( "output" );

}

ObjectCode Program::getObjectCode(){

    // First passage algorithm
    this->firstPassage();

    // Print labels and its addresses
    for(auto it = symbolTable.begin() ; it != symbolTable.end() ; ++it){
        if((*it).second == nullptr) continue;
        cout << (*it).second->getName() << ": " << (*it).second->getAddress() << endl;
    }

    return this->secondPassage();

    //for(auto it = machineCode.getText()->begin() ; it != machineCode.getText()->end() ; ++it){
    //    cout << hex << (int)(*it) << " ";
    //}

    //cout << machineCode.getText();
    //cout << machineCode.getData();

}

void Program::firstPassage(){
    uint32_t textPosition = 0;
    uint32_t dataPosition = 0;
    for(auto it = this->statements.begin(); it != this->statements.end() ; ++it){
        if((*it) == nullptr) continue;

        if((*it)->getSection() == "text"){
            if((*it)->getLabel() != nullptr){
                (*it)->getLabel()->setAddress(textPosition);
                (*it)->getLabel()->setSection("text");
            }
            (*it)->setAddress(textPosition);
            textPosition += (*it)->size();
        }
        else if ((*it)->getSection() == "data"){
            if((*it)->getLabel() != nullptr){
                (*it)->getLabel()->setAddress(dataPosition);
                (*it)->getLabel()->setSection("data");
            }
            (*it)->setAddress(dataPosition);
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
    uint32_t tc = 0;
    uint32_t dc = 0;

    for(auto it = this->statements.begin(); it != this->statements.end() ; ++it){
        if((*it) == nullptr) continue;

        MachineCode code = (*it)->machineCode();

        if((*it)->getSection() == "text"){
            text->insert(text->end(), code.begin(), code.end());

            cout << hex << tc << "\t";
            tc += (*it)->size();
            for(auto it = code.begin() ; it != code.end() ; ++it){
                cout << hex << (int)(*it) << " ";
            }
            cout << " \t ";
            (*it)->prettyPrinter();
            cout << endl;

        }
        else if ((*it)->getSection() == "data"){
            data->insert(data->end(), code.begin(), code.end());

            cout << hex << dc << "\t";
            dc += (*it)->size();
            for(auto it = code.begin() ; it != code.end() ; ++it){
                cout << hex << (int)(*it) << " ";
            }
            cout << " \t ";
            (*it)->prettyPrinter();
            cout << endl;

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
    for(auto it = this->statements.begin() ; it != this->statements.end() ; ++it){
        (*it)->prettyPrinter();
        cout << endl;
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

}
