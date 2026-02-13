// RenderPipeline.h
//
#ifndef S2_RENDERER_RENDERPIPELINE_H
#define S2_RENDERER_RENDERPIPELINE_H

#include "s2Engine_API.h"
#include "RenderPass.h"

#include <vector>
#include <memory>

namespace s2 {
namespace Renderer {

class CommandBuffer;
struct FrameData;

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
  !- PostProcessPass
       |- Read(lit scene)
       !- Apply(bloom, tonemapping, etc.)
*/



/**
 * Manages the sequence of render passes.
 * Different rendering techniques (Forward, Deferred) use different pipelines.
 */
class S2ENGINE_API RenderPipeline
{
public:
    static RenderPipeline createForwardPipeline();
    static RenderPipeline createDeferredPipeline();

public:
    RenderPipeline() = default;
    ~RenderPipeline() = default;

    //// Non-copyable
    //RenderPipeline( const RenderPipeline& ) = delete;
    //RenderPipeline& operator=( const RenderPipeline& ) = delete;

    void initialize();
    void clear();


    void addPass( const RenderPassPtr &pass );
    void removePass( const std::string& name );
    RenderPassPtr findPass( const std::string& name ) const;

    void execute( const CommandBuffer& cmd, FrameData& frameData );

private:
    std::vector<RenderPassPtr> _passes;
};

} // namespace Renderer
} // namespace s2

#endif // S2_RENDERER_RENDERPIPELINE_H