// Window.cpp
//
#include "Window.h"

#include "InputState.h"
#include "UILayer.h"

#include "RenderCore/Context.h"
#include "RenderCore/RenderTarget.h"
#include "RenderCore/RenderBackend.h"

#include "Renderer/Renderer.h"

#include "GLFW/glfw3.h"

#include <iostream>

using namespace s2;

// ================================================================================================
// Window::Impl -> platform-specific internals (GLFW), invisible from headers
// ================================================================================================
struct Window::Impl
{
    GLFWwindow*        window = nullptr;
    Input::InputState* input  = nullptr;

    // ---- Factory -----------------------------------------------------------
    static std::unique_ptr<Impl> create( const std::string& name,
                                         int width, int height,
                                         const WindowParameters& params,
                                         Window* owner )
    {
        glfwDefaultWindowHints();
        glfwWindowHint( GLFW_CLIENT_API,            GLFW_OPENGL_API );
        glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, params.contextVersionMajor );
        glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, params.contextVersionMinor );
        glfwWindowHint( GLFW_OPENGL_PROFILE,        [params]
        { 
            switch( params.openglProfile )
            {
                case OpenGLProfile::Any:    return 0; // Don't set the hint, let GLFW decide
                case OpenGLProfile::Compat: return GLFW_OPENGL_COMPAT_PROFILE;
                case OpenGLProfile::Core:   return GLFW_OPENGL_CORE_PROFILE;
            }
            return 0;
        }() );

        auto impl    = std::make_unique<Impl>();
        impl->input  = new Input::InputState;
        impl->window = glfwCreateWindow( width, height, name.c_str(), nullptr, nullptr );

        if( !impl->window )
            throw std::runtime_error( "Failed to create GLFW window" );

        glfwSetWindowUserPointer( impl->window, owner );
        installCallbacks( impl->window );

        return impl;
    }

    ~Impl()
    {
        delete input;
        if( window )
            glfwDestroyWindow( window );
    }

    // ---- Queries -----------------------------------------------------------
    uint32_t width()  const { int w, h; glfwGetWindowSize( window, &w, &h ); return static_cast<uint32_t>( w ); }
    uint32_t height() const { int w, h; glfwGetWindowSize( window, &w, &h ); return static_cast<uint32_t>( h ); }

    void framebufferSize( int& w, int& h ) const { glfwGetFramebufferSize( window, &w, &h ); }

    bool shouldClose()  const { return glfwWindowShouldClose( window ); }
    void swapBuffers()        { glfwSwapBuffers( window ); }
    void makeCurrent()        { glfwMakeContextCurrent( window ); }

    // ---- Callbacks ---------------------------------------------------------
    // Application-side callbacks
    static void installCallbacks( GLFWwindow* w )
    {
        glfwSetWindowCloseCallback( w, []( GLFWwindow* w )
        {
            static_cast<Window*>( glfwGetWindowUserPointer( w ) )->onCloseEvent();
        } );

        glfwSetCursorPosCallback( w, []( GLFWwindow* w, double x, double y )
        {
            auto* self = static_cast<Window*>( glfwGetWindowUserPointer( w ) );
            auto* impl = self->_impl.get();
            impl->input->updateMouseState(
                Input::MouseMoveEvent{ x, static_cast<double>( impl->height() ) - y - 1 } );
            self->onMouseMoveEvent( impl->input->mouseState() );
        } );

        glfwSetMouseButtonCallback( w, []( GLFWwindow* w, int button, int action, int mods )
        {
            auto* self = static_cast<Window*>( glfwGetWindowUserPointer( w ) );
            auto* impl = self->_impl.get();
            impl->input->updateMouseState(
                Input::MouseButtonEvent
                {
                    .eventType = action == GLFW_PRESS ? Input::MouseButtonEvent::Press
                                                      : Input::MouseButtonEvent::Release,
                    .button    = uint32_t( 1 ) << static_cast<uint32_t>( button ),
                    .modifiers = static_cast<uint32_t>( mods )
                } );

            if( impl->input->mouseState().doubleClickButton() != Input::MouseState::ButtonNone )
                self->onMouseDoubleClickEvent( impl->input->mouseState() );
            else
                self->onMouseButtonEvent( impl->input->mouseState() );
        } );

        glfwSetScrollCallback( w, []( GLFWwindow* w, double x, double y )
        {
            auto* self = static_cast<Window*>( glfwGetWindowUserPointer( w ) );
            auto* impl = self->_impl.get();
            impl->input->updateMouseState( Input::MouseWheelEvent{ x, y } );
            self->onMouseScrollEvent( impl->input->mouseState() );
        } );

        glfwSetKeyCallback( w, []( GLFWwindow* w, int key, int scancode, int action, int mods )
        {
            auto* self = static_cast<Window*>( glfwGetWindowUserPointer( w ) );
            auto* impl = self->_impl.get();
            impl->input->updateKeyboardState(   
                Input::KeyboardEvent{
                    .eventType = action == GLFW_PRESS ? Input::KeyboardEvent::Press
                                                      : Input::KeyboardEvent::Release,
                    .key = key,
                    .modifiers = static_cast<uint32_t>( mods )
                } );
            self->onKeyboardEvent( impl->input->keyboardState() );
        } );

        glfwSetCharCallback( w, []( GLFWwindow* w, unsigned int c )
        {
            // Application char handling can be added here if needed
        } );

        glfwSetWindowFocusCallback( w, []( GLFWwindow* w, int focused )
        {
        } );

        glfwSetCursorEnterCallback( w, []( GLFWwindow* w, int entered )
        {
        } );

        glfwSetMonitorCallback( []( GLFWmonitor* monitor, int event )
        {
        } );

        glfwSetFramebufferSizeCallback( w, []( GLFWwindow* w, int width, int height )
        {
            static_cast<Window*>( glfwGetWindowUserPointer( w ) )->postResize( width, height );
        } );

        glfwSetWindowSizeCallback( w, []( GLFWwindow* w, int, int )
        {
            int fbW, fbH;
            glfwGetFramebufferSize( w, &fbW, &fbH );
            static_cast<Window*>( glfwGetWindowUserPointer( w ) )->postResize( fbW, fbH );
        } );

        glfwSetWindowPosCallback( w, []( GLFWwindow*, int, int ) {} );
    }
};

// ================================================================================================
// Helpers
// ================================================================================================
static constexpr uint64_t packSize( int w, int h ) noexcept
{
    return ( static_cast<uint64_t>( static_cast<uint32_t>( w ) ) << 32 )
         |   static_cast<uint64_t>( static_cast<uint32_t>( h ) );
}

static constexpr std::pair<int,int> unpackSize( uint64_t v ) noexcept
{
    return { static_cast<int>( static_cast<uint32_t>( v >> 32 ) ),
             static_cast<int>( static_cast<uint32_t>( v        ) ) };
}

// ================================================================================================
// Construction / Destruction
// ================================================================================================
Window::Window()
{}

// ------------------------------------------------------------------------------------------------
Window::Window( const std::string& name, int width, int height, const WindowParameters& params )
    : _impl( Impl::create( name, width, height, params, this ) )
{
    makeCurrent();
    _renderingContext = std::make_unique<RenderCore::Context>();
	_renderer         = std::make_unique<Renderer::Renderer>( _renderingContext.get() );
    _mainRenderTarget = std::make_unique<RenderCore::RenderTarget>();
	_mainRenderTarget->setObjectLabel( "s2Engine.MainRenderTarget" );
	
    _ui               = createDefaultUILayer();
	_ui->init( _impl->window ); // install callbacks and initialize UI layer with the native window handle
}

// ------------------------------------------------------------------------------------------------
Window::~Window()
{
    stopRenderThread();
    
	_renderer.reset();
    _ui.reset();
    _impl.reset();
}

// ================================================================================================
// Public API — delegates to Impl
// ================================================================================================
uint32_t Window::width()  const { return _impl->width();  }
uint32_t Window::height() const { return _impl->height(); }

void* Window::nativeHandle() const noexcept { return _impl ? _impl->window : nullptr; }
bool  Window::shouldClose()  const          { return _impl->shouldClose();  }
void  Window::swapBuffers()                 { _impl->swapBuffers(); }
void  Window::makeCurrent()                 { _impl->makeCurrent(); }

void Window::framebufferSize( int& w, int& h ) const { _impl->framebufferSize( w, h ); }

// ================================================================================================
// Render thread
// ================================================================================================
void Window::startRenderThread()
{
    glfwMakeContextCurrent( nullptr );

    _renderThread.start( [this]
    {
        makeCurrent();
    } );
}

// ------------------------------------------------------------------------------------------------
void Window::stopRenderThread()
{
    if( !_renderThread.isRunning() )
        return;

    _renderThread.stop( [this]
    {
        onShutdownEvent();

        _mainRenderTarget.reset();
        _renderingContext.reset();

        glfwMakeContextCurrent( nullptr );
    } );
}

// ================================================================================================
// Frame
// ================================================================================================
void Window::submitFrameAndWait()
{
    _renderThread.enqueueFrame( [this] { drawCurrentFrame(); } );
    _renderThread.waitFrameComplete();
}

// ------------------------------------------------------------------------------------------------
void Window::drawCurrentFrame()
{
    const uint64_t pending = _pendingResize.exchange( 0, std::memory_order_acquire );
    if( pending != 0 )
    {
        const auto [fbWidth, fbHeight] = unpackSize( pending );
        applyFrameBufferResize( fbWidth, fbHeight );
        onResizeEvent( static_cast<uint32_t>( fbWidth ),
                       static_cast<uint32_t>( fbHeight ) );
    }

    // Application rendering callback.
    // the pipeline will be executed on the render thread
    onDraw();

    _renderingContext->rendererBackend().blitToScreen( *_mainRenderTarget );
}

// ------------------------------------------------------------------------------------------------
void Window::applyFrameBufferResize( int width, int height )
{
    _mainRenderTarget->resize( width, height );
}

// ------------------------------------------------------------------------------------------------
void Window::postResize( int width, int height ) noexcept
{
    _pendingResize.store( packSize( width, height ), std::memory_order_release );
}

// ================================================================================================
// UI helpers
// ================================================================================================
bool Window::uiWantCaptureMouse() const noexcept
{
    return _ui && _ui->wantCaptureMouse();
}

// ------------------------------------------------------------------------------------------------
bool Window::uiWantCaptureKeyboard() const noexcept
{
    return _ui && _ui->wantCaptureKeyboard();
}