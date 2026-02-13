// FrameData.h (rinominato da RenderContext.h)
//
#ifndef S2_RENDERER_FRAMEDATA_H
#define S2_RENDERER_FRAMEDATA_H

#include "s2Engine_API.h"

#include <unordered_map>
#include <string>

namespace s2 {
namespace RenderCore {
    class RenderTarget;
}

namespace Renderer {

// Per-frame rendering data shared between render passes.
struct S2ENGINE_API FrameData
{
    // Main render target for this frame
    RenderCore::RenderTarget* mainTarget = nullptr;

	Math::dmat4 cameraViewMatrix;       // View matrix from the camera
	Math::dmat4 cameraProjectionMatrix; // Projection matrix from the camera

	// @todo: consider having a separate struct for camera, lights, etc.
    // and pass it to render passes instead of using passData map
    
    // Shared data between passes (e.g., shadow maps, intermediate textures)
    std::unordered_map<std::string, void*> passData;
};

} // namespace Renderer
} // namespace s2

#endif // !S2_RENDERER_FRAMEDATA_H