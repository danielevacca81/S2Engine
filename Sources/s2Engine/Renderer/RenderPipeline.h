// RenderPipeline.h
//
#ifndef S2_RENDERER_RENDERPIPELINE_H
#define S2_RENDERER_RENDERPIPELINE_H

#include "s2Engine_API.h"

#include "RenderPass.h"

#include <vector>
#include <optional>
#include <memory>

namespace s2 {
namespace RenderCore { class RendererBackend; }
namespace Renderer {

class CommandBuffer;
struct FrameData;
class ResourceManager;

/*
EXAMPLE: FORWARD RENDERING PIPELINE
Pipeline:
!- ForwardPass
    |- Clear(color + depth)
    |- Draw(opaque meshes) - shading immediato
    !- Draw(transparent meshes)

=======================================================================

EXAMPLE: DEFERRED RENDERING PIPELINE
Pipeline:
  |- GeometryPass
  |    |- Bind(G-Buffer)
  |    |- Clear(all)
  |    !- Draw(meshes) - scrive position, normal, albedo
  |
  |- LightingPass
  |    | Bind(screen framebuffer)
  |    |- Read(G-Buffer textures)
  |    !- Draw(fullscreen quad) - calcola lighting
  |
  |- PostProcessPass
  |    |- Read(lit scene)
  |    !- Apply(bloom, tonemapping, etc.)
  |
  !- PickingPass (optional)


*/



/**
 * Manages the sequence of render passes.
 * Different rendering techniques (Forward, Deferred) use different pipelines.
 */
class S2ENGINE_API RenderPipeline
{
public:
	static RenderPipeline createDefaultPipeline();
    static RenderPipeline createForwardPipeline();
    static RenderPipeline createDeferredPipeline();

public:
	// move-only, not copyable
	RenderPipeline() = default;
    RenderPipeline( RenderPipeline&& ) = default;
    RenderPipeline& operator=( RenderPipeline&& ) = default;

    void clear();

    RenderPipeline& addPass( std::unique_ptr<RenderPass> pass );
    RenderPipeline& removePass( const std::string& name );
    std::optional<std::reference_wrapper<RenderPass>> findPass( const std::string& name ) const;

protected:
    void initialize( ResourceManager* resourceManager );
    void execute( const CommandBuffer& cmd, FrameData& frameData, const RenderCore::RendererBackend& backend );

private:
    std::vector< std::unique_ptr<RenderPass>> _passes;

	friend class Renderer; // internal execution and initialization
};

} // namespace Renderer
} // namespace s2

#endif // S2_RENDERER_RENDERPIPELINE_H