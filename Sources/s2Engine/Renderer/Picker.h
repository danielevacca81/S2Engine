// Picker.h
//
#ifndef S2_RENDERER_PICKER_H
#define S2_RENDERER_PICKER_H

#include "s2Engine_API.h"

#include "PickPass.h"
#include "FrameData.h"
#include "Renderer.h"

#include "Math/Math.h"

#include "sigslot/signal.hpp"

#include <cstdint>
#include <functional>

namespace s2 {
namespace Renderer {

// ------------------------------------------------------------------------------------------------
// PickResult
// ------------------------------------------------------------------------------------------------
struct S2ENGINE_API PickResult
{
    uint32_t    objectID    { 0 };
    uint32_t    primitiveID { 0 };
    Math::ivec2 screenPos   { 0, 0 };

    bool isHit() const { return objectID != 0; }
};

// ------------------------------------------------------------------------------------------------
// Picker: Decoupled GPU picking
//
// Design:
//   - Picker connects to Renderer::onRenderCompleted in its constructor.
//   - pickObjectAt() stores screen coordinates from an input event (no GL context required).
//   - When the renderer fires onRenderCompleted GL context still current, Picker reads from the pick
//     RenderTarget in frameData and invokes user callback.
//   - PickPass has zero knowledge of Picker.
//
// Usage:
//   pipeline.addPass( std::make_shared<PickPass>() );
//   Picker picker( renderer );
//
//   picker.onObjectHit( []( const PickResult& r )
//   {
//       if( r.isHit() ) selectEntity( r.objectID );
//   });
//
//   // Input event (no GL context required)
//   picker.requestPickAt( mousePos );
//
//   // After renderer.endFrame() -> user callback is called internally with the result.
// ------------------------------------------------------------------------------------------------
class S2ENGINE_API Picker
{
public:
    explicit Picker( Renderer& renderer );

    void setEnabled( bool enabled ) { _enabled = enabled; }
    bool isEnabled() const          { return _enabled; }

    // Register a pick request from an input event.
    // No GL context required. Overwrites any pending unresolved request.
    void pickObjectAt( const Math::ivec2& screenPos );
    
	// Register a callback to be invoked when a pick result is resolved.
    void onObjectHit( const std::function<void( const PickResult& )>& callback );

private:
	void onRenderCompleted( const FrameData& frameData ); // internal slot connected to Renderer::onRenderCompleted

private:
    bool        _enabled           { true };
    bool        _hasPendingRequest { false };
    Math::ivec2 _pendingScreenPos  { 0, 0 };
	std::function<void( const PickResult& )> _hitCallback; // invoked when a pick result is resolved
};

} // namespace Renderer
} // namespace s2

#endif // !S2_RENDERER_PICKER_H