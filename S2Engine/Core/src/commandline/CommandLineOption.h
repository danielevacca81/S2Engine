// CommandLineOption.h
//
#ifndef COMMANDLINEOPTION_H
#define COMMANDLINEOPTION_H

#include "Core_API.h"

#include <string>
#include <vector>

class CORE_API CommandLineOption
{
public:
	CommandLineOption( const std::string &name, const std::vector<std::string> &parameters = std::vector<std::string>() )
	: _name( name )
	, _parameters(parameters)
	{}
	
	~CommandLineOption()
	{};

	std::string              name()       const;
	std::vector<std::string> parameters() const;

private:
	std::string              _name;
	std::vector<std::string> _parameters;
};


#endif