#include "statement.hpp"
#include <iostream>

uint32_t DeclareStatement::size(){
    return this->getDataLength() * 4;
}

uint32_t AddInstruction::size(){
    if(this->is(t_Register, t_ContentOfLabel)) return 6;
    if(this->is(t_Register, t_Integer)) return 3;
    cout << "Unsupported format for " << this->getName() << endl;
    return 0;
}

uint32_t SubInstruction::size(){
    if(this->is(t_Register, t_ContentOfLabel)) return 6;
    cout << "Unsupported format for " << this->getName() << endl;
    return 0;
}

uint32_t ShlInstruction::size(){
    if(this->is(t_Register, t_Integer)) return 3;
    cout << "Unsupported format for " << this->getName() << endl;
    return 0;
}

uint32_t CmpInstruction::size(){
    if(this->is(t_Register, t_Integer)) return 3;
    cout << "Unsupported format for " << this->getName() << endl;
    return 0;
}

uint32_t MovInstruction::size(){
    if(this->is(t_Register, t_ContentOfLabel)){
        Register *reg = dynamic_cast<Register *>(this->getLhs());
        if(reg->getName() == "eax") return 5;
        if(reg->getName() == "ebx") return 6;
    }

    if(this->is(t_Register, t_ContentOfRegister)){
        Register *reg = dynamic_cast<Register *>(this->getLhs());
        if((reg->getName() == "ecx" || reg->getName() == "edx"))
            return 3;
    }

    if(this->is(t_ContentOfLabel, t_Register)){
        Register *reg = dynamic_cast<Register *>(this->getRhs());
        if(reg->getName() == "eax") return 5;
        if(reg->getName() == "ebx") return 6;
    }

    if(this->is(t_Register, t_Integer)) return 5;

    else if(this->is(t_Register, t_Register)) return 2;

    cout << "Unsupported format for " << this->getName() << endl;
    return 0;
}

uint32_t MultInstruction::size(){
    if(this->is(t_ContentOfLabel)) return 6;

    cout << "Unsupported format for " << this->getName() << endl;
    return 0;
}

uint32_t DivInstruction::size(){
    if(this->is(t_ContentOfLabel)) return 6;
    cout << "Unsupported format for " << this->getName() << endl;
    return 0;
}

uint32_t JmpInstruction::size(){
    // !FIXME: Relative jump, it will only work if address to jump is not far
    if(this->is(t_Label)) return 2;
    cout << "Unsupported format for " << this->getName() << endl;
    return 0;
}

uint32_t JlInstruction::size(){
    if(this->is(t_Label)) return 2;
    cout << "Unsupported format for " << this->getName() << endl;
    return 0;
}

uint32_t JgInstruction::size(){
    if(this->is(t_Label)) return 2;
    cout << "Unsupported format for " << this->getName() << endl;
    return 0;
}

uint32_t JeInstruction::size(){
    if(this->is(t_Label)) return 2;
    cout << "Unsupported format for " << this->getName() << endl;
    return 0;
}

uint32_t PushInstruction::size(){
    if(this->is(t_Label) || this->is(t_LabelAdd)) return 5;
    if(this->is(t_ContentOfLabel)) return 6;
    if(this->is(t_Register)) return 1;
    cout << "Unsupported format for " << this->getName() << endl;
    return 0;
}

uint32_t CallInstruction::size(){
    if(this->is(t_Label)) return 5;
    cout << "Unsupported format for " << this->getName() << endl;
    return 0;
}

uint32_t IntInstruction::size(){
    if(this->is(t_Integer)) return 2;
    cout << "Unsupported format for " << this->getName() << endl;
    return 0;
}

uint32_t PopInstruction::size(){
    if(this->is(t_Register)){
        Register *reg = dynamic_cast<Register *>(this->getExp());
        if(reg->getName() == "ebp") return 1;
        cout << "Unsupported register " << reg->getName() << endl;
    }
    cout << "Unsupported format for " << this->getName() << endl;
    return 0;
}

uint32_t RetInstruction::size(){
    if(this->is(t_Integer)) return 3;
    cout << "Unsupported format for " << this->getName() << endl;
    return 0;
}
