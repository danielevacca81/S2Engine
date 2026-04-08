// PickPass.h
//
#ifndef S2_RENDERER_PICKPASS_H
#define S2_RENDERER_PICKPASS_H

#include "s2Engine_API.h"

#include "RenderPass.h"

#include "RenderCore/Shader.h"
#include "RenderCore/RenderTarget.h"
#include "RenderCore/Texture.h"

#include <memory>
#include <string>

namespace s2 {
namespace Renderer {

// ================================================================================================
// PickPass: Optional render pass for GPU object picking.
//
// Renders all pickable objects into a dedicated RG32I render target and publishes
// it in FrameData::passData under kPickTargetKey.
//
// The pass has no knowledge of Picker or any consumer - it only writes data.
// Consumers connect to Renderer::onFrameDone to read the target while the GL
// context is still current.
//
// Usage:
//   pipeline.addPass( std::make_shared<ForwardPass>() );
//   pipeline.addPass( std::make_shared<PickPass>() );
// ================================================================================================
class S2ENGINE_API PickPass : public RenderPass
{
public:
    static constexpr const char* kPickTargetKey = "PickPass.RenderTarget";

public:
    PickPass()  = default;
    ~PickPass() override = default;

    void initialize( ResourceManager& resourceManager ) override;
    void execute( const CommandBuffer& queue, FrameData& frameData, const RenderCore::Context* ctx ) override;
    const std::string& name() const override { return _name; }

private:
    void ensurePickTarget( uint32_t width, uint32_t height );

private:
    std::string _name { "PickPass" };

    RenderCore::ShaderPtr                     _pickShader;
    std::unique_ptr<RenderCore::RenderTarget> _pickTarget;
    ResourceManager*                          _resourceManager { nullptr };
};

} // namespace Renderer
} // namespace s2

#endif // !S2_RENDERER_PICKPASS_H