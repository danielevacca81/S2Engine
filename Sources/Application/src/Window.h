// Window.h
//
#ifndef APPLICATION_WINDOW_H
#define APPLICATION_WINDOW_H

#include "Application_API.h"

#include "InputState.h"
#include "WindowParameters.h"

#include <string>
#include <vector>
#include <memory>

namespace RenderCore {
class Context;
class RenderTarget;
}

namespace s2 {

class APPLICATION_API Window
{

public:
	Window( const std::string& name, int width, int height, const WindowParameters &params /**/ );
	virtual ~Window();

	uint32_t width() const;
	uint32_t height() const;

	// @todo:
	// isMinimized
	// isFullScreen
	// isMaximized
	// ...

	virtual void onMouseMoveEvent( const MouseState& ms ) {} //rename argument to MouseEvent?
	virtual void onMouseDoubleClickEvent( const MouseState& ms ) {}
	virtual void onMouseButtonEvent( const MouseState& ms ) {}
	virtual void onMouseScrollEvent( const MouseState& ms ) {}
	virtual void onCloseEvent() {}

	// ***
	// Do not pass the window size to glViewport or other pixel - based OpenGL calls.
	// The window size is in screen coordinates, not pixels. Use the framebuffer size, which is in pixels, for pixel - based calls.	
	// On some machines screen coordinates and pixels are the same, but on others they will not be.
	virtual void onSizeEvent( int, int)                        {}
	virtual void onFramebufferSizeEvent( int w, int h );          // <-
	virtual void onPaintEvent()                                {}
	virtual void onInitializeEvent()                           {}


protected:
#if 0
	virtual void onCharEvent(unsigned int)                     {}
	virtual void onCursorEnterEvent(bool)                      {}
	virtual void onCursorPosEvent( double, double );
	virtual void onDropEvent(const std::vector<const char*> &) {}
	virtual void onFocusEvent(bool)                            {}
	virtual void onIconifyEvent(bool)                          {}
	virtual void onKeyEvent(/***/)                             {}
	virtual void onMouseButtonEvent(/***/)                     {}
	virtual void onPosEvent( int, int)                         {}
	virtual void onRefreshEvent()                              {}
	virtual void onScrollEvent( double, double)                {}
#endif	

protected:
	void makeCurrent();
	void paint();

protected:
	RenderCore::Context* _renderingContext {nullptr};
	std::unique_ptr<RenderCore::RenderTarget> _renderTarget;

private:
	void* _handle = nullptr;
	InputState _inputState;

	friend class Application;
};

}


#endif