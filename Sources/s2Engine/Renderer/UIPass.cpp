// UIPass.cpp
//
#include "UIPass.h"


#include "RenderCore/OpenGL.h"
#include "Renderer/CommandBuffer.h"
#include "Renderer/FrameData.h"
#include "Renderer/ResourceManager.h"

#include "RenderCore/RenderBackend.h"
#include "RenderCore/RenderTarget.h"
#include "RenderCore/ShaderCompiler.h"
#include "RenderCore/DrawState.h"
#include "RenderCore/PrimitiveType.h"
#include "RenderCore/IndexBuffer.h"
#include "RenderCore/ImageFormat.h"

#include "imgui.h"

#include <cassert>
#include <cstring>
#include <algorithm>

using namespace s2::RenderCore;
using namespace s2::Renderer;

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
#extension GL_ARB_bindless_texture : require
#extension GL_ARB_gpu_shader_int64 : require

in vec2 vUV;
in vec4 vColor;

layout(location = 0) uniform uint64_t u_CurrTextureHandle;

layout(location = 0) out vec4 FragColor;

void main()
{
    // Trasformiamo l'handle numerico in un sampler "al volo" per usarlo con texture()
    FragColor = vColor * texture(sampler2D(u_CurrTextureHandle), vUV);
}
)";

static inline auto usageHint = GPUBufferObject::UsageHint::StreamDraw;
//static inline auto usageHint = GPUBufferObject::UsageHint::StaticDraw;

// ------------------------------------------------------------------------------------------------
UIPass::UIPass()
{
    createShader();
    createFontTexture();

    _vao = VertexArray::New( usageHint );
}

// ------------------------------------------------------------------------------------------------
void UIPass::createShader()
{
    auto vs = ShaderCompiler::compile( ShaderStageType::Vertex,   kImGuiVertexShader );
    auto fs = ShaderCompiler::compile( ShaderStageType::Fragment, kImGuiFragmentShader );

    assert( vs.success && "UIPass: vertex shader compilation failed" );
    assert( fs.success && "UIPass: fragment shader compilation failed" );

    _shader = Shader::New();
    _shader->attachVertexShaderStage( vs.stage );
    _shader->attachFragmentShaderStage( fs.stage );

    auto linkResult = ShaderCompiler::linkShader( _shader, "ImGuiPass" );
    assert( linkResult.success && "ImGuiPass: shader link failed" );

    _shader->setObjectLabel( "ImGuiPass.Shader" );
    //_resourceManager->registerShader( _shader->name(), _shader );
}

// ------------------------------------------------------------------------------------------------
void UIPass::createFontTexture()
{
    ImGuiIO& io = ImGui::GetIO();

    unsigned char* pixels = nullptr;
    int width  = 0;
    int height = 0;
    io.Fonts->GetTexDataAsRGBA32( &pixels, &width, &height );

    assert( pixels && width > 0 && height > 0 && "UIPass: font atlas build failed" );

    TextureDescription desc( width, height, TextureFormat::RedGreenBlueAlpha8, false, "UIPass.FontAtlas" );
    _fontTexture = Texture2D::New( desc, pixels );

    _fontTexture->setMinFilter( Texture2D::MinFilter::Linear );
    _fontTexture->setMagFilter( Texture2D::MagFilter::Linear );
    _fontTexture->setWrapS( Texture2D::WrapMode::ClampToEdge );
    _fontTexture->setWrapT( Texture2D::WrapMode::ClampToEdge );
	_fontTexture->makeResident(); // the font atlas can be resident as long as the UIPass exists (never changes, small size)

    // font atlas is passed to imgui as s2Engine TexturePtr,
	// it will be retrieved in the render pass to set the shader texture uniform (DSA) and bindless handle.
    io.Fonts->SetTexID( _fontTexture->bindlessHandle() );
}

// ------------------------------------------------------------------------------------------------
// Ensures the shared VBO has enough capacity. If the buffer needs to grow,
// a new GPUBufferObject is created and all three interleaved attributes are
// re-attached to the VAO via setAttribute (DSA). When the existing capacity
// is sufficient the same GPU buffer is reused - only the data is updated.
// ------------------------------------------------------------------------------------------------
static GPUBufferObjectPtr ensureVertexBuffer( const VertexArrayPtr& vao,
                                              GPUBufferObjectPtr    currentVBO,
                                              int64_t               requiredBytes )
{
    if( currentVBO && currentVBO->size() >= requiredBytes )
    {
        // Existing buffer is large enough - invalidate and reuse
        currentVBO->invalidate();
        return currentVBO;
    }

    // Grow with 25 % headroom to avoid reallocating every frame
    const int64_t allocBytes = requiredBytes + requiredBytes / 4;

    auto vbo = GPUBufferObject::New( allocBytes,
                                     GPUBufferObject::Type::ArrayBuffer,
                                     usageHint );
    vbo->setObjectLabel( "ImGuiPass::VBO" );

    constexpr int64_t stride = sizeof( ImDrawVert );
    // Re-attach all three interleaved attributes to the new buffer (DSA).
    // All share the same VBO with bufferOffset = 0 and stride = sizeof(ImDrawVert).
    // Each attribute specifies its own relativeOffset = offsetof(ImDrawVert, field).
    //
    // location 0: aPos  -> 2 floats at relativeOffset 0
    AttributeBuffer posAttr( vbo,
                             AttributeBuffer::ComponentDatatype::Float, 2,
                             false,
                             /*bufferOffset*/   0,
                             /*relativeOffset*/ static_cast<int64_t>( offsetof( ImDrawVert, pos ) ),
                             stride );

    // location 1: aUV   -> 2 floats at relativeOffset 8
    AttributeBuffer uvAttr( vbo,
                            AttributeBuffer::ComponentDatatype::Float, 2,
                            false,
                            /*bufferOffset*/   0,
                            /*relativeOffset*/ static_cast<int64_t>( offsetof( ImDrawVert, uv ) ),
                            stride );

    // location 2: aColor -> 4 unsigned bytes, normalized to [0,1], at relativeOffset 16
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
        ib.set( allocBytes, idxType, usageHint );
        vao->setIndexBuffer( ib );
        return;
    }

    const int64_t allocBytes = requiredBytes + requiredBytes / 4;
    IndexBuffer ib( allocBytes, idxType, usageHint );
    vao->setIndexBuffer( ib );
}

// ------------------------------------------------------------------------------------------------
void UIPass::execute( const RenderBackend& rendererBackend,
                   const ResourceManager& resourceManager,
                   const CommandBuffer& queue,
					  FrameData& frameData )
{
    ImDrawData* drawData = ImGui::GetDrawData();
    if( !drawData || drawData->TotalVtxCount == 0 )
        return;

    assert( frameData.renderTarget && "ImGuiPass: renderTarget must be set" );

    // ------------------------------------------------------------------
    // 1. Compute framebuffer dimensions from ImGui draw data
    // ------------------------------------------------------------------
    const ImVec2 clipOff   = drawData->DisplayPos;
    const ImVec2 clipScale = drawData->FramebufferScale;

    const int fbWidth  = static_cast<int>( drawData->DisplaySize.x * clipScale.x );
    const int fbHeight = static_cast<int>( drawData->DisplaySize.y * clipScale.y );

    if( fbWidth <= 0 || fbHeight <= 0 )
        return;

    // ------------------------------------------------------------------
    // 2. Build orthographic projection from ImGui's display coordinates
    // ------------------------------------------------------------------
    const float L = drawData->DisplayPos.x;
    const float R = drawData->DisplayPos.x + drawData->DisplaySize.x;
    const float T = drawData->DisplayPos.y;
    const float B = drawData->DisplayPos.y + drawData->DisplaySize.y;

    const Math::fmat4 ortho = Math::ortho( L, R, B, T, -1.0f, 1.0f );

    _shader->setUniform( "u_ProjectionMatrix" ,ortho );
   
    // default to font atlas; may be overridden per ImDrawCmd below
    _shader->setUniform( "u_CurrTextureHandle" , _fontTexture->bindlessHandle() );

    // ------------------------------------------------------------------
    // 3. Ensure GPU buffers have enough capacity, then upload data
    // ------------------------------------------------------------------
    const int64_t totalVtxBytes = static_cast<int64_t>( drawData->TotalVtxCount ) * sizeof( ImDrawVert );
    const int64_t totalIdxBytes = static_cast<int64_t>( drawData->TotalIdxCount ) * sizeof( ImDrawIdx );

    _vbo = ensureVertexBuffer( _vao, _vbo, totalVtxBytes );
    ensureIndexBuffer( _vao, totalIdxBytes );

    _vbo->writeRange<ImDrawVert>( 0, totalVtxBytes, [&]( ImDrawVert* dst ) 
    {
        for( int n = 0; n < drawData->CmdListsCount; ++n )
        {
            const ImDrawList* cmdList = drawData->CmdLists[n];
            std::memcpy( dst, cmdList->VtxBuffer.Data, cmdList->VtxBuffer.Size * sizeof( ImDrawVert ) );
            dst += cmdList->VtxBuffer.Size;
        }
    });

    _vao->indexBuffer().writeAll<ImDrawIdx>( [&]( ImDrawIdx* dst ) 
    {
        for( int n = 0; n < drawData->CmdListsCount; ++n )
        {
            const ImDrawList* cmdList = drawData->CmdLists[n];
            std::memcpy( dst, cmdList->IdxBuffer.Data, cmdList->IdxBuffer.Size * sizeof( ImDrawIdx ) );
            dst += cmdList->IdxBuffer.Size;
        }
    });

    // ------------------------------------------------------------------
    // 4. Setup draw state for ImGui rendering
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

    ds.viewport.rect = Math::irect( 0, 0, fbWidth, fbHeight );

    // ------------------------------------------------------------------
    // 5. Iterate draw commands and issue drawRange calls
    // ------------------------------------------------------------------
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

            ImVec2 clipMin( ( pcmd.ClipRect.x - clipOff.x ) * clipScale.x, ( pcmd.ClipRect.y - clipOff.y ) * clipScale.y );
            ImVec2 clipMax( ( pcmd.ClipRect.z - clipOff.x ) * clipScale.x, ( pcmd.ClipRect.w - clipOff.y ) * clipScale.y );
            if( clipMax.x <= clipMin.x || clipMax.y <= clipMin.y )
                continue;

            // glScissor origin is bottom-left: (x, fbHeight - y2, width, height)
            const int sx = static_cast<int>( clipMin.x );
            const int sy = static_cast<int>( fbHeight - clipMax.y );
            const int sw = static_cast<int>( clipMax.x - clipMin.x );
            const int sh = static_cast<int>( clipMax.y - clipMin.y );

            if( sw <= 0 || sh <= 0 )
                continue;

            ds.viewport.scissorTest.enabled = true;
            ds.viewport.scissorTest.rect    = Math::irect( sx, sy, sw, sh );

            const uint32_t idxByteOffset = ( pcmd.IdxOffset + globalIdxOffset ) * sizeof( ImDrawIdx );

            // bind the texture for this draw command (pcmd.TextureId)
            // ImGui stores textures in an opaque ImTextureID type.
            // Our integration uses it to store OpenGL bindles texture handles.
			// Be sure to make the shader's sampler uniform resident and set it to the correct handle value before drawing.
			auto bindlessHandle = static_cast<uint64_t>( pcmd.GetTexID() );
            if( bindlessHandle != 0 ) // resource ID
                _shader->setUniform( "u_CurrTextureHandle" , bindlessHandle );

            rendererBackend.drawRange(
                *frameData.renderTarget,
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

// -------------------------------------------------------------------------------
const std::string& UIPass::name() const
{
    return _name;
}