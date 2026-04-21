// RenderPipeline.h
//
#ifndef S2_RENDERER_RENDERPIPELINE_H
#define S2_RENDERER_RENDERPIPELINE_H

#include "s2Engine_API.h"

#include "RenderPass.h"

#include <vector>
#include <initializer_list>
#include <memory>

namespace s2 {
namespace RenderCore { class RenderBackend; }
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
  * User defines pipelines in application code as sequence of passes.
  * Passes can be shared between pipelines (e.g., a common shadow pass used in both forward and deferred pipelines).
  * 
 */
class S2ENGINE_API RenderPipeline
{
public:
	RenderPipeline() = default;
	RenderPipeline( const std::initializer_list<std::shared_ptr<RenderPass>>& passes ) 
        : _passes( passes ) 
    {}
    
    void clear();
	bool isEmpty() const { return _passes.empty(); }

    RenderPipeline& addPass( const std::shared_ptr<RenderPass> &pass );
    RenderPipeline& removePass( const std::string& name );
    std::shared_ptr<RenderPass> findPass( const std::string& name ) const;

protected:
    void execute( const RenderCore::RenderBackend& backend,
				  const ResourceManager& resourceManager,
                  const CommandBuffer& cmd,
                  FrameData& frameData );

private:
    std::vector< std::shared_ptr<RenderPass>> _passes;

	friend class Renderer; // internal execution
};

} // namespace Renderer
} // namespace s2

#endif // S2_RENDERER_RENDERPIPELINE_H