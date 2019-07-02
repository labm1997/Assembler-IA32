#include "statement.hpp"
#include <iostream>
#include <sstream>

MachineCode DeclareStatement::machineCode(){
    return vector<uint8_t>({});
}

MachineCode AddInstruction::machineCode(){
    if(this->is(t_Register, t_ContentOf)) return vector<uint8_t>({});
    if(this->is(t_Register, t_Integer)) {
        int32_t value = dynamic_cast<Integer *>(this->getRhs())->getValue();
        return vector<uint8_t>({0x83, 0xc1, (uint8_t)value});
    }
    cout << "Unsupported format for " << this->getName() << endl;
    return vector<uint8_t>({});
}

MachineCode SubInstruction::machineCode(){
    if(this->is(t_Register, t_ContentOf)) return vector<uint8_t>({});
    cout << "Unsupported format for " << this->getName() << endl;
    return vector<uint8_t>({});
}

MachineCode ShlInstruction::machineCode(){
    if(this->is(t_Register, t_Integer)) return vector<uint8_t>({});
    cout << "Unsupported format for " << this->getName() << endl;
    return vector<uint8_t>({});
}

MachineCode CmpInstruction::machineCode(){
    if(this->is(t_Register, t_Integer)) return vector<uint8_t>({});
    cout << "Unsupported format for " << this->getName() << endl;
    return vector<uint8_t>({});
}

MachineCode MovInstruction::machineCode(){
    if(this->is(t_Register, t_ContentOf)){
        Register *reg = dynamic_cast<Register *>(this->getLhs());
        ContentOf *contentof = dynamic_cast<ContentOf *>(this->getRhs());
        if(reg->getName() == "eax") return vector<uint8_t>({});
        if(reg->getName() == "ebx") return vector<uint8_t>({});
        if((reg->getName() == "ecx" || reg->getName() == "edx") &&
            contentof->getExpression()->isType(t_AddExpression))
            return vector<uint8_t>({});
    }

    if(this->is(t_ContentOf, t_Register)){
        Register *reg = dynamic_cast<Register *>(this->getRhs());
        if(reg->getName() == "eax") return vector<uint8_t>({});
        if(reg->getName() == "ebx") return vector<uint8_t>({});
    }

    if(this->is(t_Register, t_Integer)) return vector<uint8_t>({});

    else if(this->is(t_Register, t_Register)) return vector<uint8_t>({});

    cout << "Unsupported format for " << this->getName() << endl;
    return vector<uint8_t>({});
}

MachineCode MultInstruction::machineCode(){
    if(this->is(t_ContentOf)) return vector<uint8_t>({});

    cout << "Unsupported format for " << this->getName() << endl;
    return vector<uint8_t>({});
}

MachineCode DivInstruction::machineCode(){
    if(this->is(t_ContentOf)) return vector<uint8_t>({});
    cout << "Unsupported format for " << this->getName() << endl;
    return vector<uint8_t>({});
}

MachineCode JmpInstruction::machineCode(){
    // !FIXME: Relative jump, it will only work if address to jump is not far
    if(this->is(t_Label)) return vector<uint8_t>({});
    cout << "Unsupported format for " << this->getName() << endl;
    return vector<uint8_t>({});
}

MachineCode JlInstruction::machineCode(){
    if(this->is(t_Label)) return vector<uint8_t>({});
    cout << "Unsupported format for " << this->getName() << endl;
    return vector<uint8_t>({});
}

MachineCode JgInstruction::machineCode(){
    if(this->is(t_Label)) return vector<uint8_t>({});
    cout << "Unsupported format for " << this->getName() << endl;
    return vector<uint8_t>({});
}

MachineCode JeInstruction::machineCode(){
    if(this->is(t_Label)) return vector<uint8_t>({});
    cout << "Unsupported format for " << this->getName() << endl;
    return vector<uint8_t>({});
}

MachineCode PushInstruction::machineCode(){
    if(this->is(t_Label)) return vector<uint8_t>({});
    if(this->is(t_ContentOf)) return vector<uint8_t>({});
    if(this->is(t_Register)) return vector<uint8_t>({});
    cout << "Unsupported format for " << this->getName() << endl;
    return vector<uint8_t>({});
}

MachineCode CallInstruction::machineCode(){
    if(this->is(t_Label)) return vector<uint8_t>({});
    cout << "Unsupported format for " << this->getName() << endl;
    return vector<uint8_t>({});
}

MachineCode IntInstruction::machineCode(){
    if(this->is(t_Integer)) return vector<uint8_t>({});
    cout << "Unsupported format for " << this->getName() << endl;
    return vector<uint8_t>({});
}

MachineCode PopInstruction::machineCode(){
    if(this->is(t_Register)){
        Register *reg = dynamic_cast<Register *>(this->getExp());
        if(reg->getName() == "ebp") return vector<uint8_t>({});
        cout << "Unsupported register " << reg->getName() << endl;
    }
    cout << "Unsupported format for " << this->getName() << endl;
    return vector<uint8_t>({});
}

MachineCode RetInstruction::machineCode(){
    if(this->is(t_Integer)) return vector<uint8_t>({});
    cout << "Unsupported format for " << this->getName() << endl;
    return vector<uint8_t>({});
}
