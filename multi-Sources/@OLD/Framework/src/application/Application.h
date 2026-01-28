// Application.h
//
#ifndef APPLICATION_ONCE
#define APPLICATION_ONCE


#include "S2FrameworkAPI.h"

#include <string>

namespace s2Fwk{

class GLWindow;
class InputManager;

class S2FRAMEWORK_API Application
{
private:
	static Application *_ptr;	
	GLWindow	       *_window;

	void updateInput();
	void createInput();
	void destroy();

protected:
	InputManager *_input;
	void defaultMainLoop();

public:
	static Application *s2App() { return _ptr; }

	Application( const std::string &name );
	virtual ~Application();

	void run();

	virtual void mainLoop();
	virtual bool init();
	virtual void resizeWindow(int w, int h);
	virtual void paint();
	virtual void update();

	virtual void onMousePressed();
	virtual void onMouseReleased();
	virtual void onMouseMoved();

	virtual void onKeyPressed();
	virtual void onKeyReleased();
};

}

#endif
