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

//namespace Input { class Input; }
namespace UI    { class UILayer; }

class Application;

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

    bool  shouldClose()  const;
    void  swapBuffers();

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

    /// Override to build UI widgets each frame.
    /// Called between UILayer::beginFrame() and UILayer::endFrame()
    /// only if a UILayer has been installed.
    virtual void onDrawUI()                                             {}

    /// Install a UI layer. Ownership is transferred to Window.
    /// Pass nullptr to remove the current UI layer.
    /// Must be called before startRenderThread().
    void setUILayer( std::unique_ptr<UI::UILayer> layer ) noexcept;

    /// Access the UI layer (may be null if none was installed).
    UI::UILayer* uiLayer() noexcept { return _uiLayer.get(); }

protected:
    void makeCurrent();

    bool uiWantCaptureMouse()    const noexcept;
    bool uiWantCaptureKeyboard() const noexcept;

protected:
    std::unique_ptr<RenderCore::Context>      _renderingContext;
    std::unique_ptr<RenderCore::RenderTarget> _renderTarget;

private:
    void startRenderThread();
    void stopRenderThread();
    void submitFrameAndWait();
    void drawCurrentFrame();
    void applyFrameBufferResize( int width, int height );
    void postResize( int width, int height ) noexcept;

    void framebufferSize( int& width, int& height ) const;

private:
    std::atomic<uint64_t> _pendingResize { 0 };

    RenderThread                   _renderThread;
    std::unique_ptr<UI::UILayer>   _uiLayer;

    struct Impl;
    std::unique_ptr<Impl> _impl;

    friend class Application;
};

} // namespace s2

#endif