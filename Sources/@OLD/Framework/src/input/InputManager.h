// InputManager.h
//
#ifndef INPUTMANAGER_ONCE
#define INPUTMANAGER_ONCE

#include "MouseStatus.h"
#include "KeyboardStatus.h"
#include "S2FrameworkAPI.h"

namespace OIS {
	class Keyboard;
	class InputManager;
	class Mouse;
}

namespace s2Fwk {

class InputEventHandler;

class S2FRAMEWORK_API InputManager
{
private:
	OIS::InputManager *_im;
	OIS::Keyboard	  *_kb;
	OIS::Mouse		  *_mouse;
	InputEventHandler *_handler;

	MouseStatus    _mouseStatus;
	KeyboardStatus _keyboardStatus;

public:
	InputManager();
	~InputManager();

	bool init(unsigned int hwnd);
	void destroy();
	void update();
	void resize( int w, int h );

	MouseStatus    mouseStatus()    const { return _mouseStatus; }
	KeyboardStatus keyboardStatus() const { return _keyboardStatus; }

};

}
#endif
