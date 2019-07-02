#include "statement.hpp"
#include <iostream>
#include <sstream>

MachineCode DeclareStatement::machineCode(){
    vector<uint8_t> ret;
    for(auto it = this->data.begin() ; it != this->data.end() ; ++it){
        ret.push_back((uint8_t)((*it) & 0xff));
        ret.push_back((uint8_t)(((*it) & 0xff00) >> 8));
        ret.push_back((uint8_t)(((*it) & 0xff0000) >> 16));
        ret.push_back((uint8_t)(((*it) & 0xff000000) >> 24));
    }
    return ret;
}

MachineCode AddInstruction::machineCode(){
    InstructionCode code;
    if(this->is(t_Register, t_ContentOfLabel)) {
        Register *reg = dynamic_cast<Register *>(this->getLhs());
        ContentOfLabel *contentof = dynamic_cast<ContentOfLabel *>(this->getRhs());
        code.setOpcode(0x03);
        if(reg->getName() == "eax"){
            code.setModRM(0x05);
            code.setImmediate(contentof->getAddress());
            return code.getCode();
        }

    }
    if(this->is(t_Register, t_Integer)) {
        int32_t value = dynamic_cast<Integer *>(this->getRhs())->getValue();
        code.setOpcode(0x83);
        code.setModRM(0xc1);
        code.set8bitsImmediate((uint8_t)value);
        return code.getCode();
    }
    cout << "Unsupported format for " << this->getName() << endl;
    return vector<uint8_t>({});
}

MachineCode SubInstruction::machineCode(){
    InstructionCode code;
    if(this->is(t_Register, t_ContentOfLabel)) {
        Register *reg = dynamic_cast<Register *>(this->getLhs());
        ContentOfLabel *contentof = dynamic_cast<ContentOfLabel *>(this->getRhs());

        code.setOpcode(0x2b);
        if(reg->getName() == "eax"){
            code.setModRM(0x05);
            code.setImmediate(contentof->getAddress());
            return code.getCode();
        }

    }
    cout << "Unsupported format for " << this->getName() << endl;
    return vector<uint8_t>({});
}

MachineCode ShlInstruction::machineCode(){
    InstructionCode code;
    if(this->is(t_Register, t_Integer)) {
        Register *reg = dynamic_cast<Register *>(this->getLhs());
        Integer *integer = dynamic_cast<Integer *>(this->getRhs());
        code.setOpcode(0xc1);
        if(reg->getName() == "edx"){
            code.setModRM(0xe2);
            code.set8bitsImmediate(integer->getValue());
            return code.getCode();
        }
    }
    cout << "Unsupported format for " << this->getName() << endl;
    return vector<uint8_t>({});
}

MachineCode CmpInstruction::machineCode(){
    InstructionCode code;
    if(this->is(t_Register, t_Integer)) {
        Register *reg = dynamic_cast<Register *>(this->getLhs());
        Integer *integer = dynamic_cast<Integer *>(this->getRhs());
        code.setOpcode(0x83);
        if(reg->getName() == "eax"){
            code.setModRM(0xf8);
            code.set8bitsImmediate(integer->getValue());
            return code.getCode();
        }
    }
    cout << "Unsupported format for " << this->getName() << endl;
    return vector<uint8_t>({});
}

MachineCode MovInstruction::machineCode(){
    InstructionCode code;
    if(this->indirectAccessSize == "byte"){
        if(this->is(t_ContentOfRegister, t_Integer)){
            ContentOfRegister *contentof = dynamic_cast<ContentOfRegister *>(this->getLhs());
            Integer *offset = contentof->getOffset();
            Integer *integer = dynamic_cast<Integer *>(this->getRhs());
            code.setOpcode(0xc6);
            code.setModRM(0x45);
            code.set8bitsDisplacement(offset->getValue());
            code.set8bitsImmediate(integer->getValue());
            return code.getCode();
        }
    }

    else if(this->is(t_Register, t_ContentOfLabel)){
        Register *reg = dynamic_cast<Register *>(this->getLhs());
        ContentOfLabel *contentof = dynamic_cast<ContentOfLabel *>(this->getRhs());
        if(reg->getName() == "eax") {
            code.setOpcode(0xa1);
            code.setDisplacement(contentof->getAddress());
            return code.getCode();
        }
        if(reg->getName() == "ebx") {
            code.setOpcode(0x8b);
            code.setModRM(0x1d);
            code.setDisplacement(contentof->getAddress());
            return code.getCode();
        }
    }

    else if(this->is(t_Register, t_ContentOfRegister)){
        Register *reg = dynamic_cast<Register *>(this->getLhs());
        ContentOfRegister *contentof = dynamic_cast<ContentOfRegister *>(this->getRhs());
        Integer *integer = contentof->getOffset();

        // !FIXME: Only if register1 is ebp
        if((reg->getName() == "ecx" || reg->getName() == "edx")){
            code.setOpcode(0x8b);
            if(reg->getName() == "ecx") code.setModRM(0x4d);
            else if(reg->getName() == "edx") code.setModRM(0x55);
            if(integer != nullptr)
                code.set8bitsDisplacement(integer->getValue());
            else
                code.set8bitsDisplacement(0x00);
            return code.getCode();
        }

    }

    else if(this->is(t_ContentOfLabel, t_Register)){
        ContentOfLabel *contentof = dynamic_cast<ContentOfLabel *>(this->getLhs());
        Register *reg = dynamic_cast<Register *>(this->getRhs());
        if(reg->getName() == "eax") {
            code.setOpcode(0xa3);
            code.setDisplacement(contentof->getAddress());
            return code.getCode();
        }
        if(reg->getName() == "ebx") {
            code.setOpcode(0x89);
            code.setModRM(0x1d);
            code.setDisplacement(contentof->getAddress());
            return code.getCode();
        }
    }

    else if(this->is(t_Register, t_Integer)) {
        Register *reg = dynamic_cast<Register *>(this->getLhs());
        Integer *integer = dynamic_cast<Integer *>(this->getRhs());
        if(reg->getName() == "eax"){
            code.setOpcode(0xb8);
            code.setImmediate(integer->getValue());
            return code.getCode();
        }
        else if(reg->getName() == "ebx"){
            code.setOpcode(0xbb);
            code.setImmediate(integer->getValue());
            return code.getCode();
        }
        else if(reg->getName() == "edx"){
            code.setOpcode(0xba);
            code.setImmediate(integer->getValue());
            return code.getCode();
        }
    }

    else if(this->is(t_Register, t_Register)) {
        Register *lhs = dynamic_cast<Register *>(this->getLhs());
        Register *rhs = dynamic_cast<Register *>(this->getRhs());
        if(lhs->getName() == "ebp" && rhs->getName() == "esp")
            return vector<uint8_t>({0x89, 0xe5});
        if(lhs->getName() == "ecx" && rhs->getName() == "ebp"){
            code.setOpcode(0x89);
            code.setModRM(0xe9);
            return code.getCode();
        }
    }

    cout << "Unsupported format for " << this->getName() << endl;
    return vector<uint8_t>({});
}

MachineCode MultInstruction::machineCode(){
    InstructionCode code;
    if(this->is(t_ContentOfLabel)) {
        ContentOfLabel *contentof = dynamic_cast<ContentOfLabel *>(this->getExp());
        code.setOpcode(0xf7);
        code.setModRM(0x25);
        code.setDisplacement(contentof->getAddress());
        return code.getCode();
    }

    cout << "Unsupported format for " << this->getName() << endl;
    return vector<uint8_t>({});
}

MachineCode DivInstruction::machineCode(){
    InstructionCode code;
    if(this->is(t_ContentOfLabel)) {
        ContentOfLabel *contentof = dynamic_cast<ContentOfLabel *>(this->getExp());
        code.setOpcode(0xf7);
        code.setModRM(0x35);
        code.setDisplacement(contentof->getAddress());
        return code.getCode();
    }
    cout << "Unsupported format for " << this->getName() << endl;
    return vector<uint8_t>({});
}

MachineCode JmpInstruction::machineCode(){
    InstructionCode code;
    // !FIXME: Relative jump, it will only work if address to jump is not far
    if(this->is(t_Label)){
        Label *label = dynamic_cast<Label *>(this->getExp());
        code.setOpcode(0xeb);
        code.set8bitsImmediate(label->getNotBiasedAddress()-this->getAddress()-this->size());
        return code.getCode();
    }
    cout << "Unsupported format for " << this->getName() << endl;
    return vector<uint8_t>({});
}

MachineCode JlInstruction::machineCode(){
    InstructionCode code;
    if(this->is(t_Label)){
        Label *label = dynamic_cast<Label *>(this->getExp());
        code.setOpcode(0x7c);
        code.set8bitsImmediate(label->getNotBiasedAddress()-this->getAddress()-this->size());
        return code.getCode();
    }
    cout << "Unsupported format for " << this->getName() << endl;
    return vector<uint8_t>({});
}

MachineCode JgInstruction::machineCode(){
    InstructionCode code;
    if(this->is(t_Label)){
        Label *label = dynamic_cast<Label *>(this->getExp());
        code.setOpcode(0x7f);
        code.set8bitsImmediate(label->getNotBiasedAddress()-this->getAddress()-this->size());
        return code.getCode();
    }
    cout << "Unsupported format for " << this->getName() << endl;
    return vector<uint8_t>({});
}

MachineCode JeInstruction::machineCode(){
    InstructionCode code;
    if(this->is(t_Label)){
        Label *label = dynamic_cast<Label *>(this->getExp());
        code.setOpcode(0x74);
        code.set8bitsImmediate(label->getNotBiasedAddress()-this->getAddress()-this->size());
        return code.getCode();
    }
    cout << "Unsupported format for " << this->getName() << endl;
    return vector<uint8_t>({});
}

MachineCode PushInstruction::machineCode(){
    InstructionCode code;
    if(this->is(t_Label)) {
        Label *label = dynamic_cast<Label *>(this->getExp());
        code.setOpcode(0x68);
        code.setImmediate(label->getAddress());
        return code.getCode();
    }
    if(this->is(t_LabelAdd)) {
        LabelAdd *label = dynamic_cast<LabelAdd *>(this->getExp());
        code.setOpcode(0x68);
        code.setImmediate(label->getAddress());
        return code.getCode();
    }
    if(this->is(t_ContentOfLabel)) {
        ContentOfLabel *contentof = dynamic_cast<ContentOfLabel *>(this->getExp());
        code.setOpcode(0xff);
        code.setModRM(0x35);
        code.setImmediate(contentof->getAddress());
        return code.getCode();
    }
    if(this->is(t_Register)) {
        return vector<uint8_t>({0x55});
    }
    cout << "Unsupported format for " << this->getName() << endl;
    return vector<uint8_t>({});
}

MachineCode CallInstruction::machineCode(){
    InstructionCode code;
    if(this->is(t_Label)){
        Label *label = dynamic_cast<Label *>(this->getExp());
        code.setOpcode(0xe8);
        code.setImmediate(label->getNotBiasedAddress()-this->getAddress()-this->size());
        return code.getCode();
    }
    cout << "Unsupported format for " << this->getName() << endl;
    return vector<uint8_t>({});
}

MachineCode IntInstruction::machineCode(){
    InstructionCode code;
    if(this->is(t_Integer)) {
        Integer *integer = dynamic_cast<Integer *>(this->getExp());
        code.setOpcode(0xcd);
        code.set8bitsImmediate(integer->getValue());
        return code.getCode();
    }
    cout << "Unsupported format for " << this->getName() << endl;
    return vector<uint8_t>({});
}

MachineCode PopInstruction::machineCode(){
    InstructionCode code;
    if(this->is(t_Register)){
        Register *reg = dynamic_cast<Register *>(this->getExp());
        if(reg->getName() == "ebp") {
            code.setOpcode(0x5d);
            return code.getCode();
        }
        cout << "Unsupported register " << reg->getName() << endl;
    }
    cout << "Unsupported format for " << this->getName() << endl;
    return vector<uint8_t>({});
}

MachineCode RetInstruction::machineCode(){
    InstructionCode code;
    if(this->is(t_Integer)) {
        Integer *integer = dynamic_cast<Integer *>(this->getExp());
        code.setOpcode(0xc2);
        code.set16bitsImmediate(integer->getValue());
        return code.getCode();
    }
    cout << "Unsupported format for " << this->getName() << endl;
    return vector<uint8_t>({});
}

MachineCode IncInstruction::machineCode(){
    InstructionCode code;
    if(this->is(t_Register)) {
        Register *reg = dynamic_cast<Register *>(this->getExp());
        if(reg->getName() == "ebp"){
            code.setOpcode(0x45);
            return code.getCode();
        }
    }
    cout << "Unsupported format for " << this->getName() << endl;
    return vector<uint8_t>({});
}

MachineCode DecInstruction::machineCode(){
    InstructionCode code;
    if(this->is(t_Register)) {
        Register *reg = dynamic_cast<Register *>(this->getExp());
        if(reg->getName() == "ebp"){
            code.setOpcode(0x4d);
            return code.getCode();
        }
    }
    cout << "Unsupported format for " << this->getName() << endl;
    return vector<uint8_t>({});
}
