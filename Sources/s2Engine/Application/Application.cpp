// Application.cpp
//
#include "Application.h"

#include "Window.h"

#include "glfwpp/glfwpp.h"

#include <optional>
#include <memory>

using namespace s2;

static Application* gGlobalAppInstance = nullptr;

// ------------------------------------------------------------------------------------------------
Application::Application( const std::string &name )
{
    if( _instance )
        throw std::runtime_error( "Only one application instance is allowed" );

    _instance = static_cast<void*>( new glfw::GlfwLibrary( glfw::init() ) );
	gGlobalAppInstance = this;
}

// ------------------------------------------------------------------------------------------------
Application::~Application()
{
	// release all windows
    for( auto &w : _windows )
        w.reset();

	// release GLFW
    delete static_cast<glfw::GlfwLibrary*>(_instance);
    _instance = nullptr;
	gGlobalAppInstance = nullptr;
}

// ------------------------------------------------------------------------------------------------
Application* Application::instance()
{
    return gGlobalAppInstance;
}

// ------------------------------------------------------------------------------------------------
void Application::addWindow( const std::shared_ptr< Window > &w)
{
    if( !w )
		throw std::runtime_error( "Application::addWindow() - Null window cannot be added to application" );

    if( _windows.size() >= 1 )
		throw std::runtime_error( "Application::addWindow() - Multiple window application not supported yet" );

    const auto glfwWindow      = static_cast<glfw::Window*>( w->_handle );
    const auto [width, height] = glfwWindow->getSize();
    //const auto [x, y]          = glfwWindow->getPos();


    w->makeCurrent();
    w->onInitializeEvent();

    // set initial size
    w->setFrameBufferSize( width, height );
    w->setSize( width, height );
    w->onResizeEvent( width, height );

	// add to window list
	_windows.push_back( w );
}

// ------------------------------------------------------------------------------------------------
std::shared_ptr< Window > Application::mainWindow() const
{
    if( _windows.empty() )
        throw std::runtime_error( "Application::mainWindow() - No window available in the application" );
    return _windows.front();
}

// ------------------------------------------------------------------------------------------------
// main application launcher and loop
int32_t Application::run()
{
	// assume single window application for now.   
	// get main window 
	const auto w = mainWindow(); // throws if no window available
    const auto glfwWindow = static_cast<glfw::Window*>( w->_handle );


    while( !glfwWindow->shouldClose() )
    {
        glfw::pollEvents();

		// update application logic
		updateState();

        // is minimized??
        if( w->width() == 0 && w->height() == 0 )
            continue;
        

		// send render on all windows
		// for( const auto &w : app->_windows ) // assume single window for now
            w->paint();


        glfwWindow->swapBuffers();
    }

    return 0;
}

// ------------------------------------------------------------------------------------------------
uint64_t Application::elapsedTime_ms() const
{
    return _appTimer.elapsed_msec();
}
