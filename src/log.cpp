#include "log.hpp"
#include <iostream>
#include <string>

void warn(string message){
	cout << UI_BOLD_RED << "Warning" << ": " << UI_NORMAL << message << "\n";
}

void error(string message){
	cout << UI_BOLD_RED << "Error" << ": " << UI_NORMAL << message << "\n";
}

void info(string message){
	cout << UI_GREEN << "Information" << ": " << UI_NORMAL << message << "\n";
}
