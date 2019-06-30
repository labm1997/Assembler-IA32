#ifndef HEADER_PARSER
#define HEADER_PARSER

#include "statement.hpp"
#include "expression.hpp"

class Parser {
    private:
    static AddExpression *matchAddExpression(string, SymbolTable&);
    static AtomicExpression *matchAtomicExpression(string, SymbolTable&);
    static Instruction *matchInstruction(string, SymbolTable&);
    static DeclareStatement *matchDeclaration(string, SymbolTable&);
    static ContentOf *matchContentOf(string, SymbolTable&);
    static AccessSize matchAccessSize(string);
    static Expression *matchExpression(string, SymbolTable&);
    static Register *matchRegister(string);
    static Integer *matchInteger(string);
    static Label *matchLabel(string, SymbolTable&);

    static vector<int32_t> splitToIntegers(string, char);

    public:
    static Program* parser(ifstream *);
};

#endif