// PickPass.cpp
//
#include "PickPass.h"


#include "FrameData.h"
#include "CommandBuffer.h"
#include "RenderCommand.h"
#include "ResourceManager.h"

#include "RenderCore/OpenGL.h"
#include "RenderCore/Context.h"
#include "RenderCore/RendererBackend.h"
#include "RenderCore/RenderTarget.h"
#include "RenderCore/DrawState.h"
#include "RenderCore/ClearState.h"
#include "RenderCore/TextureDescription.h"
#include "RenderCore/ShaderCompiler.h"
#include "RenderCore/PrimitiveType.h"

#include "Core/Strings.h"

#include <stdexcept>
#include <cassert>

using namespace s2::Renderer;
using namespace s2::RenderCore;

// ------------------------------------------------------------------------------------------------
void PickPass::initialize( ResourceManager* resourceManager )
{
	RenderPass::initialize( resourceManager );

    // Internal pick shader
    _pickShader = Shader::New();

    // Vertex: transforms position only (no normals, UVs, etc.)
    const auto vtx  = ShaderCompiler::compile( ShaderStageType::Vertex, STRINGIFY( #version 460 core\n
    layout( location = 0 ) in vec3 a_Position;
    uniform mat4 u_MVP;
    void main()
    {
        gl_Position = u_MVP * vec4( a_Position, 1.0 );
    }
    ) );
    
    // Fragment: outputs (objectID, gl_PrimitiveID) to RG32I attachment
    const auto frag = ShaderCompiler::compile( ShaderStageType::Fragment, STRINGIFY( #version 460 core\n
    layout( location = 0 ) out ivec2 fragPickID;
    uniform int u_ObjectID;
    void main()
    {
        fragPickID = ivec2( u_ObjectID, gl_PrimitiveID );
    }
    ) );

    if( !vtx || !frag )
        throw std::runtime_error( "PickPass: failed to compile internal pick shader" );

    _pickShader->attachVertexShaderStage( vtx.stage );
    _pickShader->attachFragmentShaderStage( frag.stage );

    if( !ShaderCompiler::linkShader( _pickShader, "Internal.PickPass" ) )
        throw std::runtime_error( "PickPass: failed to link internal pick shader" );

	_resourceManager->registerShader( _name, _pickShader );
}

// ------------------------------------------------------------------------------------------------
void PickPass::execute( const CommandBuffer& queue, FrameData& frameData, const RenderCore::RendererBackend& renderBackend )
{
    assert( _resourceManager && "PickPass not initialized" );

    if( !isEnabled() || !frameData.renderTarget )
        return;

    // 1. Ensure pick target matches main target dimensions
    ensurePickTarget( frameData.renderTarget->width(), frameData.renderTarget->height() );

    // 2. Publish pick target so Picker can read from it
    frameData.passData[kPickTargetKey] = _pickTarget.get();

    // 3. Clear pick buffer (0 = invalid objectID)
    ClearState cs;
    cs.buffers = ClearBuffers::ColorAndDepthBuffer;
    cs.color   = Color( 0, 0, 0, 0 );
    renderBackend.clear( *_pickTarget, cs );

    // 4. Render all pickable objects with internal pick shader
    for( const auto& cmd : queue.renderCommands() )
    {
        if( cmd.pickableID == 0 )
            continue; // Non-pickable, skip

        auto mesh = _resourceManager->mesh( cmd.mesh );
        if( !mesh )
            continue;

        const auto mvp = frameData.cameraProjectionMatrix *
                         frameData.cameraViewMatrix        *
                         cmd.modelMatrix;

        _pickShader->setUniform( "u_MVP",      mvp );
        _pickShader->setUniform( "u_ObjectID", static_cast<int>( cmd.pickableID ) );

        DrawState ds( _pickShader );
        ds.renderState.depthTest.enabled  = true;
        ds.renderState.depthTest.function = DepthTest::Function::Less;
        ds.viewport.rect                  = _pickTarget->size();

        renderBackend.draw( *_pickTarget, PrimitiveType::Triangles, mesh, ds );
    }
}

// ------------------------------------------------------------------------------------------------
void PickPass::ensurePickTarget( uint32_t width, uint32_t height )
{
    if( _pickTarget &&
        _pickTarget->width()  == width &&
        _pickTarget->height() == height )
        return; // Already correct size

    // (Re)create pick render target
    _pickTarget = std::make_unique<RenderTarget>();

    _pickTarget->createAttachment( FrameBuffer::AttachmentPoint::ColorAttachment0, TextureFormat::RedGreen32i );
    _pickTarget->createAttachment( FrameBuffer::AttachmentPoint::DepthAttachment,  TextureFormat::Depth24 );
    _pickTarget->resize( static_cast<int32_t>( width ), static_cast<int32_t>( height ) );
}