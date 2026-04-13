// Window.cpp
//
#include "Window.h"

#include "InputState.h"
#include "UI/UILayer.h"

#include "RenderCore/Context.h"
#include "RenderCore/RenderTarget.h"
#include "RenderCore/RenderCommands.h"

#include "GLFW/glfw3.h"
#include "imgui_impl_glfw.h"

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
        glfwWindowHint( GLFW_OPENGL_PROFILE,        GLFW_OPENGL_COMPAT_PROFILE );

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
    static void installCallbacks( GLFWwindow* w )
    {
        glfwSetWindowCloseCallback( w, []( GLFWwindow* w )
        {
            static_cast<Window*>( glfwGetWindowUserPointer( w ) )->onCloseEvent();
        } );

        glfwSetCursorPosCallback( w, []( GLFWwindow* w, double x, double y )
        {
            // Forward to ImGui first
            ImGui_ImplGlfw_CursorPosCallback( w, x, y );

            auto* self = static_cast<Window*>( glfwGetWindowUserPointer( w ) );
            auto* impl = self->_impl.get();
            impl->input->updateMouseState(
                Input::MouseMoveEvent{ x, static_cast<double>( impl->height() ) - y - 1 } );
            self->onMouseMoveEvent( impl->input->mouseState() );
        } );

        glfwSetMouseButtonCallback( w, []( GLFWwindow* w, int button, int action, int mods )
        {
            // Forward to ImGui first
            ImGui_ImplGlfw_MouseButtonCallback( w, button, action, mods );

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
            // Forward to ImGui first
            ImGui_ImplGlfw_ScrollCallback( w, x, y );

            auto* self = static_cast<Window*>( glfwGetWindowUserPointer( w ) );
            auto* impl = self->_impl.get();
            impl->input->updateMouseState( Input::MouseWheelEvent{ x, y } );
            self->onMouseScrollEvent( impl->input->mouseState() );
        } );

        glfwSetKeyCallback( w, []( GLFWwindow* w, int key, int scancode, int action, int mods )
        {
            // Forward to ImGui � keyboard input is event-driven only
            ImGui_ImplGlfw_KeyCallback( w, key, scancode, action, mods );
        } );

        glfwSetCharCallback( w, []( GLFWwindow* w, unsigned int c )
        {
            // Forward to ImGui � text input is event-driven only
            ImGui_ImplGlfw_CharCallback( w, c );
        } );

        glfwSetWindowFocusCallback( w, []( GLFWwindow* w, int focused )
        {
            // Forward to ImGui � needed to suppress input when unfocused
            ImGui_ImplGlfw_WindowFocusCallback( w, focused );
        } );

        glfwSetCursorEnterCallback( w, []( GLFWwindow* w, int entered )
        {
            // Forward to ImGui � needed for mouse leave/enter tracking
            ImGui_ImplGlfw_CursorEnterCallback( w, entered );
        } );

        glfwSetMonitorCallback( []( GLFWmonitor* monitor, int event )
        {
            ImGui_ImplGlfw_MonitorCallback( monitor, event );
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
    {
        GLFWwindow* ctx = glfwGetCurrentContext();
        std::cout << "Current GLFW Context: " << std::hex << ctx << '\n';
    }

    _renderingContext = std::make_unique<RenderCore::Context>();
    _renderTarget     = std::make_unique<RenderCore::RenderTarget>();
}

// ------------------------------------------------------------------------------------------------
Window::~Window()
{
    stopRenderThread();
    _uiLayer.reset();
    _impl.reset();
}

// ================================================================================================
// Public API � delegates to Impl
// ================================================================================================
uint32_t Window::width()  const { return _impl->width();  }
uint32_t Window::height() const { return _impl->height(); }

void* Window::nativeHandle() const noexcept { return _impl ? _impl->window : nullptr; }
bool  Window::shouldClose()  const          { return _impl->shouldClose();  }
void  Window::swapBuffers()                 { _impl->swapBuffers(); }
void  Window::makeCurrent()                 { _impl->makeCurrent(); }

void Window::framebufferSize( int& w, int& h ) const { _impl->framebufferSize( w, h ); }

// ================================================================================================
// UILayer
// ================================================================================================
void Window::setUILayer( std::unique_ptr<UI::UILayer> layer ) noexcept
{
    _uiLayer = std::move( layer );
    _uiLayer->init( _impl->window );
}

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

        if( _uiLayer )
            _uiLayer->shutdown();

        _renderTarget.reset();
        _renderingContext.reset();

        glfwMakeContextCurrent( nullptr );
    } );
}

// ================================================================================================
// Frame
// ================================================================================================
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
        onDraw();

        if( _uiLayer )
            _uiLayer->drawUI( [this] { onDrawUI(); } );
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

// ================================================================================================
// UI helpers
// ================================================================================================
bool Window::uiWantCaptureMouse() const noexcept
{
    return _uiLayer && _uiLayer->wantCaptureMouse();
}

// ------------------------------------------------------------------------------------------------
bool Window::uiWantCaptureKeyboard() const noexcept
{
    return _uiLayer && _uiLayer->wantCaptureKeyboard();
}