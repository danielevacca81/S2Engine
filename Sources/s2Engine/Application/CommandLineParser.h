// CommandLineParser.h
//
#ifndef COMMANDLINEPARSER_H
#define COMMANDLINEPARSER_H

#include "s2Engine_API.h"

#include <string>
#include <vector>

namespace s2 {

class S2ENGINE_API CommandLineParser
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
