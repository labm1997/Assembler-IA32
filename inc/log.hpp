#ifndef HEADER_LOG
#define HEADER_LOG

#include "log.hpp"
#include <iostream>
#include <string>

using namespace std;

#define UI_BOLD_RED_BACK_RED "\x1B[1;37;41m"
#define UI_BOLD_RED "\x1B[1;31m"
#define UI_BOLD "\x1B[1m"
#define UI_NORMAL  "\x1B[0m"
#define UI_GREEN  "\x1b[32m"

void warn(string message);
void error(string message);
void info(string message);

#endif
