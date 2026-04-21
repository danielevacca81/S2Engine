// UIPass.h
//
#ifndef UI_UIPASS_H
#define UI_UIPASS_H

#include "s2Engine_API.h"

#include "Renderer/RenderPass.h"

#include "RenderCore/Shader.h"
#include "RenderCore/Texture.h"
#include "RenderCore/VertexArray.h"
#include "RenderCore/GPUBufferObject.h"

namespace s2 {
namespace Renderer {

class ResourceManager;
class CommandBuffer;
struct FrameData;

// ================================================================================================
// UIPass
//
// A RenderPass that renders Dear ImGui draw data using the RenderCore API.
// Fetches draw data directly from ImGui (via ImGui::GetDrawData()) and renders it using
// a custom shader and ImGui font texture.
//
// Note: This pass is designed to be used as the final pass in the render pipeline, after all 3D rendering is done.
//
// ================================================================================================
class S2ENGINE_API UIPass final : public RenderPass
{
public:
    UIPass();
    // RenderPass interface
    const std::string& name() const override;

private:
    void createShader();
    void createFontTexture();

protected:
    void execute( const RenderCore::RenderBackend& rendererBackend,
                  const ResourceManager& resourceManager,
                  const CommandBuffer& queue,
				  FrameData& frameData ) override;


private:
    std::string _name { "s2Engine.UIPass" };

    // GPU resources
    RenderCore::ShaderPtr          _shader;
    RenderCore::Texture2DPtr       _fontTexture;
    RenderCore::VertexArrayPtr     _vao;
    RenderCore::GPUBufferObjectPtr _vbo;  // persistent interleaved vertex buffer (shared by all 3 attributes)
};

} // namespace Renderer
} // namespace s2

#endif // UI_UIPASS_H