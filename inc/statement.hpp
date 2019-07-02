#ifndef HEADER_STATEMENT
#define HEADER_STATEMENT

#include <string>
#include <list>
#include <vector>
#include <fstream>
#include <map>
#include <stdint.h>

#include "expression.hpp"

using namespace std;

typedef map<string, Label *> SymbolTable;
typedef string MachineCode;

class ObjectCode {
    private:
    MachineCode text;
    MachineCode data;

    public:
    ObjectCode(MachineCode text, MachineCode data) : text(text), data(data) {};
    MachineCode getText() { return text; }
    MachineCode getData() { return data; }
};

class Statement {
    protected:
    Label *label;

    public:
    virtual MachineCode machineCode() = 0;
    virtual void prettyPrinter() = 0;
    Label *getLabel() {return label;};
    virtual string getSection() = 0;
    virtual uint32_t size() = 0;
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
    string getObjectCode();

};

class UnaryInstruction : public Instruction {
    protected:
    Expression *exp;
    public:
    UnaryInstruction(Label *, AccessSize, Expression *);
    virtual MachineCode machineCode() = 0;
    void prettyPrinter();
    virtual string getName() = 0;
    Expression *getExp() { return exp ; };
    bool is(ExpressionType t){ return this->exp->isType(t); };
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
    public:
    BinaryInstruction(Label *, AccessSize, Expression *, Expression *);
    virtual MachineCode machineCode() = 0;
    void prettyPrinter();
    virtual string getName() = 0;
    Expression *getLhs() { return lhs ; };
    Expression *getRhs() { return rhs ; };
    bool is(ExpressionType t1, ExpressionType t2){
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