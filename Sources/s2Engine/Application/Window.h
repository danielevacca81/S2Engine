// Window.h
//
#ifndef APPLICATION_WINDOW_H
#define APPLICATION_WINDOW_H

#include "s2Engine_API.h"

#include "Input.h"
#include "RenderThread.h"
#include "WindowParameters.h"

#include <atomic>
#include <string>
#include <memory>


namespace s2 {

namespace RenderCore {
class Context;
class RenderTarget;
}

namespace Renderer { class Renderer; }

class Application;
class UILayer;

class S2ENGINE_API Window
{
public:
    Window();
    Window( const std::string& name, int width, int height, const WindowParameters& params );
    virtual ~Window();

    Window( const Window& )            = delete;
    Window& operator=( const Window& ) = delete;

    uint32_t width()  const;
    uint32_t height() const;

    void* nativeHandle() const noexcept;

    // @todo:
    // isMinimized
    // isFullScreen
    // isMaximized
    // ...
    virtual void onInitializeEvent()                                    {}
    virtual void onShutdownEvent()                                      {}
    virtual void onMouseMoveEvent( const Input::MouseState& ms )        {}
    virtual void onMouseDoubleClickEvent( const Input::MouseState& ms ) {}
    virtual void onMouseButtonEvent( const Input::MouseState& ms )      {}
    virtual void onMouseScrollEvent( const Input::MouseState& ms )      {}
    virtual void onKeyboardEvent( const Input::KeyboardState& ks )      {}
    virtual void onCloseEvent()                                         {}
    virtual void onResizeEvent( uint32_t width, uint32_t height )       {}

    virtual void onDraw()                                               {}

protected:
    void makeCurrent();

    bool uiWantCaptureMouse()    const noexcept;
    bool uiWantCaptureKeyboard() const noexcept;

protected:
    std::unique_ptr<RenderCore::Context>      _renderingContext;
	std::unique_ptr<RenderCore::RenderTarget> _mainRenderTarget; // default renderTarget for the window
	std::unique_ptr<Renderer::Renderer>       _renderer;
	std::unique_ptr<UILayer>                  _ui;

private:
    void startRenderThread();
    void stopRenderThread();
    void submitFrameAndWait();
    void drawCurrentFrame();
    void applyFrameBufferResize( int width, int height );
    void postResize( int width, int height ) noexcept;
    bool shouldClose()  const;
    void swapBuffers();

    void framebufferSize( int& width, int& height ) const;

private:
    std::atomic<uint64_t> _pendingResize { 0 };
    RenderThread          _renderThread;

    struct Impl;
    std::unique_ptr<Impl> _impl;

    friend class Application;
};

} // namespace s2

#endif