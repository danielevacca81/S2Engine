// FrameData.h
//
#ifndef S2_RENDERER_FRAMEDATA_H
#define S2_RENDERER_FRAMEDATA_H

#include "s2Engine_API.h"

#include "RenderPipeline.h"
#include "View.h"

#include <unordered_map>
#include <string>
#include <vector>
#include <any>

namespace s2 {
namespace RenderCore { class RenderTarget;}
namespace Renderer {

// Per-frame rendering data shared between render passes.
struct S2ENGINE_API FrameData
{
	RenderPipeline            renderPasses;           // The passes to execute for this frame (consumed after execution)

    RenderCore::RenderTarget* renderTarget = nullptr; // Main render target for this frame

    View                      view;                   // Camera view and projection for this frame


    // Shared data between passes (e.g., shadow maps, intermediate textures)
    std::unordered_map<std::string, std::any> passData;
};

} // namespace Renderer
} // namespace s2

#endif // !S2_RENDERER_FRAMEDATA_H