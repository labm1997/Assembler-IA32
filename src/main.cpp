#include "expression.hpp"
#include "statement.hpp"
#include "parser.hpp"
#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char **argv){
	ifstream input;
	Program *program;
	
	// Verify arguments
	if(argc < 2) {
		cout << "Usage: ./" << argv[0] << " FILENAME\n";
		return 1;
	}
	
	// Try to open file
	input.open(argv[1]);
	if(!input.is_open()) {
		cout << "Failed to open \"" << argv[1] << "\"\n";
		return 1;
	}
	
	// Parser file
	program = Parser::parser(&input);
	program->prettyPrinter();
	
	return 0;
}
