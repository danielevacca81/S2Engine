// ConsoleFlag.cpp
//
#include "CommandLineOption.h"

std::string              CommandLineOption::name()       const { return _name; }
std::vector<std::string> CommandLineOption::parameters() const { return _parameters; }