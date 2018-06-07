// ConsoleFlag.cpp
//
#include "CommandLineOption.h"

namespace s2 {

std::string              CommandLineOption::name()       const { return _name; }
std::vector<std::string> CommandLineOption::parameters() const { return _parameters; }

}