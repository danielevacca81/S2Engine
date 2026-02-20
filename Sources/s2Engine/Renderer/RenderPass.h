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




/*

struct S2ENGINE_API PickResult
{
    bool hit = false;
    MeshHandle mesh = InvalidHandle;
    int32_t objectID = RenderCore::PickerConstants::kClearValue;
};

// Callback invocata quando il picking è completato
using PickCallback = std::function<void(const PickResult&)>;

class S2ENGINE_API PickingPass : public RenderPass
{
public:
    void initialize(ResourceManager& resourceManager) override;
    void execute(const CommandBuffer& queue, FrameData& frameData) override;
    const std::string& name() const override { return _name; }

    // ---- SAFE DEFERRED PICKING ----
    // Accoda una richiesta di picking che verrà eseguita nel prossimo frame
    void requestPick(const Math::ivec2& screenPos, PickCallback callback);

    // Ridimensiona il framebuffer
    void resize(uint32_t width, uint32_t height);

private:
    void processPendingPickRequests(const CommandBuffer& queue, FrameData& frameData);
    void renderObjectIDs(const CommandBuffer& queue, FrameData& frameData);

    struct PickRequest
    {
        Math::ivec2 screenPos;
        PickCallback callback;
    };

    ResourceManager* _resourceManager = nullptr;
    std::string _name = "PickingPass";

    RenderCore::Picker _picker;
    RenderCore::FrameBufferPtr _pickingTarget;
    RenderCore::Texture2DPtr _depthTexture;
    RenderCore::ProgramPtr _pickingShader;

    std::unordered_map<int32_t, MeshHandle> _objectIDToMesh;
    int32_t _nextObjectID = 1;

    // Coda di richieste di picking pendenti (thread-safe)
    std::vector<PickRequest> _pendingPickRequests;
    std::mutex _pickRequestsMutex;
};


*/


} // namespace Renderer
} // namespace s2

#endif // S2_RENDERER_RENDERPASS_H