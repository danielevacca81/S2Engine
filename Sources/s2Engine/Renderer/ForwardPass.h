// ForwardPass.h
//
#ifndef S2_RENDERER_FORWARDPASS_H
#define S2_RENDERER_FORWARDPASS_H

#include "s2Engine_API.h"

#include "RenderPass.h"

#include <string>

namespace s2 {
namespace Renderer {

class S2ENGINE_API ForwardPass final : public RenderPass
{
public:
    const std::string& name() const override;

protected:
    void initialize( ResourceManager* resourceManager ) override;
    void execute( const CommandBuffer& queue, FrameData& frameData, const RenderCore::RendererBackend& rendererBackend ) override;

private:
    std::string      _name { "s2Engine.ForwardPass" };
};

} // namespace Renderer
} // namespace s2

#endif // !S2_RENDERER_RENDERPASS_H