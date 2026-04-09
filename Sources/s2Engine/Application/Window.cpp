// Window.cpp
//
#include "Window.h"

#include "InputWrapper.h"

#include "RenderCore/Context.h"
#include "RenderCore/RenderTarget.h"
#include "RenderCore/RenderCommands.h"

#include "glfwpp/window.h"

#include <iostream>

using namespace s2;

// ------------------------------------------------------------------------------------------------
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

// ------------------------------------------------------------------------------------------------
Window::Window()
{}

// ------------------------------------------------------------------------------------------------
Window::Window( const std::string& name, int width, int height, const WindowParameters& params )
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
        handle->closeEvent.setCallback( [=]( glfw::Window& )
        {
            onCloseEvent();
        } );

        handle->cursorPosEvent.setCallback( [=]( glfw::Window&, double x, double y )
        {
            _inputWrapper->updateMouseState( Input::MouseMoveEvent{ x, this->height() - y - 1 } );
            onMouseMoveEvent( _inputWrapper->mouseState() );
        } );

        handle->mouseButtonEvent.setCallback( [=]( glfw::Window&, glfw::MouseButton b, glfw::MouseButtonState s, glfw::ModifierKeyBit k )
        {
            _inputWrapper->updateMouseState(
                Input::MouseButtonEvent
                {
                    .eventType = s == glfw::MouseButtonState::Press ? Input::MouseButtonEvent::Press
                                                                     : Input::MouseButtonEvent::Release,
                    .button    = uint32_t( 1 ) << static_cast<uint32_t>( b ),
                    .modifiers = static_cast<uint32_t>( k )
                } );

            if( _inputWrapper->mouseState().doubleClickButton() != Input::MouseState::ButtonNone )
                onMouseDoubleClickEvent( _inputWrapper->mouseState() );
            else
                onMouseButtonEvent( _inputWrapper->mouseState() );
        } );

        handle->scrollEvent.setCallback( [=]( glfw::Window&, double x, double y )
        {
            _inputWrapper->updateMouseState( Input::MouseWheelEvent{ x, y } );
            onMouseScrollEvent( _inputWrapper->mouseState() );
        } );

        handle->framebufferSizeEvent.setCallback( [=]( glfw::Window&, int w, int h )
        {
            postResize( w, h );
        } );

        handle->sizeEvent.setCallback( [=]( glfw::Window&, int /*w*/, int /*h*/ ) {} );
        handle->posEvent .setCallback( [=]( glfw::Window&, int /*x*/, int /*y*/ ) {} );
    }

    _handle = static_cast<void*>( handle );

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
    stopRenderThread();

    delete _inputWrapper;
    delete static_cast<glfw::Window*>( _handle );
    _handle = nullptr;
}

// ------------------------------------------------------------------------------------------------
void Window::makeCurrent()
{
    glfw::makeContextCurrent( *static_cast<glfw::Window*>( _handle ) );
}

// ------------------------------------------------------------------------------------------------
void Window::startRenderThread()
{
	// release the GL context from the main thread before starting the render thread.
    glfwMakeContextCurrent( nullptr );

    _renderThread.start( [this]
    {
        makeCurrent();
    } );
}

// ------------------------------------------------------------------------------------------------
// Shutdown sequence — the key insight:
//
// The shutdown job runs ON THE RENDER THREAD with the GL context still current.
// This means onShutdownEvent() and all RAII destructors of the derived class's
// members are guaranteed to have a valid GL context.
//
// Timeline:
//   render thread:  onShutdownEvent()          <- explicit teardown (optional)
//                   _renderTarget.reset()       <- engine GL cleanup
//                   _renderingContext.reset()    <- engine GL cleanup
//                   glfwMakeContextCurrent(null) <- release context
//   main thread:    join() returns
//                   ~Window() continues (non-GL teardown)
// ------------------------------------------------------------------------------------------------
void Window::stopRenderThread()
{
    if( !_renderThread.isRunning() )
        return;

	// Signal the render thread to stop, run the shutdown job.
	// Shutdown job runs on the render thread where the GL context is still valid.
    _renderThread.stop( [this]
    {      
        // 1. Give the user a chance to do explicit ordered cleanup.
        onShutdownEvent();

        // 2. Destroy engine GL resources while context is still valid.
        _renderTarget.reset();
        _renderingContext.reset();

        // 3. Release the GL context as the very last GL operation.
        glfwMakeContextCurrent( nullptr );
    } );
}

// ------------------------------------------------------------------------------------------------
void Window::submitFrameAndWait()
{
    _renderThread.enqueueFrame( [this] { paintFrame(); } );
    _renderThread.waitFrameComplete();
}

// ------------------------------------------------------------------------------------------------
void Window::paintFrame()
{
    const uint64_t pending = _pendingResize.exchange( 0, std::memory_order_acquire );
    if( pending != 0 )
    {
        const auto [fbWidth, fbHeight] = unpackSize( pending );
        applyFrameBufferResize( fbWidth, fbHeight );
        onResizeEvent( static_cast<uint32_t>( fbWidth ),
                       static_cast<uint32_t>( fbHeight ) );
    }

    _renderingContext->beginFrame();
    {
        onPaintEvent();
    }
    _renderingContext->endFrame();

    _renderingContext->commands().blitToScreen( *_renderTarget );
}

// ------------------------------------------------------------------------------------------------
void Window::applyFrameBufferResize( int width, int height )
{
    _renderTarget->resize( width, height );
}

// ------------------------------------------------------------------------------------------------
void Window::postResize( int width, int height ) noexcept
{
    _pendingResize.store( packSize( width, height ), std::memory_order_release );
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