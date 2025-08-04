// InputWrapper.h
//
#ifndef APPLICATION_INPUTWRAPPER_H
#define APPLICATION_INPUTWRAPPER_H

#include "Application_API.h"

#include "Input.h"

#include <variant>

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

class InputWrapper
{
public:
	//void updateKeyboardState();
	void updateMouseState( const MouseEvent &e );

	const MouseState& mouseState()    const { return _mouseState; }


private:
	MouseState _mouseState;
	//KeyboardState _keyboardState;
};

}
}
#endif