#include <regex>
#include <iostream>
#include "parser.hpp"

Program *Parser::parser(ifstream *input){
	SymbolTable symbolTable;
	list<Statement *> statements;
	string line;

	for(int lineNumber = 1 ; getline(*input, line) ; lineNumber++){
		Instruction *lineInstruction;
		lineInstruction = Parser::matchInstruction(line, symbolTable);
		if(lineInstruction != nullptr)
		    statements.push_back(lineInstruction);
	}

	return new Program(statements, symbolTable);
}

Instruction *Parser::matchInstruction(string line, SymbolTable &symbolTable){
	Instruction *ret = nullptr;
	regex re1("(?:(\\w*):\\s)?(\\w*) (?:(\\w*)\\s)?(\\[?\\w*\\]?),(\\[?\\w*\\]?)");
	smatch match;

	if(regex_search(line, match, re1)){
	    // Parse label
	    Label *label = Parser::matchLabel(match.str(1), symbolTable);

		string instruction = match.str(2);
		if(instruction == "add"){
		    AccessSize accessSize = Parser::matchAccessSize(match.str(3));
		    Expression *lhs = Parser::matchExpression(match.str(4), symbolTable);
		    Expression *rhs = Parser::matchExpression(match.str(5), symbolTable);
			ret = new AddInstruction(label, accessSize, lhs, rhs);
			cout << "Add instruction found" << endl;
		}
		else {
			cout << "Unsupported instruction " << instruction << endl;
		}
	}

	return ret;
}

Expression *Parser::matchExpression(string expstr, SymbolTable &symbolTable){
    // Try to match a containsOf
    ContentOf *cof = Parser::matchContentOf(expstr, symbolTable);
    if(cof != nullptr) return cof;

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
    return "";
}

ContentOf *Parser::matchContentOf(string cofstr, SymbolTable &symbolTable){
    regex re("^\\[(\\w*)\\]$");
    smatch match;
    if(regex_search(cofstr, match, re)){
        /* !FIXME: Only one content of is allowed,
         should we create a dedicated matchContentOfExpression */
        return new ContentOf(matchExpression(match.str(1), symbolTable));
    }
    return nullptr;
}

Register *Parser::matchRegister(string regname){
    if(regname == "eax"){
        return new Register("eax");
    }
    else if(regname == "ebx"){
        return new Register("ebx");
    }
    else {
        return nullptr;
    }
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
