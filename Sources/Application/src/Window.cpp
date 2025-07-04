// Window.cpp
//
#include "Window.h"

#include "RenderCore/Context.h"
#include "RenderCore/SwapChain.h"
#include "RenderCore/RenderTarget.h"

#include "glfwpp/glfwpp.h"
#include "glfwpp/window.h"


#include <iostream>

using namespace s2;

// ------------------------------------------------------------------------------------------------
Window::Window( const std::string& name, int width, int height, const WindowParameters &params /**/ )
{
    const glfw::WindowHints hints 
    {
	    .clientApi           = glfw::ClientApi::OpenGl,
	    .contextVersionMajor = params.contextVersionMajor,
	    .contextVersionMinor = params.contextVersionMinor,
        .openglProfile       = glfw::OpenGlProfile::Compat,
    };
    hints.apply();

    auto handle = new glfw::Window( width, height, name.c_str() );
    {
        //handle.charEvent           .setCallback( [w] () { w.onCharEvent           (); } );
        handle->closeEvent           .setCallback( [=] ( glfw::Window& ) { makeCurrent(); onCloseEvent(); } );
        //handle.cursorEnterEvent    .setCallback( [=] () { w.onCursorEnterEvent    (); } );
        //handle.cursorPosEvent      .setCallback( [=] () { w.onCursorPosEvent      (); } );
        //handle.dropEvent           .setCallback( [=] () { w.onDropEvent           (); } );
        //handle.focusEvent          .setCallback( [=] () { w.onFocusEvent          (); } );
        handle->framebufferSizeEvent .setCallback( [=] ( glfw::Window&, int width, int height ) { makeCurrent(); onFramebufferSizeEvent( width, height ); } );
        //handle.iconifyEvent        .setCallback( [=] () { w.onIconifyEvent        (); } );
        //handle.keyEvent            .setCallback( [=] () { w.onKeyEvent            (); } );
        //handle.mouseButtonEvent    .setCallback( [=] () { w.onMouseButtonEvent    (); } );
        handle->posEvent             .setCallback( [=] ( glfw::Window&, int width, int height ) { makeCurrent();  } );
        //handle.refreshEvent        .setCallback( [=] () { w.onRefreshEvent        (); } );
        //handle.scrollEvent         .setCallback( [=] () { w.onScrollEvent         (); } );
        handle->sizeEvent            .setCallback( [=] ( glfw::Window&, int width, int height ) { makeCurrent(); onSizeEvent(width,height); } );
    }
    
    _handle = static_cast<void*>( handle);

    // no current context before this call:
    makeCurrent();
    {
        auto& ctx = glfw::getCurrentContext();
        std::cout << "Current GLFW Context: " << std::hex << ctx << '\n';
    }

    _renderingContext = RenderCore::Context::current();
    _renderTarget     = std::make_unique<RenderCore::RenderTarget>();
}

// ------------------------------------------------------------------------------------------------
Window::~Window()
{
    makeCurrent();
	_renderTarget.reset();
	delete _renderingContext;

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

    _renderingContext->beginRendering();
    {
        onPaintEvent();
    }
    _renderingContext->endRendering();

	RenderCore::SwapChain::swapToScreen( *_renderTarget);
}

// ------------------------------------------------------------------------------------------------
void Window::onFramebufferSizeEvent( int width, int height )
{
    _renderTarget->resize( width, height );
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