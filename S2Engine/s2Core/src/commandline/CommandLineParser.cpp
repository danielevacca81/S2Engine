// ConsoleFlag.cpp
//
#include "CommandLineParser.h"

#include "utils/String.h"

#include <sstream>
#include <algorithm>

namespace s2 {

// ------------------------------------------------------------------------------------------------
CommandLineParser::CommandLineParser( int argc, char *argv[] )
{
	const std::string token( "-" );

	if( argc > 1 )
	{
		// build the command line as a whole string
		std::string cmdLine;
		for( int i=1; i < argc; ++i )
		{
			const std::string s( argv[i] );

			if( String::startsWith( s, token ) )
				cmdLine += s + " ";
			else
				cmdLine += "\"" + s + "\""; // surround options with quotes
		}

		// tokenize it using _DASH_ as separator:
		// enumerates the options
		const std::vector<std::string> options = String::tokenize<std::string>( cmdLine, token );

		if( options.empty() )
			_lastError = "No option found";

		// for each option
		for( size_t i=0; i < options.size(); ++i )
		{
			if( options[i].size() <= 1 )
			{
				std::stringstream ss;
				ss << "Invalid command line argument " << i + 1 << ";" << std::endl;
				_lastError += ss.str();
				continue; // invalid or empty option
			}

			// find option position in the command line
			size_t optStart    = cmdLine.find( options[i] );
			size_t optEnd      = i + 1 < options.size() ? cmdLine.find( options[i + 1], optStart + 1 ) : cmdLine.size() - 1;
			std::string option = cmdLine.substr( optStart, optEnd - optStart );


			if( option.empty() )
				continue;

			// split options using " as separator char
			const std::vector<std::string> tmp = String::split<std::string>( option, "\"" );

			// first value is the option name, the rest are the option parameters
			std::string optionName( tmp[0].substr( token.size() ) ); // skip token chars
			optionName.erase( std::remove( optionName.begin(), optionName.end(), ' ' ), optionName.end() ); // remove trailing spaces from the option name

			const std::vector<std::string> optionParameters( tmp.begin() + 1, tmp.end() );

			_commandLineOptions.push_back( CommandLineOption( optionName, optionParameters ) );
		}
	}
}

// ------------------------------------------------------------------------------------------------
CommandLineParser::~CommandLineParser()
{}

// ------------------------------------------------------------------------------------------------
std::string CommandLineParser::lastError() const
{
	return _lastError;
}

}