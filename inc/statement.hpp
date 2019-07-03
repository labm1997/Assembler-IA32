#ifndef HEADER_STATEMENT
#define HEADER_STATEMENT

#include <string>
#include <list>
#include <vector>
#include <fstream>
#include <map>
#include <stdint.h>

#include "expression.hpp"

#define START_TEXT_ADDRESS 0x08049000
#define START_DATA_ADDRESS 0x0804a000

using namespace std;

typedef map<string, Label *> SymbolTable;
typedef vector<uint8_t> MachineCode;

class InstructionCode {
    private:
    vector<uint8_t> opcode;
    vector<uint8_t> modRM;
    vector<uint8_t> sib;
    vector<uint8_t> displacement;
    vector<uint8_t> immediate;
    public:
    void setOpcode(uint8_t op) {
        this->opcode.clear();
        this->opcode.push_back(op);
    };
    void setOpcode(uint8_t op1, uint8_t op2) {
        this->opcode.clear();
        this->opcode.push_back(op1);
        this->opcode.push_back(op2);
    };
    void setModRM(uint8_t mod, uint8_t reg, uint8_t rm) {
        this->modRM.clear();
        this->modRM.push_back(((mod & 0xfc) << 6) | ((reg & 0xf8) << 3) | (rm & 0xf8));
    };
    void setModRM(uint8_t modRm) {
        this->modRM.clear();
        this->modRM.push_back(modRm);
    };
    void setSib(uint8_t scale, uint8_t index, uint8_t base) {
        this->sib.clear();
        this->sib.push_back(((scale & 0xfc) << 6) | ((index & 0xf8) << 3) | (base & 0xf8));
    };
    void setDisplacement(uint32_t displ) {
        this->displacement.clear();
        this->displacement.push_back((uint8_t)(displ & 0xff));
        this->displacement.push_back((uint8_t)((displ & 0xff00) >> 8));
        this->displacement.push_back((uint8_t)((displ & 0xff0000) >> 16));
        this->displacement.push_back((uint8_t)((displ & 0xff000000) >> 24));
    };
    void set8bitsDisplacement(uint8_t displ) {
        this->displacement.clear();
        this->displacement.push_back(displ);
    };
    void set8bitsImmediate(uint8_t immed) {
        this->immediate.clear();
        this->immediate.push_back(immed);
    };
    void set16bitsImmediate(uint16_t immed) {
        this->immediate.clear();
        this->immediate.push_back((uint8_t)(immed & 0xff));
        this->immediate.push_back((uint8_t)((immed & 0xff00) >> 8));
    };
    void setImmediate(uint32_t immed) {
        this->immediate.clear();
        this->immediate.push_back((uint8_t)(immed & 0xff));
        this->immediate.push_back((uint8_t)((immed & 0xff00) >> 8));
        this->immediate.push_back((uint8_t)((immed & 0xff0000) >> 16));
        this->immediate.push_back((uint8_t)((immed & 0xff000000) >> 24));
    };
    vector<uint8_t> getCode(){
        vector<uint8_t> ret;
        ret.insert(ret.end(), opcode.begin(), opcode.end());
        ret.insert(ret.end(), modRM.begin(), modRM.end());
        ret.insert(ret.end(), sib.begin(), sib.end());
        ret.insert(ret.end(), displacement.begin(), displacement.end());
        ret.insert(ret.end(), immediate.begin(), immediate.end());
        return ret;
    };
};

class ObjectCode {
    private:
    MachineCode *text;
    MachineCode *data;

    public:
    ObjectCode(MachineCode *text, MachineCode *data) : text(text), data(data) {};
    MachineCode *getText() { return text; }
    MachineCode *getData() { return data; }
};

class Statement {
    protected:
    Label *label;
    uint32_t address;
    bool toAssemble = true;

    public:
    virtual MachineCode machineCode() = 0;
    virtual void prettyPrinter() = 0;
    Label *getLabel() {return label;};
    uint32_t getAddress() {return address;};
    void setAddress(uint32_t address) { this->address = address; };
    virtual string getSection() = 0;
    virtual uint32_t size() = 0;
    void setNotAssemble(){ this->toAssemble = false; };
    bool assemblable(){ return this->toAssemble; };
};

typedef list<Statement *> StatementList;
typedef string AccessSize;

class Instruction : public Statement {
    protected:
    AccessSize indirectAccessSize;
    public:
    virtual MachineCode machineCode() = 0;
    virtual void prettyPrinter() = 0;
    virtual string getName() = 0;
    string getSection(){ return "text"; };
    virtual uint32_t size() = 0;
};

class DeclareStatement : public Statement {
    private:
    vector<int32_t> data;

    public:
    DeclareStatement(Label *, vector<int32_t>);
    MachineCode machineCode();
    void prettyPrinter();
    string getSection(){ return "data"; };
    uint32_t getDataLength() { return this->data.size(); };
    uint32_t size();
};

class Program {
    private:
    // List of program statements
    StatementList statements;

    // Hash map to labels
    SymbolTable symbolTable;

    // First passage algorithm
    void firstPassage();

    // Second passage algorithm
    ObjectCode secondPassage();

    public:
    Program(StatementList, SymbolTable);
    void prettyPrinter();
    ObjectCode getObjectCode();
    void saveExecutable();

};

class UnaryInstruction : public Instruction {
    protected:
    Expression *exp;
    string line;
    public:
    UnaryInstruction(Label *, AccessSize, Expression *, string line);
    virtual MachineCode machineCode() = 0;
    void prettyPrinter();
    virtual string getName() = 0;
    Expression *getExp() { return exp ; };
    bool is(ExpressionType t){
        if(this->exp == nullptr) return false;
        return this->exp->isType(t);
    };
    virtual uint32_t size() = 0;
};

/* Macro that expands a given macro for all unary instructions */
#define UnExpander(macro)\
    macro(Mult, "mul")\
    macro(Div, "div")\
    macro(Jmp, "jmp")\
    macro(Jl, "jl")\
    macro(Jg, "jg")\
    macro(Je, "je")\
    macro(Push, "push")\
    macro(Pop, "pop")\
    macro(Call, "call")\
    macro(Ret, "ret")\
    macro(Int, "int")\
    macro(Inc, "inc")\
    macro(Dec, "dec")\

/* Macro that declares a class for an unary instruction */
#define UnInstruction(prefix, name)\
class prefix##Instruction : public UnaryInstruction {\
    public:\
    using UnaryInstruction::UnaryInstruction;\
    MachineCode machineCode();\
    string getName() { return name; };\
    uint32_t size();\
};

/* Create classes for all unary instructions */
UnExpander(UnInstruction);

class BinaryInstruction : public Instruction {
    protected:
    Expression *lhs, *rhs;
    string line;
    public:
    BinaryInstruction(Label *, AccessSize, Expression *, Expression *, string line);
    virtual MachineCode machineCode() = 0;
    void prettyPrinter();
    virtual string getName() = 0;
    Expression *getLhs() { return lhs ; };
    Expression *getRhs() { return rhs ; };
    bool is(ExpressionType t1, ExpressionType t2){
        if(this->lhs == nullptr || this->rhs == nullptr) return false;
        return this->lhs->isType(t1) && this->rhs->isType(t2);
    };
    virtual uint32_t size() = 0;
};

/* Macro that expands a given macro for all binary instructions */
#define BinExpander(macro)\
    macro(Add, "add")\
    macro(Sub, "sub")\
    macro(Shl, "shl")\
    macro(Cmp, "cmp")\
    macro(Mov, "mov")\

/* Macro that declares a class for a binary instruction */
#define BinInstruction(prefix, name)\
class prefix##Instruction : public BinaryInstruction {\
    public:\
    using BinaryInstruction::BinaryInstruction;\
    MachineCode machineCode();\
    string getName() { return name; };\
    uint32_t size();\
};\

/* Create classes for all binary instructions */
BinExpander(BinInstruction);

#endif