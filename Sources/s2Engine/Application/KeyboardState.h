// KeyboardState.h
//
#ifndef APPLICATION_KEYBOARDSTATE_H
#define APPLICATION_KEYBOARDSTATE_H

#include "s2Engine_API.h"

#include <cstdint>

namespace s2 {

class S2ENGINE_API  KeyboardState
{
public:
	KeyboardState();

	void update();

private:
	//
	enum Modifier
	{
		//Qt::NoModifier           0x00000000  No modifier key is pressed.
		//Qt::ShiftModifier        0x02000000  A Shift key on the keyboard is pressed.
		//Qt::ControlModifier      0x04000000  A Ctrl key on the keyboard is pressed.
		//Qt::AltModifier          0x08000000  An Alt key on the keyboard is pressed.
		//Qt::MetaModifier         0x10000000  A Meta key on the keyboard is pressed.
		//Qt::KeypadModifier       0x20000000  A keypad button is pressed.
		//Qt::GroupSwitchModifier  0x40000000  X11 only.A Mode_switch key on the keyboard is pressed.

		ModifierNone  = 0x0000, // No modifier
		ModifierShift = 0x0100, // Shift key pressed
		ModifierCtrl  = 0x0200, // Ctrl key pressed
		ModifierAlt   = 0x0400, // Alt key pressed
		ModifierMeta  = 0x0800, // Meta key pressed
		ModifierAny   = 0x0F00, // Mask to get modifiers only
	};

private:
	uint32_t _modifiers;
};


}
#endif