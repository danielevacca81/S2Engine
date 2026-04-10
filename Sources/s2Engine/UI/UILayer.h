// UILayer.h
//
#ifndef UI_UILAYER_H
#define UI_UILAYER_H

#include "s2Engine_API.h"

#include <memory>
#include <functional>
#include <unordered_map>
#include <string>

namespace s2 {
namespace UI {

// ------------------------------------------------------------------------------------------------
// UILayer
//
// Abstract interface for a UI system lifecycle.
// Hides the concrete UI library (ImGui, Nuklear, etc.) behind an opaque API.
//
// Responsibilities:
//   - Create / destroy the UI library context         (init / shutdown)
//   - Bracket each frame for UI building code         (beginFrame / endFrame)
//   - Report whether the UI wants to consume input    (wantCaptureMouse/Keyboard)
//
// What UILayer does NOT do:
//   - Rendering.  A concrete RenderPass (e.g. ImGuiPass) handles that.
//     The pass knows which UI library is in use and fetches the draw
//     data directly from the library after endFrame().
//
// - Window owns a UILayer* and calls beginFrame/endFrame each frame.
// - Application code overrides Window::onUIEvent() to build the UI using
//   the concrete library's API directly (e.g. ImGui::Begin, etc.).
// ------------------------------------------------------------------------------------------------
class S2ENGINE_API UILayer
{
public:
	std::unordered_map<std::string, void*> uiData; // user data storage for UI-related pointers (e.g. ImGuiContext*)

public:
    virtual ~UILayer() = default;

    /// Initialize the UI system.
    /// @param windowHandle  Opaque platform window handle.
    virtual void init( void* windowHandle ) = 0;

    /// Shutdown the UI system and release all resources.
    virtual void shutdown() = 0;

	/// Bracket the UI building code for each frame.
    virtual void drawUI( const std::function<void()> &draw) = 0;

    /// Returns true after a successful init() and before shutdown().
    virtual bool isInitialized() const noexcept = 0;

    /// Returns true if the UI system wants to consume mouse input.
    virtual bool wantCaptureMouse() const noexcept = 0;

    /// Returns true if the UI system wants to consume keyboard input.
    virtual bool wantCaptureKeyboard() const noexcept = 0;
};

/// Factory: creates the default UILayer implementation for the current build.
S2ENGINE_API std::unique_ptr<UILayer> createUILayer();

} // namespace UI
} // namespace s2

#endif // UI_UILAYER_H