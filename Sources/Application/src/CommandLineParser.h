// CommandLineParser.h
//
#ifndef COMMANDLINEPARSER_H
#define COMMANDLINEPARSER_H

#include "Application_API.h"

#include <string>
#include <vector>

namespace s2 {

class APPLICATION_API CommandLineParser
{
public:
	struct CommandLineOption 
	{
		std::string              name;
		std::vector<std::string> parameter;
	};

public:
	static std::vector<CommandLineOption> parse( int argc, char* argv[], std::string* parseErrors = nullptr );
};

}

#endif
