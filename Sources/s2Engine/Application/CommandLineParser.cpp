// ConsoleFlag.cpp
//
#include "CommandLineParser.h"


#include <sstream>
#include <algorithm>

using namespace s2;

static const std::string kOptionSeparator{ "-" };

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
static bool startsWith( const std::string& s, const std::string& key )
{
	return s.find_first_of( key ) == 0;
}
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------
std::vector<CommandLineParser::CommandLineOption> CommandLineParser::parse( int argc, char *argv[], std::string *parseErrors )
{
	if( argc <= 1 )
		return {};

	if( parseErrors )
		parseErrors->clear();

	// build the command line as a whole string
	std::string cmdLine;
	for( int i=1; i < argc; ++i )
	{
		const std::string s( argv[i] );

		if( startsWith( s, kOptionSeparator ) )
			cmdLine += s + " ";
		else
			cmdLine += "\"" + s + "\""; // surround options with quotes
	}

#if 0
	// tokenize it using _DASH_ as separator:
	// enumerates the options
	const std::vector<std::string> options = String::tokenize<std::string>( cmdLine, kOptionSeparator );

	if( options.empty() )
	{
		if( parseErrors ) *parseErrors += "No option found";
		return {};
	}

	std::vector<CommandLineOption> commandLineOptions;
	// for each option
	for( auto &opt : options )
	{
		// invalid or empty option ?
		if( opt.size() <= 1 )
		{
			if( parseErrors )
				*parseErrors += "Expected command line argument\n";
			continue; 
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

		commandLineOptions.push_back( CommandLineOption( optionName, optionParameters ) );
	}
#endif
	return {};
}