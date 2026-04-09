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

namespace Input { class InputWrapper; }

class Application;

class S2ENGINE_API Window
{
public:
    Window();
    Window( const std::string& name, int width, int height, const WindowParameters& params );
    virtual ~Window();

    uint32_t width()  const;
    uint32_t height() const;

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
    virtual void onCloseEvent()                                         {}
    virtual void onPaintEvent()                                         {}
    virtual void onResizeEvent( uint32_t width, uint32_t height )       {}

protected:
    void makeCurrent();

protected:
    std::unique_ptr<RenderCore::Context>      _renderingContext;
    std::unique_ptr<RenderCore::RenderTarget> _renderTarget;

private:
    void startRenderThread();
    void stopRenderThread();
    void submitFrameAndWait();
    void paintFrame();
    void applyFrameBufferResize( int width, int height );
    void postResize( int width, int height ) noexcept;

private:
    std::atomic<uint64_t> _pendingResize { 0 };

    RenderThread         _renderThread;
    void*                _handle       = nullptr;
    Input::InputWrapper* _inputWrapper = nullptr;

    friend class Application;
};

} // namespace s2

#endif