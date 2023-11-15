// Window.cpp
//
#include "Window.h"

#include "glfwpp/glfwpp.h"
#include "glfwpp/window.h"

#include "RenderSystem/Context.h"

#include <iostream>

using namespace s2;

// ------------------------------------------------------------------------------------------------
Window::Window( const std::string& name, int width, int height, const WindowParameters &params /**/ )
{
    const glfw::WindowHints hints 
    {
	    .clientApi = glfw::ClientApi::OpenGl,
	    .contextVersionMajor = params.contextVersionMajor,
	    .contextVersionMinor = params.contextVersionMinor,
        .openglProfile = glfw::OpenGlProfile::Compat,
    };
    hints.apply();

    auto handle = new glfw::Window( width, height, name.c_str() );
    {
        //handle.charEvent           .setCallback( [w] () { w.onCharEvent           (); } );
        handle->closeEvent.setCallback( [=] ( glfw::Window& ) { makeCurrent(); onCloseEvent(); } );
        //handle.cursorEnterEvent    .setCallback( [=] () { w.onCursorEnterEvent    (); } );
        //handle.cursorPosEvent      .setCallback( [=] () { w.onCursorPosEvent      (); } );
        //handle.dropEvent           .setCallback( [=] () { w.onDropEvent           (); } );
        //handle.focusEvent          .setCallback( [=] () { w.onFocusEvent          (); } );
        //handle.framebufferSizeEvent.setCallback( [=] () { w.onFramebufferSizeEvent(); } );
        //handle.iconifyEvent        .setCallback( [=] () { w.onIconifyEvent        (); } );
        //handle.keyEvent            .setCallback( [=] () { w.onKeyEvent            (); } );
        //handle.mouseButtonEvent    .setCallback( [=] () { w.onMouseButtonEvent    (); } );
        handle->posEvent             .setCallback( [=] ( glfw::Window&, int width, int height ) { makeCurrent();  } );
        //handle.refreshEvent        .setCallback( [=] () { w.onRefreshEvent        (); } );
        //handle.scrollEvent         .setCallback( [=] () { w.onScrollEvent         (); } );
        handle->sizeEvent            .setCallback( [=] ( glfw::Window&, int width, int height ) { makeCurrent(); onSizeEvent(width,height); } );
    }
    
    // no current context before this call:
    glfw::makeContextCurrent( *handle );

    // store current context into contextRegistry
    _context = RenderSystem::Context::current();
    {
        auto& ctx = glfw::getCurrentContext();
        std::cout << "Current GLFW Context: " << std::hex << handle << '\n';
    
        //std::cout << "Current S2 Context: " << std::hex << _context->nativeHandle() << '\n';
    }

    _handle = static_cast<void*>( handle);
}

// ------------------------------------------------------------------------------------------------
void Window::makeCurrent()
{
    glfw::makeContextCurrent( *static_cast<glfw::Window*>(_handle) );
}


// ------------------------------------------------------------------------------------------------
Window::~Window()
{
    delete static_cast<glfw::Window*>( _handle );
    _handle = nullptr;
}