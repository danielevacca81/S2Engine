// RenderPass.h
//
#ifndef S2_RENDERER_RENDERPASS_H
#define S2_RENDERER_RENDERPASS_H

#include "s2Engine_API.h"

#include <string>
#include <memory>

namespace s2 {
namespace Renderer {

class CommandBuffer;
struct FrameData;
class ResourceManager;

/**
 * Abstract base class for render passes.
 * A render pass performs a specific rendering operation (forward, deferred, shadow, post-process, etc.)
 */
class RenderPass;
using RenderPassPtr = std::shared_ptr<RenderPass>;

class S2ENGINE_API RenderPass
{
public:
    virtual ~RenderPass() = default;
    virtual void initialize( ResourceManager& resourceManager ) = 0;

    virtual void execute( const CommandBuffer& queue, FrameData& frameData ) = 0;

    virtual const std::string& name() const = 0;

    bool isEnabled() const { return _enabled; }
    void setEnabled( bool enabled ) { _enabled = enabled; }

protected:
    bool _enabled = true;
};


/**
 * Traditional forward rendering pass.
 * Renders geometry directly to the framebuffer with lighting in the fragment shader.
 */
class S2ENGINE_API ForwardPass : public RenderPass
{
public:
    void initialize( ResourceManager& resourceManager ) override;
    void execute( const CommandBuffer& queue, FrameData& context ) override;
    const std::string& name() const override;

private:
	ResourceManager* _resourceManager { nullptr };
    // Statistics for the current frame
    struct Stats
    {
        size_t drawCalls = 0;
        size_t triangles = 0;
        size_t vertices = 0;
    } _stats;


    std::string _name = "ForwardPass";
};


} // namespace Renderer
} // namespace s2

#endif // S2_RENDERER_RENDERPASS_H