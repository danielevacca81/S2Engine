// Window.h
//
#ifndef APPLICATION_WINDOW_H
#define APPLICATION_WINDOW_H

#include "Application_API.h"

#include "WindowParameters.h"

#include <string>
#include <vector>

namespace RenderSystem {
class Context;
}

namespace s2 {

class APPLICATION_API Window
{

public:
	Window( const std::string& name, int width, int height, const WindowParameters &params /**/ );
	virtual ~Window();

	virtual void onCharEvent(unsigned int)                     {}
	virtual void onCloseEvent()                                {}
	virtual void onCursorEnterEvent(bool)                      {}
	virtual void onCursorPosEvent( double,double)              {}
	virtual void onDropEvent(const std::vector<const char*> &) {}
	virtual void onFocusEvent(bool)                            {}
	virtual void onFramebufferSizeEvent()                      {}
	virtual void onIconifyEvent(bool)                          {}
	virtual void onInitializeEvent()                           {}
	virtual void onKeyEvent(/***/)                             {}
	virtual void onMouseButtonEvent(/***/)                     {}
	virtual void onPaintEvent()                                {}
	virtual void onPosEvent( int, int)                         {}
	virtual void onRefreshEvent()                              {}
	virtual void onScrollEvent( double, double)                {}
	virtual void onSizeEvent( int, int)                        {}

private:
	void makeCurrent();

private:
	void* _handle = nullptr;
	RenderSystem::Context* _context = nullptr;

	friend class Application;
};

}


#endif