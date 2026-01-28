// Window.h
//
#ifndef APPLICATION_WINDOW_H
#define APPLICATION_WINDOW_H

#include "s2Engine_API.h"

#include "Input.h"
#include "WindowParameters.h"

#include <string>
#include <vector>
#include <memory>

namespace RenderCore {
class Context;
class RenderTarget;
}

namespace s2 {

namespace Input { class InputWrapper; }

class Application;

class S2ENGINE_API Window
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

	virtual void onInitializeEvent()                           {}
	virtual void onMouseMoveEvent( const Input::MouseState& ms ) {}
	virtual void onMouseDoubleClickEvent( const Input::MouseState& ms ) {}
	virtual void onMouseButtonEvent( const Input::MouseState& ms ) {} // onmousebuttonpress only. todo: onmousebuttonrelease
	virtual void onMouseScrollEvent( const Input::MouseState& ms ) {}
	virtual void onCloseEvent() {}
	virtual void onPaintEvent()                                {}
	virtual void onResizeEvent( uint32_t width, uint32_t height ) {} 


protected:
	// ***
	// Do not pass the window size to glViewport or other pixel - based OpenGL calls.
	// The window size is in screen coordinates, not pixels. Use the framebuffer size, which is in pixels, for pixel - based calls.	
	// On some machines screen coordinates and pixels are the same, but on others they will not be.
	void setFrameBufferSize( int width, int height );
	void setSize( int width, int height );

	void makeCurrent();
	void paint();

protected:
	RenderCore::Context* _renderingContext {nullptr};
	std::unique_ptr<RenderCore::RenderTarget> _renderTarget;

private:
	void*                _handle       = nullptr;
	Input::InputWrapper* _inputWrapper = nullptr;

	friend class Application;
};

}


#endif