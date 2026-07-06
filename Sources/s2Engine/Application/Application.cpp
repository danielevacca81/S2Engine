// Application.cpp
//
#include "Application.h"

#include "Window.h"

#include "GLFW/glfw3.h"

using namespace s2;

static Application* gGlobalAppInstance = nullptr;

// ------------------------------------------------------------------------------------------------
struct Application::Impl
{
    static std::unique_ptr<Impl> create()
    {
        if( !glfwInit() )
            throw std::runtime_error( "Failed to initialize GLFW" );

        return std::make_unique<Impl>();
    }

    ~Impl()
    {
        glfwTerminate();
    }
};


// ------------------------------------------------------------------------------------------------
Application::Application( const std::string& name )
    : _impl( Impl::create() )
    , _name( name )
{
    if( gGlobalAppInstance )
        throw std::runtime_error( "Only one application instance is allowed" );

    gGlobalAppInstance = this;
}

// ------------------------------------------------------------------------------------------------
Application::~Application()
{
    for( auto& w : _windows )
        w.reset();

    _impl.reset();
    gGlobalAppInstance = nullptr;
}

// ------------------------------------------------------------------------------------------------
Application* Application::instance()
{
    return gGlobalAppInstance;
}

// ------------------------------------------------------------------------------------------------
void Application::enableLogging( const std::string &loggerName, const LogLevel &level, const LogParams &params )
{
    Log::init(loggerName, level, params );
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
    if( _windows.empty() )
        throw std::runtime_error( "Application::run() - No window available in the application" );

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

		// skip rendering if the window is minimized
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
