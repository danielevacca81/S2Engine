// UserInteractionManager.cpp
//
#include "UserInteractionManager.h"

#include <QKeySequence>
#include <QShortcut>

#include <iostream>

using namespace s2::Qt;


// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
std::map<QString, unsigned int> UserInteractionManager::mouseCommandCodes;
std::map<QString, unsigned int> UserInteractionManager::keyboardCommandCodes;
std::map<QString, unsigned int> UserInteractionManager::gestureCommandCodes;
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------
UserInteractionManager::UserInteractionManager()
{ 
	clear(); 
}

// ------------------------------------------------------------------------------------------------
UserInteractionManager::~UserInteractionManager()
{ 
	clear(); 
}

// ------------------------------------------------------------------------------------------------
void UserInteractionManager::setPreferences( const Preferences &p )
{
	_preferences = p;
}

UserInteractionManager::Preferences UserInteractionManager::preferences() const { return _preferences; }

// ------------------------------------------------------------------------------------------------
bool UserInteractionManager::registerMouseCommand( const UIMCommand &c )
{
	unsigned int commandCode = decodeMouseCommnand( QString::fromStdString( c.activator() ) );
	if( _mouseBindings.find( commandCode ) != _mouseBindings.end() )
		return false;	// bind already used!

	_mouseBindings[commandCode] = c;
	return true;
}

// ------------------------------------------------------------------------------------------------
bool UserInteractionManager::registerKeyboardCommand( const UIMCommand &c )
{
	unsigned int commandCode = decodeKeyboardCommnand( QString::fromStdString( c.activator() ) );
	if( _keyBindings.find( commandCode ) != _keyBindings.end() )
		return false;	// bind already used! 

	_keyBindings[commandCode] = c;
	return true;
}

// ------------------------------------------------------------------------------------------------
bool UserInteractionManager::registerGestureCommand( const UIMCommand &c )
{
	unsigned int commandCode = decodeGestureCommand( QString::fromStdString( c.activator() ) );
	if( _gestureBindings.find( commandCode ) != _gestureBindings.end() )
		return false;	// bind already used! 

	_gestureBindings[commandCode] = c;
	return true;
}

// ------------------------------------------------------------------------------------------------
bool UserInteractionManager::unregisterMouseCommand( const QString &activator )
{
	unsigned int code = decodeMouseCommnand( activator );
	return _mouseBindings.erase( code ) == 1;
}

// ------------------------------------------------------------------------------------------------
bool UserInteractionManager::unregisterKeyboardCommand( const QString &activator )
{
	unsigned int code = decodeKeyboardCommnand( activator );
	return _keyBindings.erase( code ) == 1;
}

// ------------------------------------------------------------------------------------------------
bool UserInteractionManager::unregisterGestureCommand( const QString &activator )
{
	unsigned int code = decodeGestureCommand( activator );
	return _gestureBindings.erase( code ) == 1;
}

// ------------------------------------------------------------------------------------------------
void UserInteractionManager::clear()
{
	_ms.reset();
	_kb.reset();
	// reset gesture?

	_mouseBindings.clear();
	_keyBindings.clear();
	_gestureBindings.clear();
}

// ------------------------------------------------------------------------------------------------
void UserInteractionManager::updateMouse( QGraphicsSceneMouseEvent *e, bool updateButtons )
{
	_ms.update( e, updateButtons );
	executeMouseCommand( _ms.getCode() );
}

// ------------------------------------------------------------------------------------------------
void UserInteractionManager::updateMouse( QGraphicsSceneWheelEvent *e, bool updateButtons )
{
	_ms.update( e, updateButtons );
	executeMouseCommand( _ms.getCode() );
}

// ------------------------------------------------------------------------------------------------
void UserInteractionManager::updateKeyboard( QKeyEvent *e, bool keyPressed )
{
	_kb.update( e, keyPressed );

	if( keyPressed )
		executeKeyboardCommand( _kb.getCode() );
}

// ------------------------------------------------------------------------------------------------
void UserInteractionManager::updateGesture( QGestureEvent *e )
{
	_gst.update( e );

	if( e->isAccepted() )
		executeGestureCommand( _gst.getCode() );
}

// ------------------------------------------------------------------------------------------------
void UserInteractionManager::executeMouseCommand( unsigned int commandID )
{
	Bindings::iterator it = _mouseBindings.find( commandID );
	if( it != _mouseBindings.end() )
		it->second.execute();
}

// ------------------------------------------------------------------------------------------------
void UserInteractionManager::executeKeyboardCommand( unsigned int commandID )
{
	UserInteractionManager::Bindings::iterator it = _keyBindings.find( commandID );
	if( it != _keyBindings.end() )
		it->second.execute();
}

// ------------------------------------------------------------------------------------------------
void UserInteractionManager::executeGestureCommand( unsigned int commandID )
{
	Bindings::iterator it = _gestureBindings.find( commandID );
	if( it != _gestureBindings.end() )
		it->second.execute();
}

// ------------------------------------------------------------------------------------------------
unsigned int UserInteractionManager::decodeMouseCommnand( const QString &command )
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
		mouseCommandCodes.insert( std::make_pair("Released",    MouseStatus::Released) );
	}

    unsigned int commandCode = 0;
    QStringList list = command.split("+");
    for( auto s: list )
    {
    	// find the command
    	auto command = mouseCommandCodes.find(s);
    	
        if( command != mouseCommandCodes.end() )
    		commandCode |= command->second;
    }
	
	return commandCode;	
}


// ------------------------------------------------------------------------------------------------
unsigned int UserInteractionManager::decodeKeyboardCommnand( const QString &command )
{
    unsigned int commandCode = 0;   //0 means no valid code
	QKeySequence ks( command );
    if( ks.count() == 1 )
    {
        //TODO_SBER: si assume che il key sequence contenga una sola sequenza di tasti (esempio CTRL+W ma non CTRL+W,ALT+T)
        //           Se devono essere gestite sequenze multiple, occorre generare un codice numerico composto a partire dai singoli codici
        //std::cout << ks.toString().toStdString() << std::endl;
        commandCode = ks[0];
        keyboardCommandCodes.insert(std::make_pair(command, ks[0]));
    }
	return commandCode;	
}

// ------------------------------------------------------------------------------------------------
unsigned int UserInteractionManager::decodeGestureCommand( const QString &command )
{
	if( gestureCommandCodes.empty() )
	{
        //QTapAndHoldGesture::setTimeout(2000); //Default 700 ms
		gestureCommandCodes.insert( std::make_pair( "Pan",   GestureStatus::Pan   ) );
		gestureCommandCodes.insert( std::make_pair( "Pinch", GestureStatus::Pinch ) );
		gestureCommandCodes.insert( std::make_pair( "Swipe", GestureStatus::Swipe ) );
        gestureCommandCodes.insert( std::make_pair( "Tap", GestureStatus::Tap) );
        gestureCommandCodes.insert( std::make_pair( "TapAndHold", GestureStatus::TapAndHold ) );
	}

	auto commandIt = gestureCommandCodes.find( command );

	unsigned int commandCode = 0;
	if( commandIt != gestureCommandCodes.end() )
		commandCode |= commandIt->second;

	return commandCode;
}