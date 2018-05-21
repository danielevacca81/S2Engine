// UserInteractionManager.cpp
//
#include "UserInteractionManager.h"

#include <QKeySequence>
#include <QShortcut>

#include <iostream>

#include "Globals/StringTools.h"

using namespace s2Qt;

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
std::map<std::string,unsigned int> UserInteractionManager::mouseCommandCodes;
std::map<std::string,unsigned int> UserInteractionManager::keyboardCommandCodes;
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------
unsigned int UserInteractionManager::decodeMouseCommnand( const std::string &command )
{
	if( mouseCommandCodes.empty() )
	{
		mouseCommandCodes.insert( std::make_pair("Alt",         MouseStatus::ModifierAlt) );
		mouseCommandCodes.insert( std::make_pair("Ctrl",        MouseStatus::ModifierCtrl) );
		mouseCommandCodes.insert( std::make_pair("Shift",       MouseStatus::ModifierShift) );
		mouseCommandCodes.insert( std::make_pair("LeftButton",  MouseStatus::ButtonLeft) );
		mouseCommandCodes.insert( std::make_pair("RightButton", MouseStatus::ButtonRight) );
		mouseCommandCodes.insert( std::make_pair("MidButton",   MouseStatus::ButtonMiddle) );
		mouseCommandCodes.insert( std::make_pair("DoubleClick", MouseStatus::DoubleClick) );
		mouseCommandCodes.insert( std::make_pair("Moving",      MouseStatus::Moving) );
		mouseCommandCodes.insert( std::make_pair("Drag",        MouseStatus::Drag) );
		mouseCommandCodes.insert( std::make_pair("Wheel",       MouseStatus::Wheel) );
	}

	unsigned int commandCode = 0;
	std::vector<std::string> v = s2::split(command,"+");
	for( size_t i=0; i<v.size(); ++i )
	{
		// find the command
		std::map<std::string,unsigned int>::iterator it = mouseCommandCodes.find(v[i]);
		if( it != mouseCommandCodes.end() )
			commandCode |= it->second;
	}
	
	return commandCode;	
}


// ------------------------------------------------------------------------------------------------
unsigned int UserInteractionManager::decodeKeyboardCommnand( const std::string &command )
{
	QKeySequence ks( QString(command.c_str() ));

	unsigned int commandCode = (int)ks;

	keyboardCommandCodes.insert( std::make_pair(command, (int)ks) );
	return commandCode;	
}