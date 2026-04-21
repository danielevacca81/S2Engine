// KeyboardState.cpp
//
#include "KeyboardState.h"

using namespace s2::Input;

// ------------------------------------------------------------------------------------------------
bool KeyboardState::isKeyDown( int key ) const
{
	return _pressedKeys.find( key ) != _pressedKeys.end();
}

// ------------------------------------------------------------------------------------------------
bool KeyboardState::isKeyUp( int key ) const
{
	return _pressedKeys.find( key ) == _pressedKeys.end();
}

// ------------------------------------------------------------------------------------------------
void KeyboardState::onKeyPress( int key, uint32_t modifiers )
{
    _pressedKeys.insert( key );
    _modifiers = modifiers;
}

// ------------------------------------------------------------------------------------------------
void KeyboardState::onKeyRelease( int key, uint32_t modifiers )
{
    _pressedKeys.erase( key );
    _modifiers = modifiers;
}