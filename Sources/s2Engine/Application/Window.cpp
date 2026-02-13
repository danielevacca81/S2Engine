// Window.cpp
//
#include "Window.h"

#include "InputWrapper.h"

#include "RenderCore/Context.h"
#include "RenderCore/RenderTarget.h"
#include "RenderCore/RenderCommands.h"

#include "glfwpp/glfwpp.h"
#include "glfwpp/window.h"


#include <iostream>

using namespace s2;

// ------------------------------------------------------------------------------------------------
Window::Window( const std::string& name, int width, int height, const WindowParameters &params /**/ )
{
    glfw::WindowHints 
    {
	    .clientApi           = glfw::ClientApi::OpenGl,
	    .contextVersionMajor = params.contextVersionMajor,
	    .contextVersionMinor = params.contextVersionMinor,
        .openglProfile       = glfw::OpenGlProfile::Compat,
    }.apply();


    _inputWrapper = new Input::InputWrapper;


    auto handle = new glfw::Window( width, height, name.c_str() );
    {
        handle->closeEvent.setCallback( [=] ( glfw::Window& )
        {
            makeCurrent(); 
            onCloseEvent(); 
        } );

        handle->cursorPosEvent.setCallback( [=] ( glfw::Window&, double x, double y )
        { 
            _inputWrapper->updateMouseState( Input::MouseMoveEvent{ x, this->height() - y - 1 } ); 
			onMouseMoveEvent( _inputWrapper->mouseState() ); // invoke custmo mouse move event handler
        } );

        handle->mouseButtonEvent.setCallback( [=] ( glfw::Window&, glfw::MouseButton b, glfw::MouseButtonState s, glfw::ModifierKeyBit k  )
        {
            _inputWrapper->updateMouseState(
                Input::MouseButtonEvent
				{
					.eventType = s == glfw::MouseButtonState::Press ? Input::MouseButtonEvent::Press : Input::MouseButtonEvent::Release,
					.button    = uint32_t( 1 ) << static_cast<uint32_t>( b ), // convert glfw::MouseButton to uint32_t
					.modifiers = static_cast<uint32_t>( k )  // convert glfw::ModifierKeyBit to uint32_t
				} );

            if( _inputWrapper->mouseState().doubleClickButton() != Input::MouseState::ButtonNone )
				onMouseDoubleClickEvent( _inputWrapper->mouseState() ); // invoke custom mouse double click event handler
            else
            {
				// if it is not a double click, invoke mouse button event handler
				onMouseButtonEvent( _inputWrapper->mouseState() ); // invoke custom mouse button event handler
            }
        } );

        handle->scrollEvent.setCallback( [=] ( glfw::Window&, double x, double y )
        {
            _inputWrapper->updateMouseState( Input::MouseWheelEvent { x,y } );
			onMouseScrollEvent( _inputWrapper->mouseState() ); // invoke custom mouse scroll event handler
        } );
        
        
        
        
        //handle.charEvent           .setCallback( [=] () 
        //  { w.onCharEvent           (); } )
        //  ;
        //handle.cursorEnterEvent    .setCallback( [=] () 
        //  { w.onCursorEnterEvent    (); } )
        //  ;
        //handle.dropEvent           .setCallback( [=] () { w.onDropEvent           (); } );
        //handle.focusEvent          .setCallback( [=] () { w.onFocusEvent          (); } );
        handle->framebufferSizeEvent .setCallback( [=] ( glfw::Window&, int width, int height )
        {
            makeCurrent();
			setFrameBufferSize( width, height ); // set the framebuffer size        
			onResizeEvent( width, height ); // invoke custom resize event handler
        } );
        //handle.iconifyEvent        .setCallback( [=] () { w.onIconifyEvent        (); } );
        //handle.keyEvent            .setCallback( [=] () { w.onKeyEvent            (); } );
        handle->posEvent             .setCallback( [=] ( glfw::Window&, int width, int height ) { makeCurrent();  } );
        //handle.refreshEvent        .setCallback( [=] () { w.onRefreshEvent        (); } );
        handle->sizeEvent            .setCallback( [=] ( glfw::Window&, int width, int height )
        {
            makeCurrent();
            setSize(width,height);
			onResizeEvent( width, height ); // invoke custom resize event handler
        } );
    }
    
    _handle = static_cast<void*>( handle);

    // no current context before this call:
    makeCurrent();
    {
        auto& ctx = glfw::getCurrentContext();
        std::cout << "Current GLFW Context: " << std::hex << ctx << '\n';
    }

    _renderingContext = std::make_unique<RenderCore::Context>();
    _renderTarget     = std::make_unique<RenderCore::RenderTarget>();
}

// ------------------------------------------------------------------------------------------------
Window::~Window()
{
    makeCurrent();
	_renderTarget.reset();
	_renderingContext.reset();
    delete _inputWrapper;

    delete static_cast<glfw::Window*>( _handle );   
    _handle = nullptr;
}

// ------------------------------------------------------------------------------------------------
void Window::makeCurrent()
{
    // if _handle is not current
    glfw::makeContextCurrent( *static_cast<glfw::Window*>(_handle) );
}

// ------------------------------------------------------------------------------------------------
void Window::paint()
{
    makeCurrent();

    _renderingContext->beginFrame();
    {
        onPaintEvent();
    }
    _renderingContext->endFrame();

	// Blit the rendered frame to the default framebuffer (nullptr == screen)
    _renderingContext->commands().blitToScreen( *_renderTarget );
}

// ------------------------------------------------------------------------------------------------
uint32_t Window::width() const
{
    return std::get<0>( static_cast<glfw::Window*>( _handle )->getSize() );
}

// ------------------------------------------------------------------------------------------------
uint32_t Window::height() const
{
    return std::get<1>( static_cast<glfw::Window*>( _handle )->getSize() );
}

// ------------------------------------------------------------------------------------------------
void Window::setFrameBufferSize( int width, int height )
{
    _renderTarget->resize( width, height );
}

// ------------------------------------------------------------------------------------------------
void Window::setSize( int width, int height )
{
}