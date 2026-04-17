// RenderPass.h
//
#ifndef S2_RENDERER_RENDERPASS_H
#define S2_RENDERER_RENDERPASS_H

#include "s2Engine_API.h"

#include <string>
#include <memory>

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
	// pass is responsible for executing its rendering commands using the provided backend and resource manager
	// cmd contains the commands submitted by the application for this frame, which the pass can consume and translate into GPU commands
	// frameData contains shared data for this frame, which the pass can read and write to share information with other passes
	virtual void execute( const RenderCore::RendererBackend& backend,
						  const ResourceManager& resourceManager,
						  const CommandBuffer& cmd,
						  FrameData& frameData ) = 0;


protected:
	std::weak_ptr<ResourceManager> _resourceManager; // shared resource manager for loading and accessing GPU resources (textures, buffers, shaders)
    Stats                          _stats;
    bool                           _enabled { true };

	friend class RenderPipeline; // RenderPipeline needs access to protected members for initialization and execution
};

} // namespace Renderer
} // namespace s2

#endif // !S2_RENDERER_RENDERPASS_H