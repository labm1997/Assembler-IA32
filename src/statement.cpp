#include "statement.hpp"
#include <iostream>
#include <elfio/elfio.hpp>

using namespace ELFIO;

Program::Program(StatementList statements, SymbolTable symbolTable) : statements(statements), symbolTable(symbolTable) {};

void Program::saveExecutable(){
    elfio writer;
    ObjectCode objectCode = this->getObjectCode();
    MachineCode *text = objectCode.getText();

    MachineCode ioObjectCode = {0x55, 0x89, 0xe5, 0xb8, 0x03, 0x00, 0x00, 0x00, 0xbb, 0x00, 0x00, 0x00, 0x00, 0x8b, 0x4d, 0x08,
0xba, 0x02, 0x00, 0x00, 0x00, 0xcd, 0x80, 0x5d, 0xc2, 0x04, 0x00, 0x55, 0x89, 0xe5, 0xb8, 0x04,
0x00, 0x00, 0x00, 0xbb, 0x00, 0x00, 0x00, 0x00, 0x8b, 0x4d, 0x08, 0xba, 0x01, 0x00, 0x00, 0x00,
0xcd, 0x80, 0x4d, 0xc6, 0x45, 0x01, 0x0a, 0xb8, 0x04, 0x00, 0x00, 0x00, 0xbb, 0x00, 0x00, 0x00,
0x00, 0x89, 0xe9, 0x83, 0xc1, 0x01, 0xba, 0x01, 0x00, 0x00, 0x00, 0xcd, 0x80, 0x45, 0x5d, 0xc2,
0x04, 0x00, 0x55, 0x89, 0xe5, 0x68, 0x00, 0x00, 0x00, 0x10, 0x6a, 0x10, 0xff, 0x75, 0x08, 0xe8,
0x1a, 0x00, 0x00, 0x00, 0x5d, 0xc2, 0x00, 0x00, 0x55, 0x89, 0xe5, 0x68, 0x00, 0xca, 0x9a, 0x3b,
0x6a, 0x0a, 0xff, 0x75, 0x08, 0xe8, 0x04, 0x00, 0x00, 0x00, 0x5d, 0xc2, 0x00, 0x00, 0x55, 0x89,
0xe5, 0x50, 0x53, 0x83, 0x7d, 0x10, 0x00, 0x74, 0x48, 0x8b, 0x45, 0x08, 0xba, 0x00, 0x00, 0x00,
0x00, 0xf7, 0x75, 0x10, 0x89, 0x55, 0x08, 0x50, 0x83, 0xf8, 0x0a, 0x7c, 0x05, 0x83, 0xc0, 0x57,
0xeb, 0x03, 0x83, 0xc0, 0x30, 0x89, 0x45, 0xf0, 0xb8, 0x04, 0x00, 0x00, 0x00, 0xbb, 0x01, 0x00,
0x00, 0x00, 0x89, 0xe9, 0x83, 0xe9, 0x10, 0xba, 0x01, 0x00, 0x00, 0x00, 0xcd, 0x80, 0x83, 0xc4,
0x04, 0x8b, 0x45, 0x10, 0xba, 0x00, 0x00, 0x00, 0x00, 0xf7, 0x75, 0x0c, 0x89, 0x45, 0x10, 0xeb,
0xb2, 0x83, 0xc4, 0x04, 0x6a, 0x0a, 0xb8, 0x04, 0x00, 0x00, 0x00, 0xbb, 0x01, 0x00, 0x00, 0x00,
0x89, 0xe1, 0xba, 0x01, 0x00, 0x00, 0x00, 0xcd, 0x80, 0x5b, 0x58, 0x5d, 0xc2, 0x0c, 0x00, 0x55,
0x89, 0xe5, 0x6a, 0x10, 0xff, 0x75, 0x08, 0xe8, 0x15, 0x00, 0x00, 0x00, 0x5d, 0xc2, 0x04, 0x00,
0x55, 0x89, 0xe5, 0x6a, 0x0a, 0xff, 0x75, 0x08, 0xe8, 0x04, 0x00, 0x00, 0x00, 0x5d, 0xc2, 0x04,
0x00, 0x55, 0x89, 0xe5, 0x6a, 0x00, 0x6a, 0x00, 0x6a, 0x00, 0x50, 0x53, 0xc7, 0x45, 0xfc, 0x00,
0x00, 0x00, 0x00, 0xb8, 0x03, 0x00, 0x00, 0x00, 0xbb, 0x00, 0x00, 0x00, 0x00, 0x89, 0xe9, 0x83,
0xe9, 0x04, 0xba, 0x01, 0x00, 0x00, 0x00, 0xcd, 0x80, 0x83, 0x7d, 0xfc, 0x0a, 0x74, 0x38, 0x83,
0x7d, 0xfc, 0x2d, 0x75, 0x09, 0xc7, 0x45, 0xf4, 0x01, 0x00, 0x00, 0x00, 0xeb, 0xce, 0x83, 0x7d,
0xfc, 0x61, 0x7d, 0x06, 0x83, 0x6d, 0xfc, 0x30, 0xeb, 0x04, 0x83, 0x6d, 0xfc, 0x57, 0x8b, 0x45,
0xf8, 0xf7, 0x65, 0x0c, 0x83, 0x7d, 0xf4, 0x01, 0x75, 0x05, 0x2b, 0x45, 0xfc, 0xeb, 0x03, 0x03,
0x45, 0xfc, 0x89, 0x45, 0xf8, 0xeb, 0xa5, 0x8b, 0x45, 0x08, 0x8b, 0x5d, 0xf8, 0x89, 0x18, 0x5b,
0x58, 0x83, 0xc4, 0x0c, 0x5d, 0xc2, 0x08, 0x00};

    text->insert(text->end(), ioObjectCode.begin(), ioObjectCode.end());

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

        if(!(*it)->assemblable()) continue;

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

    cout << "Text position: " << textPosition << endl;

    // Add references to io procedures
    vector<pair<string, uint32_t>> labelsToCreate = {
        pair<string, uint32_t>("LerChar", 0x00),
        pair<string, uint32_t>("EscreverChar", 0x1b),
        pair<string, uint32_t>("EscreverHexa", 0x52),
        pair<string, uint32_t>("EscreverInteiro", 0x68),
        pair<string, uint32_t>("LerHexa", 0xef),
        pair<string, uint32_t>("LerInteiro", 0x100)
    };

    for(auto it = labelsToCreate.begin() ; it != labelsToCreate.end() ; ++it){
        if(this->symbolTable.count((*it).first) == 0){
            Label *label = new Label((*it).first);
            label->setSection("text");
            label->setAddress((*it).second + textPosition);
            this->symbolTable[(*it).first] = label;
        }
        else {
            Label *label = this->symbolTable[(*it).first];
            label->setSection("text");
            label->setAddress((*it).second + textPosition);
            cout << hex << label->getAddress() << endl;
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

        if(!(*it)->assemblable()) continue;

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

BinaryInstruction::BinaryInstruction(Label *label, AccessSize accessSize, Expression *lhs, Expression *rhs, string line) {
    this->label = label;
    this->indirectAccessSize = accessSize;
    this->lhs = lhs;
    this->rhs = rhs;
    this->line = line;
}

UnaryInstruction::UnaryInstruction(Label *label, AccessSize accessSize, Expression *exp, string line) {
    this->label = label;
    this->indirectAccessSize = accessSize;
    this->exp = exp;
    this->line = line;
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
