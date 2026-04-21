// KeyboardState.h
//
#ifndef APPLICATION_KEYBOARDSTATE_H
#define APPLICATION_KEYBOARDSTATE_H

#include "s2Engine_API.h"

#include <cstdint>
#include <set>

namespace s2 {
namespace Input {

class S2ENGINE_API  KeyboardState
{
public:
	//
	enum Modifier
	{
		ModifierNone  = 0x0000, // No modifier
		ModifierShift = 0x0100, // Shift key pressed
		ModifierCtrl  = 0x0200, // Ctrl key pressed
		ModifierAlt   = 0x0400, // Alt key pressed
		ModifierMeta  = 0x0800, // Meta key pressed
		ModifierAny   = 0x0F00, // Mask to get modifiers only
	};


public:
	KeyboardState() = default;

	void update();

	bool isKeyDown( int key ) const;
	bool isKeyUp( int key ) const;
	Modifier modifiers() const { return Modifier(_modifiers); }	
	
private:
	void onKeyPress( int key, uint32_t modifiers );
	void onKeyRelease( int key, uint32_t modifiers );

private:
	uint32_t      _modifiers { ModifierNone };
	std::set<int> _pressedKeys;

	friend class InputState;
};

}
}
#endif