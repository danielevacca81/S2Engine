// ImGuiPass.cpp
//
#include "ImGuiPass.h"

#include "RenderCore/Context.h"
#include "RenderCore/RenderTarget.h"
#include "RenderCore/RenderCommands.h"
#include "RenderCore/ShaderCompiler.h"
#include "RenderCore/DrawState.h"
#include "RenderCore/PrimitiveType.h"
#include "RenderCore/AttributeBuffer.h"
#include "RenderCore/TextureDescription.h"

#include "Math/ProjectionTransform.h"

#include "Renderer/FrameData.h"

#include "imgui.h"

#include <stdexcept>

using namespace s2;
using namespace s2::UI;
using namespace s2::RenderCore;

// ================================================================================================
// Shaders
// ================================================================================================
static const char* kVertexShader = R"(
#version 460 core

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aUV;
layout(location = 2) in vec4 aColor;

uniform mat4 uProjection;

out vec2 vUV;
out vec4 vColor;

void main()
{
    vUV    = aUV;
    vColor = aColor;
    gl_Position = uProjection * vec4( aPos, 0.0, 1.0 );
}
)";

static const char* kFragmentShader = R"(
#version 460 core

in vec2 vUV;
in vec4 vColor;

uniform sampler2D uTexture;

layout(location = 0) out vec4 FragColor;

void main()
{
    FragColor = vColor * texture( uTexture, vUV );
}
)";

// ================================================================================================
// Construction
// ================================================================================================
ImGuiPass::ImGuiPass() = default;

// ================================================================================================
// Initialization (called once, on render thread with GL context)
// ================================================================================================
void ImGuiPass::initialize( Renderer::ResourceManager& /*resourceManager*/ )
{
    createShader();
    createFontTexture();
}

// ------------------------------------------------------------------------------------------------
const std::string& ImGuiPass::name() const { return _name; }

// ------------------------------------------------------------------------------------------------
void ImGuiPass::createShader()
{
    auto vs = ShaderCompiler::compile( ShaderStageType::Vertex, kVertexShader );
    auto fs = ShaderCompiler::compile( ShaderStageType::Fragment, kFragmentShader );

    if( !vs || !fs )
        throw std::runtime_error( "ImGuiPass: shader compilation failed:\n"
                                  + vs.errorLog + "\n" + fs.errorLog );

    _shader = Shader::New();
    _shader->attachVertexShaderStage( vs.stage );
    _shader->attachFragmentShaderStage( fs.stage );

    auto result = ShaderCompiler::linkShader( _shader, "ImGuiPass" );
    if( !result )
        throw std::runtime_error( "ImGuiPass: shader link failed:\n" + result.errorLog );
}

// ------------------------------------------------------------------------------------------------
void ImGuiPass::createFontTexture()
{
    unsigned char* pixels = nullptr;
    int w = 0, h = 0;
    ImGui::GetIO().Fonts->GetTexDataAsRGBA32( &pixels, &w, &h );

    TextureDescription desc( w, h, TextureFormat::RedGreenBlueAlpha8, false, "ImGuiPass_FontAtlas" );
    _fontTexture = Texture2D::New( desc, pixels );
    _fontTexture->setMinFilter( Texture2D::MinFilter::Linear );
    _fontTexture->setMagFilter( Texture2D::MagFilter::Linear );

    // ImTextureID is ImU64 since v1.91.4 — store the GL texture name directly.
    ImGui::GetIO().Fonts->SetTexID( static_cast<ImTextureID>( _fontTexture->id() ) );
}

// ================================================================================================
// Per-frame buffer management
// ================================================================================================
void ImGuiPass::ensureBuffers( int64_t vtxBytes, int64_t idxBytes )
{
    if( !_vtxBuffer || _vtxBufferSize < vtxBytes )
    {
        _vtxBufferSize = vtxBytes + 4096;
        _vtxBuffer = GPUBufferObject::New( _vtxBufferSize,
                                           GPUBufferObject::Type::ArrayBuffer,
                                           GPUBufferObject::UsageHint::DynamicDraw );
    }

    if( !_idxBuffer || _idxBufferSize < idxBytes )
    {
        _idxBufferSize = idxBytes + 4096;
        _idxBuffer = GPUBufferObject::New( _idxBufferSize,
                                           GPUBufferObject::Type::ElementBuffer,
                                           GPUBufferObject::UsageHint::DynamicDraw );
    }
}

// ================================================================================================
// Execute — fetches draw data directly from ImGui
// ================================================================================================
void ImGuiPass::execute( const Renderer::CommandBuffer& /*queue*/,
                         Renderer::FrameData& frameData,
                         const RenderCore::Context* ctx )
{
    // Fetch draw data directly from ImGui.
    // Contract: Window calls UILayer::endFrame() before the pipeline executes,
    // so ImGui::GetDrawData() is valid here.
    ImDrawData* drawData = ImGui::GetDrawData();
    if( !drawData || drawData->TotalVtxCount == 0 || !frameData.mainTarget )
        return;

    auto& rt   = *frameData.mainTarget;
    auto& cmds = ctx->commands();

    // ----- Upload vertex + index data -----
    const int64_t totalVtxBytes = drawData->TotalVtxCount * static_cast<int64_t>( sizeof( ImDrawVert ) );
    const int64_t totalIdxBytes = drawData->TotalIdxCount * static_cast<int64_t>( sizeof( ImDrawIdx ) );

    ensureBuffers( totalVtxBytes, totalIdxBytes );

    int64_t vtxOff = 0;
    int64_t idxOff = 0;

    for( int n = 0; n < drawData->CmdListsCount; ++n )
    {
        const ImDrawList* cmdList = drawData->CmdLists[n];
        _vtxBuffer->setData( cmdList->VtxBuffer.Data,
                             cmdList->VtxBuffer.Size * sizeof( ImDrawVert ), vtxOff );
        _idxBuffer->setData( cmdList->IdxBuffer.Data,
                             cmdList->IdxBuffer.Size * sizeof( ImDrawIdx ), idxOff );
        vtxOff += cmdList->VtxBuffer.Size * sizeof( ImDrawVert );
        idxOff += cmdList->IdxBuffer.Size * sizeof( ImDrawIdx );
    }

    // ----- Setup VAO -----
    if( !_vao )
        _vao = VertexArray::New( GPUBufferObject::UsageHint::DynamicDraw );

    _vao->setAttribute( 0, AttributeBuffer(
        nullptr, _vtxBufferSize, GPUBufferObject::Type::ArrayBuffer,
        GPUBufferObject::UsageHint::DynamicDraw,
        AttributeBuffer::ComponentDatatype::Float, 2, false,
        offsetof( ImDrawVert, pos ), sizeof( ImDrawVert ) ) );

    _vao->setAttribute( 1, AttributeBuffer(
        nullptr, _vtxBufferSize, GPUBufferObject::Type::ArrayBuffer,
        GPUBufferObject::UsageHint::DynamicDraw,
        AttributeBuffer::ComponentDatatype::Float, 2, false,
        offsetof( ImDrawVert, uv ), sizeof( ImDrawVert ) ) );

    _vao->setAttribute( 2, AttributeBuffer(
        nullptr, _vtxBufferSize, GPUBufferObject::Type::ArrayBuffer,
        GPUBufferObject::UsageHint::DynamicDraw,
        AttributeBuffer::ComponentDatatype::UnsignedByte, 4, true,
        offsetof( ImDrawVert, col ), sizeof( ImDrawVert ) ) );

    // ----- Ortho projection (top-left origin, as ImGui expects) -----
    const double L = static_cast<double>( drawData->DisplayPos.x );
    const double R = static_cast<double>( drawData->DisplayPos.x + drawData->DisplaySize.x );
    const double T = static_cast<double>( drawData->DisplayPos.y );
    const double B = static_cast<double>( drawData->DisplayPos.y + drawData->DisplaySize.y );

    // Note: bottom=B, top=T flips the Y axis so that Y=0 is at the top (ImGui convention).
    const auto projection = Math::ProjectionTransform::createOrthographic( L, R, B, T, -1.0, 1.0 );

    _shader->setUniform( "uProjection", Math::mat4( projection.matrix() ) );
    _shader->setTexture( "uTexture", _fontTexture );

    // ----- Draw state -----
    DrawState ds;
    ds.shader = _shader;
    ds.renderState.faceCulling.enabled = false;
    ds.renderState.depthTest.enabled   = false;
    ds.renderState.depthMask.enabled   = false;
    ds.renderState.blending.enableAll( true );
    ds.renderState.blending.sourceRGBFactor        = Blending::Factor::SourceAlpha;
    ds.renderState.blending.destinationRGBFactor   = Blending::Factor::OneMinusSourceAlpha;
    ds.renderState.blending.sourceAlphaFactor      = Blending::Factor::One;
    ds.renderState.blending.destinationAlphaFactor = Blending::Factor::OneMinusSourceAlpha;
    ds.renderState.blending.rgbEquation   = Blending::Equation::Add;
    ds.renderState.blending.alphaEquation = Blending::Equation::Add;
    ds.viewport.rect = rt.size();
    ds.viewport.scissorTest.enabled = true;

    // ----- Render draw lists -----
    const ImVec2 clipOff   = drawData->DisplayPos;
    const ImVec2 clipScale = drawData->FramebufferScale;
    const int fbHeight = static_cast<int>( drawData->DisplaySize.y * clipScale.y );

    int globalVtxOffset = 0;
    int globalIdxOffset = 0;

    for( int n = 0; n < drawData->CmdListsCount; ++n )
    {
        const ImDrawList* cmdList = drawData->CmdLists[n];

        for( int i = 0; i < cmdList->CmdBuffer.Size; ++i )
        {
            const ImDrawCmd& pcmd = cmdList->CmdBuffer[i];

            if( pcmd.UserCallback )
            {
                pcmd.UserCallback( cmdList, &pcmd );
                continue;
            }

            const float cx = ( pcmd.ClipRect.x - clipOff.x ) * clipScale.x;
            const float cy = ( pcmd.ClipRect.y - clipOff.y ) * clipScale.y;
            const float cw = ( pcmd.ClipRect.z - clipOff.x ) * clipScale.x;
            const float ch = ( pcmd.ClipRect.w - clipOff.y ) * clipScale.y;

            if( cw <= cx || ch <= cy )
                continue;

            ds.viewport.scissorTest.rect = Math::irect(
                static_cast<int>( cx ),
                fbHeight - static_cast<int>( ch ),
                static_cast<int>( cw - cx ),
                static_cast<int>( ch - cy ) );

            // Per-command texture override.
            // ImTextureID stores the GL texture name (unsigned int -> ImU64).
            // The font atlas texture was registered via SetTexID( fontTexture->id() ).
            // For user textures, the app must pass the GL name via ImTextureID as well.
            const auto texID = static_cast<unsigned int>( pcmd.GetTexID() );
            if( texID != _fontTexture->id() )
                _shader->setUniform( "uTexture", texID );
            else
                _shader->setTexture( "uTexture", _fontTexture );

            cmds.draw( rt, PrimitiveType::Triangles, _vao, ds );
            _stats.drawCalls++;
        }

        globalVtxOffset += cmdList->VtxBuffer.Size;
        globalIdxOffset += cmdList->IdxBuffer.Size;
    }

    _stats = {};
}