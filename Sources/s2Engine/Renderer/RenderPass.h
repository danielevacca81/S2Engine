// RenderPass.h
//
#ifndef S2_RENDERER_RENDERPASS_H
#define S2_RENDERER_RENDERPASS_H

#include "s2Engine_API.h"

#include <string>

namespace s2 {
namespace RenderCore { class RendererBackend; }
namespace Renderer {

class ResourceManager;
class CommandBuffer;
struct FrameData;

// ================================================================================================
// RenderPass: Abstract base class for rendering passes
// ================================================================================================
class S2ENGINE_API RenderPass
{
public:
    virtual ~RenderPass() = default;

    // Get pass name
    virtual const std::string& name() const = 0;

    bool isEnabled() const { return _enabled; }
    void setEnabled( bool enabled ) { _enabled = enabled; }


    // Get pass statistics
    struct Stats
    {
        size_t drawCalls { 0 };
        size_t vertices  { 0 };
        size_t triangles { 0 };
    };

    const Stats& stats() const { return _stats; }

protected:
    // Initialize pass with resource manager
    virtual void initialize( ResourceManager* resourceManager ) { _resourceManager = resourceManager; }

    // Execute pass with command buffer and frame data
    virtual void execute( const CommandBuffer& queue, FrameData& frameData, const RenderCore::RendererBackend& rendererBackend ) = 0;


protected:
    ResourceManager* _resourceManager { nullptr }; // not owned, set during initialization
    Stats _stats;
    bool  _enabled { true };

	friend class RenderPipeline; // RenderPipeline needs access to protected members for initialization and execution
};

} // namespace Renderer
} // namespace s2

#endif // !S2_RENDERER_RENDERPASS_H