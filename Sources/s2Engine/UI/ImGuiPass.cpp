// ImGuiPass.cpp
//
// Renders Dear ImGui draw data using the s2 RenderCore API.
// No direct OpenGL calls — everything goes through RenderCore abstractions.
//
#include "ImGuiPass.h"

#include "Renderer/CommandBuffer.h"
#include "Renderer/FrameData.h"
#include "Renderer/ResourceManager.h"

#include "RenderCore/Context.h"
#include "RenderCore/RenderCommands.h"
#include "RenderCore/RenderTarget.h"
#include "RenderCore/ShaderCompiler.h"
#include "RenderCore/DrawState.h"
#include "RenderCore/PrimitiveType.h"
#include "RenderCore/IndexBuffer.h"
#include "RenderCore/ImageFormat.h"

#include "imgui.h"

#include <cassert>
#include <cstring>

using namespace s2::UI;
using namespace s2::RenderCore;

// ================================================================================================
// Embedded GLSL shaders (OpenGL 4.5 / GLSL 450)
// ================================================================================================

static const char* kImGuiVertexShader = R"(
#version 450 core

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aUV;
layout(location = 2) in vec4 aColor;

uniform mat4 u_ProjectionMatrix;

out vec2 vUV;
out vec4 vColor;

void main()
{
    vUV    = aUV;
    vColor = aColor;
    gl_Position = u_ProjectionMatrix * vec4(aPos, 0.0, 1.0);
}
)";

static const char* kImGuiFragmentShader = R"(
#version 450 core

in vec2 vUV;
in vec4 vColor;

uniform sampler2D u_FontTexture;

layout(location = 0) out vec4 FragColor;

void main()
{
    FragColor = vColor * texture(u_FontTexture, vUV);
}
)";

// ================================================================================================
// ImGuiPass implementation
// ================================================================================================

ImGuiPass::ImGuiPass() = default;

// ------------------------------------------------------------------------------------------------
void ImGuiPass::initialize( Renderer::ResourceManager& /*resourceManager*/ )
{
    createShader();
    createFontTexture();

    // Create VAO with dynamic usage (data changes every frame)
    _vao = VertexArray::New( GPUBufferObject::UsageHint::DynamicDraw );
}

// ------------------------------------------------------------------------------------------------
void ImGuiPass::createShader()
{
    auto vs = ShaderCompiler::compile( ShaderStageType::Vertex,   kImGuiVertexShader );
    auto fs = ShaderCompiler::compile( ShaderStageType::Fragment, kImGuiFragmentShader );

    assert( vs.success && "ImGuiPass: vertex shader compilation failed" );
    assert( fs.success && "ImGuiPass: fragment shader compilation failed" );

    _shader = Shader::New();
    _shader->attachVertexShaderStage( vs.stage );
    _shader->attachFragmentShaderStage( fs.stage );

    auto linkResult = ShaderCompiler::linkShader( _shader, "ImGuiPass" );
    assert( linkResult.success && "ImGuiPass: shader link failed" );

    _shader->setObjectLabel( "ImGuiPass::Shader" );
}

// ------------------------------------------------------------------------------------------------
void ImGuiPass::createFontTexture()
{
    ImGuiIO& io = ImGui::GetIO();

    unsigned char* pixels = nullptr;
    int width  = 0;
    int height = 0;
    io.Fonts->GetTexDataAsRGBA32( &pixels, &width, &height );

    assert( pixels && width > 0 && height > 0 && "ImGuiPass: font atlas build failed" );

    TextureDescription desc( width, height, TextureFormat::RedGreenBlueAlpha8, false, "ImGuiPass::FontAtlas" );
    _fontTexture = Texture2D::New( desc, pixels );

    _fontTexture->setMinFilter( Texture2D::MinFilter::Linear );
    _fontTexture->setMagFilter( Texture2D::MagFilter::Linear );
    _fontTexture->setWrapS( Texture2D::WrapMode::ClampToEdge );
    _fontTexture->setWrapT( Texture2D::WrapMode::ClampToEdge );

    io.Fonts->SetTexID( reinterpret_cast<ImTextureID>( _fontTexture.get() ) );
}

// ------------------------------------------------------------------------------------------------
// Ensures the shared VBO has enough capacity. If the buffer needs to grow,
// a new GPUBufferObject is created and all three interleaved attributes are
// re-attached to the VAO via setAttribute (DSA). When the existing capacity
// is sufficient the same GPU buffer is reused — only the data is updated.
// ------------------------------------------------------------------------------------------------
static GPUBufferObjectPtr ensureVertexBuffer( const VertexArrayPtr& vao,
                                              GPUBufferObjectPtr    currentVBO,
                                              int64_t               requiredBytes )
{
    constexpr int64_t stride = sizeof( ImDrawVert );

    if( currentVBO && currentVBO->size() >= requiredBytes )
    {
        // Existing buffer is large enough — invalidate and reuse
        currentVBO->invalidate();
        return currentVBO;
    }

    // Grow with 25 % headroom to avoid reallocating every frame
    const int64_t allocBytes = requiredBytes + requiredBytes / 4;

    auto vbo = GPUBufferObject::New( allocBytes,
                                     GPUBufferObject::Type::ArrayBuffer,
                                     GPUBufferObject::UsageHint::DynamicDraw );
    vbo->setObjectLabel( "ImGuiPass::VBO" );

    // Re-attach all three interleaved attributes to the new buffer (DSA).
    // All share the same VBO with bufferOffset = 0 and stride = sizeof(ImDrawVert).
    // Each attribute specifies its own relativeOffset = offsetof(ImDrawVert, field).
    //
    // location 0: aPos  — 2 floats at relativeOffset 0
    AttributeBuffer posAttr( vbo,
                             AttributeBuffer::ComponentDatatype::Float, 2,
                             false,
                             /*bufferOffset*/   0,
                             /*relativeOffset*/ static_cast<int64_t>( offsetof( ImDrawVert, pos ) ),
                             stride );

    // location 1: aUV   — 2 floats at relativeOffset 8
    AttributeBuffer uvAttr( vbo,
                            AttributeBuffer::ComponentDatatype::Float, 2,
                            false,
                            /*bufferOffset*/   0,
                            /*relativeOffset*/ static_cast<int64_t>( offsetof( ImDrawVert, uv ) ),
                            stride );

    // location 2: aColor — 4 unsigned bytes, normalized to [0,1], at relativeOffset 16
    AttributeBuffer colAttr( vbo,
                             AttributeBuffer::ComponentDatatype::UnsignedByte, 4,
                             true,
                             /*bufferOffset*/   0,
                             /*relativeOffset*/ static_cast<int64_t>( offsetof( ImDrawVert, col ) ),
                             stride );

    vao->setAttribute( 0, posAttr );
    vao->setAttribute( 1, uvAttr );
    vao->setAttribute( 2, colAttr );

    return vbo;
}

// ------------------------------------------------------------------------------------------------
// Ensures the index buffer inside the VAO has enough capacity.
// Re-creates the IndexBuffer only when it must grow.
// ------------------------------------------------------------------------------------------------
static void ensureIndexBuffer( const VertexArrayPtr& vao,
                               int64_t               requiredBytes )
{
    constexpr IndexBuffer::IndexDataType idxType =
        sizeof( ImDrawIdx ) == 2 ? IndexBuffer::IndexDataType::UnsignedShort
                                 : IndexBuffer::IndexDataType::UnsignedInt;

    if( vao->isIndexed() )
    {
        auto& ib = vao->indexBuffer();

        if( ib.id() != 0 &&
            static_cast<int64_t>( ib.count() ) * static_cast<int64_t>( sizeof( ImDrawIdx ) ) >= requiredBytes )
            return;

        const int64_t allocBytes = requiredBytes + requiredBytes / 4;
        ib.set( allocBytes, idxType, GPUBufferObject::UsageHint::DynamicDraw );
        vao->setIndexBuffer( ib );
        return;
    }

    const int64_t allocBytes = requiredBytes + requiredBytes / 4;
    IndexBuffer ib( allocBytes, idxType, GPUBufferObject::UsageHint::DynamicDraw );
    vao->setIndexBuffer( ib );
}

// ------------------------------------------------------------------------------------------------
void ImGuiPass::execute( const Renderer::CommandBuffer& /*queue*/,
                         Renderer::FrameData& frameData,
                         const RenderCore::Context* ctx )
{
    ImDrawData* drawData = ImGui::GetDrawData();
    if( !drawData || drawData->TotalVtxCount == 0 )
        return;

    assert( ctx && "ImGuiPass: context must be valid" );
    assert( frameData.mainTarget && "ImGuiPass: mainTarget must be set" );

    auto& renderCommands = ctx->commands();

    // ------------------------------------------------------------------
    // 1. Build orthographic projection from ImGui's display coordinates
    // ------------------------------------------------------------------
    const float L = drawData->DisplayPos.x;
    const float R = drawData->DisplayPos.x + drawData->DisplaySize.x;
    const float T = drawData->DisplayPos.y;
    const float B = drawData->DisplayPos.y + drawData->DisplaySize.y;

    const Math::fmat4 ortho = Math::ortho( L, R, B, T, -1.0f, 1.0f );

    _shader->setUniform( "u_ProjectionMatrix", ortho );
    _shader->setTexture( "u_FontTexture", _fontTexture );

    // ------------------------------------------------------------------
    // 2. Ensure GPU buffers have enough capacity, then upload data
    // ------------------------------------------------------------------
    const int64_t totalVtxBytes = static_cast<int64_t>( drawData->TotalVtxCount ) * sizeof( ImDrawVert );
    const int64_t totalIdxBytes = static_cast<int64_t>( drawData->TotalIdxCount ) * sizeof( ImDrawIdx );

    _vbo = ensureVertexBuffer( _vao, _vbo, totalVtxBytes );
    ensureIndexBuffer( _vao, totalIdxBytes );

    // Stream vertex data — RAII mapped access, unmap is automatic
    _vbo->writeRange<ImDrawVert>( 0, totalVtxBytes, [&]( ImDrawVert* dst ) {
        for( int n = 0; n < drawData->CmdListsCount; ++n )
        {
            const ImDrawList* cmdList = drawData->CmdLists[n];
            std::memcpy( dst, cmdList->VtxBuffer.Data, cmdList->VtxBuffer.Size * sizeof( ImDrawVert ) );
            dst += cmdList->VtxBuffer.Size;
        }
    });

    // Stream index data — RAII mapped access, unmap is automatic
    _vao->indexBuffer().writeAll<ImDrawIdx>( [&]( ImDrawIdx* dst ) {
        for( int n = 0; n < drawData->CmdListsCount; ++n )
        {
            const ImDrawList* cmdList = drawData->CmdLists[n];
            std::memcpy( dst, cmdList->IdxBuffer.Data, cmdList->IdxBuffer.Size * sizeof( ImDrawIdx ) );
            dst += cmdList->IdxBuffer.Size;
        }
    });

    // ------------------------------------------------------------------
    // 3. Setup draw state for ImGui rendering
    // ------------------------------------------------------------------
    DrawState ds;
    ds.shader = _shader;

    ds.renderState.blending.enableAll( true );
    ds.renderState.blending.sourceRGBFactor        = Blending::Factor::SourceAlpha;
    ds.renderState.blending.destinationRGBFactor   = Blending::Factor::OneMinusSourceAlpha;
    ds.renderState.blending.sourceAlphaFactor      = Blending::Factor::SourceAlpha;
    ds.renderState.blending.destinationAlphaFactor = Blending::Factor::OneMinusSourceAlpha;
    ds.renderState.blending.rgbEquation            = Blending::Equation::Add;
    ds.renderState.blending.alphaEquation          = Blending::Equation::Add;

    ds.renderState.depthTest.enabled   = false;
    ds.renderState.depthMask.enabled   = false;
    ds.renderState.faceCulling.enabled = false;
    ds.renderState.stencilTest.enabled = false;
    ds.renderState.primitiveRestart.enabled = false;

    ds.viewport.rect = frameData.mainTarget->size();

    // ------------------------------------------------------------------
    // 4. Iterate draw commands and issue drawRange calls
    // ------------------------------------------------------------------
    const ImVec2 clipOff   = drawData->DisplayPos;
    const ImVec2 clipScale = drawData->FramebufferScale;

    uint32_t globalIdxOffset = 0;
    uint32_t globalVtxOffset = 0;

    for( int n = 0; n < drawData->CmdListsCount; ++n )
    {
        const ImDrawList* cmdList = drawData->CmdLists[n];

        for( int cmdIdx = 0; cmdIdx < cmdList->CmdBuffer.Size; ++cmdIdx )
        {
            const ImDrawCmd& pcmd = cmdList->CmdBuffer[cmdIdx];

            if( pcmd.UserCallback )
            {
                pcmd.UserCallback( cmdList, &pcmd );
                continue;
            }

            const float clipX1 = ( pcmd.ClipRect.x - clipOff.x ) * clipScale.x;
            const float clipY1 = ( pcmd.ClipRect.y - clipOff.y ) * clipScale.y;
            const float clipX2 = ( pcmd.ClipRect.z - clipOff.x ) * clipScale.x;
            const float clipY2 = ( pcmd.ClipRect.w - clipOff.y ) * clipScale.y;

            const int fbWidth  = static_cast<int>( drawData->DisplaySize.x * clipScale.x );
            const int fbHeight = static_cast<int>( drawData->DisplaySize.y * clipScale.y );

            if( clipX1 >= fbWidth || clipY1 >= fbHeight || clipX2 < 0.0f || clipY2 < 0.0f )
                continue;

            const int sx = static_cast<int>( clipX1 );
            const int sy = static_cast<int>( static_cast<float>( fbHeight ) - clipY2 );
            const int sw = static_cast<int>( clipX2 - clipX1 );
            const int sh = static_cast<int>( clipY2 - clipY1 );

            ds.viewport.scissorTest.enabled = true;
            ds.viewport.scissorTest.rect    = Math::irect( sx, sy, sw, sh );

            const uint32_t idxByteOffset = ( pcmd.IdxOffset + globalIdxOffset ) * sizeof( ImDrawIdx );

            renderCommands.drawRange(
                *frameData.mainTarget,
                PrimitiveType::Triangles,
                _vao,
                pcmd.ElemCount,
                idxByteOffset,
                pcmd.VtxOffset + globalVtxOffset,
                ds
            );

            _stats.drawCalls++;
            _stats.triangles += pcmd.ElemCount / 3;
            _stats.vertices  += pcmd.ElemCount;
        }

        globalIdxOffset += cmdList->IdxBuffer.Size;
        globalVtxOffset += cmdList->VtxBuffer.Size;
    }

    _stats = {};
}

// ------------------------------------------------------------------------------------------------
const std::string& ImGuiPass::name() const
{
    return _name;
}