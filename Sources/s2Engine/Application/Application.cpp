// Application.cpp
//
#include "Application.h"

#include "Window.h"

#include "GLFW/glfw3.h"

#include <memory>

using namespace s2;

static Application* gGlobalAppInstance = nullptr;

// ------------------------------------------------------------------------------------------------
Application::Application( const std::string& name )
{
    if( _instance )
        throw std::runtime_error( "Only one application instance is allowed" );

    if( !glfwInit() )
        throw std::runtime_error( "Failed to initialize GLFW" );

    _instance = reinterpret_cast<void*>( std::uintptr_t( 1 ) ); // sentinel: GLFW is alive
    gGlobalAppInstance = this;
}

// ------------------------------------------------------------------------------------------------
Application::~Application()
{
    for( auto& w : _windows )
        w.reset();

    glfwTerminate();
    _instance          = nullptr;
    gGlobalAppInstance = nullptr;
}

// ------------------------------------------------------------------------------------------------
Application* Application::instance()
{
    return gGlobalAppInstance;
}

// ------------------------------------------------------------------------------------------------
void Application::addWindow( std::unique_ptr<Window> w )
{
    if( !w )
        throw std::runtime_error( "Application::addWindow() - Null window cannot be added to application" );

    if( _windows.size() >= 1 )
        throw std::runtime_error( "Application::addWindow() - Multiple window application not supported yet" );

    _windows.push_back( std::move( w ) );
}

// ------------------------------------------------------------------------------------------------
const std::unique_ptr<Window>& Application::mainWindow() const
{
    if( _windows.empty() )
        throw std::runtime_error( "Application::mainWindow() - No window available in the application" );
    return _windows.front();
}

// ------------------------------------------------------------------------------------------------
int32_t Application::run()
{
    // Initialize all windows before entering the main loop
    for( auto& w : _windows )
    {
        int fbWidth, fbHeight;
        w->framebufferSize( fbWidth, fbHeight );

        w->makeCurrent();
        w->onInitializeEvent();
        w->postResize( fbWidth, fbHeight );
        w->startRenderThread();
    }

    // consider only the main window for now, multiple window support is not implemented yet
    const auto& w = mainWindow();

    while( !w->shouldClose() )
    {
        glfwPollEvents();
        updateState();

        if( w->width() == 0 && w->height() == 0 )
            continue;

        w->submitFrameAndWait();
        w->swapBuffers();
    }

    // the main loop has exited, which means the application is shutting down.
    for( auto& w : _windows )
        w->stopRenderThread();

    return 0;
}

// ------------------------------------------------------------------------------------------------
uint64_t Application::elapsedTime_ms() const
{
    return _appTimer.elapsed_msec();
}
