#include <regex>
#include <iostream>
#include <set>
#include "parser.hpp"
#include "log.hpp"

Program *Parser::parser(ifstream *input){
    SymbolTable symbolTable;
    list<Statement *> statements;
    string line;
    uint8_t sectionTextCounter = 0;

    for(int lineNumber = 1 ; getline(*input, line) ; lineNumber++){

        if(Parser::matchSectionText(line)){
            sectionTextCounter++;
            continue;
        }

        if(Parser::matchSectionData(line) || Parser::matchGlobalStart(line)){
            continue;
        }

        // Try to match instruction
        Instruction *lineInstruction;
        lineInstruction = Parser::matchInstruction(line, symbolTable);
        if(lineInstruction != nullptr){
            // Assembly only first text section
            if(sectionTextCounter > 1) lineInstruction->setNotAssemble();
            statements.push_back(lineInstruction);
            continue;
        }

        // Try to match declaration directive
        DeclareStatement *lineDeclaration;
        lineDeclaration = Parser::matchDeclaration(line, symbolTable);
        if(lineDeclaration != nullptr){
            if(sectionTextCounter > 1) lineDeclaration->setNotAssemble();
            statements.push_back(lineDeclaration);
            continue;
        }


        if(sectionTextCounter <= 1) warn("Ignoring statement: " + line);

    }

    return new Program(statements, symbolTable);
}

vector<int32_t> Parser::splitToIntegers(string line, char delimiter){
    vector<int32_t> ret;
    string token;
    istringstream tokenStream(line);
    while(getline(tokenStream, token, delimiter)){
        ret.push_back(atoi(token.c_str()));
    }
    return ret;
}

bool Parser::matchSectionText(string line){
    if(line == "section .text") return true;
    return false;
}

bool Parser::matchSectionData(string line){
    if(line == "section .data") return true;
    return false;
}

bool Parser::matchGlobalStart(string line){
    if(line == "global _start") return true;
    return false;
}

DeclareStatement *Parser::matchDeclaration(string line, SymbolTable &symbolTable){
    regex re("(?:(\\w*):\\s)?dd (\\d*(?:,\\s?\\d*)*)");
    smatch match;
    if(regex_search(line, match, re)){
        Label *label = Parser::matchLabel(match.str(1), symbolTable);
        vector<int32_t> data = Parser::splitToIntegers(match.str(2), ',');

        return new DeclareStatement(label, data);
    }
    return nullptr;
}

Instruction *Parser::matchInstruction(string line, SymbolTable &symbolTable){
    regex binre("(?:(\\w*):\\s)?(\\w*)(?: (dword))?(?: (\\[?\\w*(?:\\+\\w*)?\\]?))?(?:,(\\[?\\w*(?:\\+\\w*)?\\]?))?");
    smatch match;

    // Try to parse a binary expression
    if(regex_search(line, match, binre)){
        // Parse label
        Label *label = Parser::matchLabel(match.str(1), symbolTable);

        // Parse access size
        AccessSize accessSize = Parser::matchAccessSize(match.str(3));

        // Instruction mnemonic
        string instruction = match.str(2);

        // Match cdq
        if(instruction == "cdq"){
            return new CdqInstruction;
        }


        // LHS
        Expression *lhs = Parser::matchExpression(match.str(4), symbolTable);

        // Binary expression
        if(!match.str(5).empty()){
            Expression *rhs = Parser::matchExpression(match.str(5), symbolTable);

            #define MatchBinInstruction(prefix, name)\
            if(instruction == name){\
                return new prefix##Instruction(label, accessSize, lhs, rhs, line);\
            }
            BinExpander(MatchBinInstruction)
        }

        // Unary Expression
        else {
            #define MatchUnInstruction(prefix, name)\
            if(instruction == name){\
                return new prefix##Instruction(label, accessSize, lhs, line);\
            }
            UnExpander(MatchUnInstruction)
        }

    }

    return nullptr;
}

Expression *Parser::matchExpression(string expstr, SymbolTable &symbolTable){
    // Try to match a containsOf
    ContentOfRegister *cofreg = Parser::matchContentOfRegister(expstr);
    if(cofreg != nullptr) return cofreg;

    // Try to match a containsOf
    ContentOfLabel *coflabel = Parser::matchContentOfLabel(expstr, symbolTable);
    if(coflabel != nullptr) return coflabel;

    // Try to match a label add
    LabelAdd *labeladd = Parser::matchLabelAdd(expstr, symbolTable);
    if(labeladd != nullptr) return labeladd;

    // Try to match an atomic expression (register, integer or label)
    AtomicExpression *atexp = Parser::matchAtomicExpression(expstr, symbolTable);
    if(atexp != nullptr) return atexp;

    return nullptr;
}

LabelAdd *Parser::matchLabelAdd(string expstr, SymbolTable &symbolTable){
    regex re("^(\\w*)(?:\\+(\\w*))$");
    smatch match;

    if(regex_search(expstr, match, re)){
        Label *label = matchLabel(match.str(1), symbolTable);
        Integer *offset = matchInteger(match.str(2));
        if(label != nullptr && offset != nullptr){
            return new LabelAdd(label, offset);
        }
    }
    return nullptr;
}

AtomicExpression *Parser::matchAtomicExpression(string expstr, SymbolTable &symbolTable){
    // Try to match a register
    Register *reg = Parser::matchRegister(expstr);
    if(reg != nullptr) return reg;

    // Try to match an integer
    Integer *i = Parser::matchInteger(expstr);
    if(i != nullptr) return i;

    // Try to match a label
    Label *label = Parser::matchLabel(expstr, symbolTable);
    if(label != nullptr) return label;

    return nullptr;
}

AccessSize Parser::matchAccessSize(string asstr){
    if(asstr == "dword") return "dword";
    else if(asstr == "byte") return "byte";
    return "";
}

ContentOfLabel *Parser::matchContentOfLabel(string cofstr, SymbolTable &symbolTable){
    regex re("^\\[(\\w*)(?:\\+(\\w*))?\\]$");
    smatch match;

    if(regex_search(cofstr, match, re)){
        Label *label = matchLabel(match.str(1), symbolTable);
        Integer *offset = matchInteger(match.str(2));
        if(label != nullptr){
            return new ContentOfLabel(label, offset);
        }
    }
    return nullptr;
}

ContentOfRegister *Parser::matchContentOfRegister(string cofstr){
    regex re("^\\[(\\w*)(?:\\+(\\w*))?\\]$");
    smatch match;

    if(regex_search(cofstr, match, re)){
        Register *reg = matchRegister(match.str(1));
        Integer *offset = matchInteger(match.str(2));
        if(reg != nullptr){
            return new ContentOfRegister(reg, offset);
        }
    }
    return nullptr;
}

Register *Parser::matchRegister(string regname){
    set<string> allowed = {"eax", "ebx", "ecx", "edx", "esp", "ebp"};

    if(allowed.find(regname) != allowed.end())
        return new Register(regname);

    return nullptr;
}

Integer *Parser::matchInteger(string intstr){
    char *c = (char *)intstr.c_str();
    char *a = c;
    bool hexa = false;
    long int ret;

    if(*c == '\0') return nullptr;

    if(*c == '-') c++;
    // Hexa
    if(*c == '0' && *(c+1) == 'x') {
        hexa = true;
        c = c+2;
    }

    while(*c != '\0'){
        if(!((*c >= '0' && *c <= '9') || (hexa && (*c >= 'a' && *c <= 'f')) ))
            return nullptr;
        c++;
    }

    ret = hexa ? strtol(a, NULL, 16) : strtol(a, NULL, 10);

    return new Integer(ret);
}

Label *Parser::matchLabel(string labelstr, SymbolTable &symbolTable){
    regex re("^(\\w*)$");
    if(regex_match(labelstr, re)){

        // Add label to symbol table
        if(labelstr != "" && symbolTable.count(labelstr) == 0){
            Label *label = new Label(labelstr);
            symbolTable[labelstr] = label;
            return label;
        }

        // Return label from symbolTable
        return symbolTable[labelstr];
    }
    return nullptr;
}
