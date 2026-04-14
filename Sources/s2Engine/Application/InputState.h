// InputState.h
//
#ifndef APPLICATION_INPUTSTATE_H
#define APPLICATION_INPUTSTATE_H

#include "Input.h"

#include <variant>
#include <cstdint>

namespace s2 {
namespace Input {

struct MouseMoveEvent
{
	double x;
	double y;
};

struct MouseButtonEvent
{
	enum EventType
	{
		Press       = 0,
		Release     = 1,
	} eventType;

	uint32_t button;
	uint32_t modifiers;
};

struct MouseWheelEvent
{
	double deltax;
	double deltay;
};

using MouseEvent = std::variant<MouseMoveEvent, MouseButtonEvent, MouseWheelEvent>;


struct KeyboardEvent
{
    enum EventType { Press = 0, Release = 1 } eventType;
    int key { 0 };          // key code
    uint32_t modifiers { 0 }; // bitmask of Modifier flags
};

class InputState
{
public:
    void updateKeyboardState( const KeyboardEvent &e );
    const KeyboardState& keyboardState() const { return _keyboardState; }

	void updateMouseState( const MouseEvent &e );
	const MouseState& mouseState()    const { return _mouseState; }


private:
	KeyboardState _keyboardState;
	MouseState    _mouseState;
};

}
}
#endif