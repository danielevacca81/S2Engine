// RenderPass.h
//
#ifndef S2_RENDERER_RENDERPASS_H
#define S2_RENDERER_RENDERPASS_H

#include "s2Engine_API.h"

#include <string>
#include <memory>
#include <unordered_map>

namespace s2 {
namespace RenderCore { class RenderBackend; }
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
	// pass statistics (tbd: more stats like GPU time, memory usage, etc.)
    struct Stats
    {
        size_t drawCalls { 0 };
        size_t vertices { 0 };
        size_t triangles { 0 };
    };

public:
    virtual ~RenderPass() = default;

    // Get pass name
    virtual const std::string& name() const = 0;

    bool isEnabled() const { return _enabled; }
    void setEnabled( bool enabled ) { _enabled = enabled; }

    const Stats& stats() const { return _stats; }

protected:
	// pass is responsible for executing its rendering commands using the provided backend and resource manager
	// cmd contains the commands submitted by the application for this frame, which the pass can consume and translate into GPU commands
	// frameData contains shared data for this frame, which the pass can read and write to share information with other passes
	virtual void execute( const RenderCore::RenderBackend& backend,
						  const ResourceManager& resourceManager,
						  const CommandBuffer& cmd,
						  FrameData& frameData ) = 0;


protected:
	std::weak_ptr<ResourceManager> _resourceManager; // shared resource manager for loading and accessing GPU resources (textures, buffers, shaders)
    Stats                          _stats;
    bool                           _enabled { true };

    friend class RenderPipeline;
};

// convenience typedef
using RenderPasses = std::unordered_map<std::string, std::shared_ptr<RenderPass>>;

} // namespace Renderer
} // namespace s2

#endif // !S2_RENDERER_RENDERPASS_H