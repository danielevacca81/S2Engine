// RenderPass.h
//
#ifndef S2_RENDERER_RENDERPASS_H
#define S2_RENDERER_RENDERPASS_H

#include "s2Engine_API.h"

#include "RenderCommand.h"
#include "ResourceManager.h"

#include "RenderCore/VertexData.h"

#include <string>

namespace s2 {
namespace Renderer {

class CommandBuffer;
struct FrameData;

// ================================================================================================
// RenderPass: Abstract base class for rendering passes
// ================================================================================================
class RenderPass;
using RenderPassPtr = std::shared_ptr<RenderPass>;

class S2ENGINE_API RenderPass
{
public:
    virtual ~RenderPass() = default;

    // Initialize pass with resource manager
    virtual void initialize( ResourceManager& resourceManager ) = 0;

    // Execute pass with command buffer and frame data
    virtual void execute( const CommandBuffer& queue, FrameData& frameData, const RenderCore::Context* ctx ) = 0;

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
    Stats _stats;
    bool  _enabled { true };
};

// ================================================================================================
// ForwardPass: Standard forward rendering pass (DSA + Bindless)
// ================================================================================================

class S2ENGINE_API ForwardPass : public RenderPass
{
public:
    void initialize( ResourceManager& resourceManager ) override;
    void execute( const CommandBuffer& queue, FrameData& frameData, const RenderCore::Context* ctx ) override;
    const std::string& name() const override;

private:
    ResourceManager* _resourceManager { nullptr };
    std::string      _name { "ForwardPass" };
};

} // namespace Renderer
} // namespace s2

#endif // !S2_RENDERER_RENDERPASS_H