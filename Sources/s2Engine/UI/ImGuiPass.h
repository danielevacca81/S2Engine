// ImGuiPass.h
//
#ifndef UI_IMGUIPASS_H
#define UI_IMGUIPASS_H

#include "s2Engine_API.h"

#include "Renderer/RenderPass.h"

#include "RenderCore/Shader.h"
#include "RenderCore/Texture.h"
#include "RenderCore/VertexArray.h"
#include "RenderCore/GPUBufferObject.h"

namespace s2 {
namespace UI {

// ================================================================================================
// ImGuiPass
//
// A RenderPass that renders Dear ImGui draw data using the RenderCore API.
// Fetches draw data directly from ImGui (via ImGui::GetDrawData()) — no
// coupling to UILayer at all.
//
// Contract: UILayer::endFrame() must have been called before pipeline.execute()
// so that ImGui::GetDrawData() returns valid data.
//
// Must be the last pass in the pipeline (renders on top of everything).
//
// Usage:
//   pipeline.addPass( std::make_shared<UI::ImGuiPass>() );
// ================================================================================================
class S2ENGINE_API ImGuiPass : public Renderer::RenderPass
{
public:
    ImGuiPass();
    ~ImGuiPass() override = default;

    // RenderPass interface
    void               initialize( Renderer::ResourceManager& resourceManager ) override;
    void               execute( const Renderer::CommandBuffer& queue, Renderer::FrameData& frameData, const RenderCore::Context* ctx ) override;
    const std::string& name() const override;

private:
    void createShader();
    void createFontTexture();

private:
    std::string _name { "ImGuiPass" };

    // GPU resources
    RenderCore::ShaderPtr          _shader;
    RenderCore::Texture2DPtr       _fontTexture;
    RenderCore::VertexArrayPtr     _vao;
    RenderCore::GPUBufferObjectPtr _vbo;  // persistent interleaved vertex buffer (shared by all 3 attributes)
};

} // namespace UI
} // namespace s2

#endif // UI_IMGUIPASS_H