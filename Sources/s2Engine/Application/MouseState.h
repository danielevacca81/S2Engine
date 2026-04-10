// MouseState.h
//
#ifndef APPLICATION_MOUSESTATE_H
#define APPLICATION_MOUSESTATE_H

#include "s2Engine_API.h"

#include "Math/Math.h"

namespace s2 {

namespace Input
{

class S2ENGINE_API MouseState
{
public:
	enum Button
	{
		ButtonNone   = 0x0000, // No button pressed.
		ButtonOne    = 0x0001,
		ButtonTwo    = 0x0002,
		ButtonThree  = 0x0003,
		ButtonFour   = 0x0004,
		ButtonFive   = 0x0005,
		ButtonSix    = 0x0006,
		ButtonSeven  = 0x0007,
		ButtonEight  = 0x0008,
		ButtonLeft   = ButtonOne,
		ButtonRight  = ButtonTwo,
		ButtonMiddle = ButtonThree,
		ButtonAny    = 0x000F,
	};

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
	double       wheel()             const { return _wheel; }
	bool         isDragging()        const { return _dragging; }
	Math::ivec2  position()          const { return Math::ivec2( _currPos ); }
	Button       doubleClickButton() const { return Button( _doubleClickButton ); }
	Button       buttonsDown()       const { return Button( _pressedButtons ); }
	Modifier     modifiers()         const { return Modifier( _keyModifier ); }
	bool         isButtonDown( Button button ) const { return ( _pressedButtons & button ) != ButtonNone; }
	bool         isButtonUp( Button button ) const { return ( _pressedButtons & button ) == ButtonNone; }
	bool 	     isModifierDown( Modifier modifier ) const { return ( _keyModifier & modifier ) != ModifierNone; }

//#ifdef _DEBUG
//	void dumpStatus( const std::string &str) const;
//#endif

private:
	void onPress      ( /*double x, double y, */uint32_t button, uint32_t Modifier );
	void onRelease    ( /*double x, double y, */uint32_t button, uint32_t Modifier );
	void onMove       ( double x, double y );
	void onWheel      ( double deltax, double deltay, uint32_t modifiers );
	void onDoubleClick( /*double x, double y, */uint32_t button, uint32_t modifiers );


private:
	Math::vec2  _currPos;
	Math::vec2  _clickPos { -1.f,-1.f };

	double   _wheel             { 0.0 };
	bool     _dragging          { false };
	uint32_t _doubleClickButton { ButtonNone };
	uint32_t _pressedButtons    { ButtonNone };
	uint32_t _keyModifier       { ModifierNone };

	friend class InputState; // allow InputState to access private members
};

}

}

#endif
