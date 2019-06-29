#ifndef HEADER_STATEMENT
#define HEADER_STATEMENT

#include <string>
#include <list>
#include <vector>
#include <fstream>
#include <map>

#include "expression.hpp"

using namespace std;

typedef map<string, Label *> SymbolTable;

class Statement {
    protected:
    Label *label;

    public:
    virtual string getObjectCode() = 0;
    virtual void prettyPrinter() = 0;
};

typedef list<Statement *> StatementList;
typedef string AccessSize;

class Instruction : public Statement {
    protected:
    AccessSize indirectAccessSize;
    public:
    virtual string getObjectCode() = 0;
    virtual void prettyPrinter() = 0;
    virtual string getName() = 0;
};

class Program {
    private:
    // List of program statements
    StatementList statements;

    // Hash map to labels
    SymbolTable symbolTable;

    public:
    Program(StatementList, SymbolTable);
    void prettyPrinter();

};

class UnaryInstruction : public Instruction {
    private:
    Expression *exp;
    public:
    UnaryInstruction(Label *, AccessSize, Expression *);
    void prettyPrinter();
    virtual string getName() = 0;
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
    macro(Call, "call")\
    macro(Int, "int")\

/* Macro that declares a class for an unary instruction */
#define UnInstruction(prefix, name)\
class prefix##Instruction : public UnaryInstruction {\
    public:\
    using UnaryInstruction::UnaryInstruction;\
    string getObjectCode() { return ""; };\
    string getName() { return name; };\
};

/* Create classes for all unary instructions */
UnExpander(UnInstruction)

class BinaryInstruction : public Instruction {
    private:
    Expression *lhs, *rhs;
    public:
    BinaryInstruction(Label *, AccessSize, Expression *, Expression *);
    void prettyPrinter();
    virtual string getName() = 0;
};

/* Macro that expands a given macro for all binary instructions */
#define BinExpander(macro)\
    macro(Add, "add")\
    macro(Sub, "sub")\
    macro(Cmp, "cmp")\
    macro(Mov, "mov")\

/* Macro that declares a class for a binary instruction */
#define BinInstruction(prefix, name)\
class prefix##Instruction : public BinaryInstruction {\
    public:\
    using BinaryInstruction::BinaryInstruction;\
    string getObjectCode() { return ""; };\
    string getName() { return name; };\
};\

/* Create classes for all binary instructions */
BinExpander(BinInstruction)

#endif
