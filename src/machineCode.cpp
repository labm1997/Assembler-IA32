#include "statement.hpp"
#include <iostream>

MachineCode DeclareStatement::machineCode(){
    return "Missing implementation!\n";
}

MachineCode AddInstruction::machineCode(){
    if(this->is(t_Register, t_ContentOf)) return "Missing implementation!\n";
    if(this->is(t_Register, t_Integer)) return "Missing implementation!\n";
    cout << "Unsupported format for " << this->getName() << endl;
    return "Missing implementation!\n";
}

MachineCode SubInstruction::machineCode(){
    if(this->is(t_Register, t_ContentOf)) return "Missing implementation!\n";
    cout << "Unsupported format for " << this->getName() << endl;
    return "Missing implementation!\n";
}

MachineCode ShlInstruction::machineCode(){
    if(this->is(t_Register, t_Integer)) return "Missing implementation!\n";
    cout << "Unsupported format for " << this->getName() << endl;
    return "Missing implementation!\n";
}

MachineCode CmpInstruction::machineCode(){
    if(this->is(t_Register, t_Integer)) return "Missing implementation!\n";
    cout << "Unsupported format for " << this->getName() << endl;
    return "Missing implementation!\n";
}

MachineCode MovInstruction::machineCode(){
    if(this->is(t_Register, t_ContentOf)){
        Register *reg = dynamic_cast<Register *>(this->getLhs());
        ContentOf *contentof = dynamic_cast<ContentOf *>(this->getRhs());
        if(reg->getName() == "eax") return "Missing implementation!\n";
        if(reg->getName() == "ebx") return "Missing implementation!\n";
        if((reg->getName() == "ecx" || reg->getName() == "edx") &&
            contentof->getExpression()->isType(t_AddExpression))
            return "Missing implementation!\n";
    }

    if(this->is(t_ContentOf, t_Register)){
        Register *reg = dynamic_cast<Register *>(this->getRhs());
        if(reg->getName() == "eax") return "Missing implementation!\n";
        if(reg->getName() == "ebx") return "Missing implementation!\n";
    }

    if(this->is(t_Register, t_Integer)) return "Missing implementation!\n";

    else if(this->is(t_Register, t_Register)) return "Missing implementation!\n";

    cout << "Unsupported format for " << this->getName() << endl;
    return "Missing implementation!\n";
}

MachineCode MultInstruction::machineCode(){
    if(this->is(t_ContentOf)) return "Missing implementation!\n";

    cout << "Unsupported format for " << this->getName() << endl;
    return "Missing implementation!\n";
}

MachineCode DivInstruction::machineCode(){
    if(this->is(t_ContentOf)) return "Missing implementation!\n";
    cout << "Unsupported format for " << this->getName() << endl;
    return "Missing implementation!\n";
}

MachineCode JmpInstruction::machineCode(){
    // !FIXME: Relative jump, it will only work if address to jump is not far
    if(this->is(t_Label)) return "Missing implementation!\n";
    cout << "Unsupported format for " << this->getName() << endl;
    return "Missing implementation!\n";
}

MachineCode JlInstruction::machineCode(){
    if(this->is(t_Label)) return "Missing implementation!\n";
    cout << "Unsupported format for " << this->getName() << endl;
    return "Missing implementation!\n";
}

MachineCode JgInstruction::machineCode(){
    if(this->is(t_Label)) return "Missing implementation!\n";
    cout << "Unsupported format for " << this->getName() << endl;
    return "Missing implementation!\n";
}

MachineCode JeInstruction::machineCode(){
    if(this->is(t_Label)) return "Missing implementation!\n";
    cout << "Unsupported format for " << this->getName() << endl;
    return "Missing implementation!\n";
}

MachineCode PushInstruction::machineCode(){
    if(this->is(t_Label)) return "Missing implementation!\n";
    if(this->is(t_ContentOf)) return "Missing implementation!\n";
    if(this->is(t_Register)) return "Missing implementation!\n";
    cout << "Unsupported format for " << this->getName() << endl;
    return "Missing implementation!\n";
}

MachineCode CallInstruction::machineCode(){
    if(this->is(t_Label)) return "Missing implementation!\n";
    cout << "Unsupported format for " << this->getName() << endl;
    return "Missing implementation!\n";
}

MachineCode IntInstruction::machineCode(){
    if(this->is(t_Integer)) return "Missing implementation!\n";
    cout << "Unsupported format for " << this->getName() << endl;
    return "Missing implementation!\n";
}

MachineCode PopInstruction::machineCode(){
    if(this->is(t_Register)){
        Register *reg = dynamic_cast<Register *>(this->getExp());
        if(reg->getName() == "ebp") return "Missing implementation!\n";
        cout << "Unsupported register " << reg->getName() << endl;
    }
    cout << "Unsupported format for " << this->getName() << endl;
    return "Missing implementation!\n";
}

MachineCode RetInstruction::machineCode(){
    if(this->is(t_Integer)) return "Missing implementation!\n";
    cout << "Unsupported format for " << this->getName() << endl;
    return "Missing implementation!\n";
}
