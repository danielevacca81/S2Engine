// Application.cpp
//
#include "Application.h"

#include "Window.h"

#include "glfwpp/glfwpp.h"

#include <optional>
#include <memory>

using namespace s2;

// ------------------------------------------------------------------------------------------------
Application::Application( const std::string &name )
{
    if( _instance )
        throw std::runtime_error( "Only one application instance is allowed" );

    _instance = static_cast<void*>( new glfw::GlfwLibrary( glfw::init() ) );
}

// ------------------------------------------------------------------------------------------------
Application::~Application()
{
    delete static_cast<glfw::GlfwLibrary*>(_instance);
    _instance = nullptr;
}

// ------------------------------------------------------------------------------------------------
int Application::run( Window *w )
{
    const auto glfwWindow = static_cast<glfw::Window*>( w->_handle );

    auto [width, height] = glfwWindow->getSize();
    auto [x, y] = glfwWindow->getPos();

    w->makeCurrent();
    w->onInitializeEvent();

    // set initial size
    w->onFramebufferSizeEvent( width, height );
    w->onSizeEvent( width, height );
    
    while( !glfwWindow->shouldClose() )
    {
        glfw::pollEvents();
        
        // is minimized??
        if( w->width() == 0 && w->height() == 0 )
            continue;
        // update timers
        // update logic/physics

        w->paint();


        glfwWindow->swapBuffers();
    }

    return 0;
}