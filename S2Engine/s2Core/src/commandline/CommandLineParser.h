// CommandLineParser.h
//
#ifndef COMMANDLINEPARSER_H
#define COMMANDLINEPARSER_H

#include "s2Core_API.h"

#include "CommandLineOption.h"

#include <string>
#include <vector>

class S2CORE_API CommandLineParser
{
public:
	CommandLineParser( int argc, char *argv[] );
	virtual ~CommandLineParser();

	
	std::string lastError() const;
	
	std::vector<CommandLineOption> options() const { return _commandLineOptions; } 

private:
	std::string _lastError;

	// Options found on the command line
	std::vector<CommandLineOption> _commandLineOptions;

	//bool parse(const std::vector<std::string> &arguments);

};


#endif
