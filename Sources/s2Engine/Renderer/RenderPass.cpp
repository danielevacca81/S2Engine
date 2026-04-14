// RenderPass.cpp
//
#include "RenderPass.h"

#include "CommandBuffer.h"
#include "FrameData.h"
#include "RenderCommand.h"

#include "RenderCore/RendererBackend.h"
#include "RenderCore/Context.h"
#include "RenderCore/ClearState.h"
#include "RenderCore/DrawState.h"
#include "RenderCore/PrimitiveType.h"
#include "RenderCore/VertexData.h"

#include <cassert>

using namespace s2::Renderer;

#pragma region HelperFunctions
// ------------------------------------------------------------------------------------------------
static inline RenderCore::DrawState createDrawState( const RenderCommand& renderCmd, const FrameData& frameData )
{
    RenderCore::DrawState drawState;

    // Transform matrices
    drawState.transform.modelMatrix      = renderCmd.modelMatrix;
    drawState.transform.viewMatrix       = frameData.cameraViewMatrix;
    drawState.transform.projectionMatrix = frameData.cameraProjectionMatrix;

    // Viewport
    drawState.viewport.rect = frameData.mainTarget->size();

    // Render state from material
    drawState.renderState = getRenderState( renderCmd );

    return drawState;
}

// ------------------------------------------------------------------------------------------------
// Get shader with fallback to default
static inline RenderCore::ShaderPtr getShader( ResourceManager* resourceManager, const RenderCommand& renderCmd )
{
    assert( resourceManager && "ResourceManager must be valid" );

    // Use material shader or fallback to default
    if( renderCmd.material.shader == InvalidHandle )
        return RenderCore::DefaultShaders.Simple;

    auto shader = resourceManager->shader( renderCmd.material.shader );
    return shader ? shader : RenderCore::DefaultShaders.Simple;
}


// ------------------------------------------------------------------------------------------------
// Setup standard transform uniforms (DSA - no binding required)
static inline void setupShaderUniforms( const RenderCore::ShaderPtr& shader,const RenderCommand& renderCmd,const FrameData& frameData )
{
    assert( shader && "Shader must be valid" );

    // ===== Standard Transform Uniforms (DSA) =====
    shader->setUniform( "u_ModelMatrix", renderCmd.modelMatrix );
    shader->setUniform( "u_ViewMatrix", frameData.cameraViewMatrix );
    shader->setUniform( "u_ProjectionMatrix", frameData.cameraProjectionMatrix );
    
    // Derived matrices
    const auto modelView = frameData.cameraViewMatrix * renderCmd.modelMatrix;
    const auto modelViewProjection = frameData.cameraProjectionMatrix * modelView;
    const auto normalMatrix = Math::transpose( Math::inverse( Math::fmat3( modelView ) ) );

    shader->setUniform( "u_ModelViewMatrix", modelView );
    shader->setUniform( "u_ModelViewProjectionMatrix", modelViewProjection );
    shader->setUniform( "u_NormalMatrix", normalMatrix );

    // NOTA: Material properties e textures sono applicate separatamente
    // tramite renderCmd.material.applyPropertiesToShader() e 
    // renderCmd.material.applyTexturesToShader() nel metodo execute()
}

// ------------------------------------------------------------------------------------------------
// Get primitive type from render mode
static inline RenderCore::PrimitiveType getPrimitiveType( RenderMode mode )
{
    switch( mode )
    {
    case RenderMode::Points:    return RenderCore::PrimitiveType::Points;
    case RenderMode::Lines:     return RenderCore::PrimitiveType::Lines;
    case RenderMode::Triangles: return RenderCore::PrimitiveType::Triangles;
    default:                    return RenderCore::PrimitiveType::Triangles;
    }
}


#pragma endregion

// ------------------------------------------------------------------------------------------------
void ForwardPass::initialize( ResourceManager& resourceManager )
{
    _resourceManager = &resourceManager;
    assert( _resourceManager && "ForwardPass initialization failed: ResourceManager is null" );
}

// ------------------------------------------------------------------------------------------------
void ForwardPass::execute( const CommandBuffer& queue, FrameData& frameData, const RenderCore::Context* ctx )
{
    if( !frameData.mainTarget )
        return; // No render target set
        
    auto& rendererBackend = ctx->rendererBackend();

    // ===== 1. Execute Clear Commands =====
    for( const auto& clearCmd : queue.clearCommands() )
        rendererBackend.clear( *frameData.mainTarget, getClearState( clearCmd ) );

    // ===== 2. Execute Render Commands =====
    for( const auto& renderCmd : queue.renderCommands() )
    {
        // Setup draw state
        auto drawState = createDrawState( renderCmd, frameData );
        
        // Get shader (with fallback to default)
        auto shader = getShader( _resourceManager, renderCmd );
        drawState.shader = shader;

        // ===== DSA: Set uniforms BEFORE drawing =====
        setupShaderUniforms( shader, renderCmd, frameData );
        
        // Apply material properties (DSA - no binding)
        renderCmd.material.applyPropertiesToShader( *shader );
        
        // Apply textures (Bindless - no TextureUnit!)
        renderCmd.material.applyTexturesToShader( *shader, *_resourceManager );

        // Get mesh
        auto mesh = _resourceManager->mesh( renderCmd.mesh );
        if( !mesh )
            continue; // Skip if mesh not found

        // Determine primitive type
        RenderCore::PrimitiveType primitiveType = getPrimitiveType( renderCmd.renderMode );

        // Execute draw call (DSA-aware)
        rendererBackend.draw( *frameData.mainTarget, primitiveType, mesh, drawState );

        // Update statistics
        _stats.drawCalls++;
        _stats.vertices += mesh->vertexCount();
        _stats.triangles += mesh->indexCount() / 3;
    }

#ifdef _DEBUG
    //printStats();
#endif

    _stats = {}; // Reset stats for next frame
}

// ------------------------------------------------------------------------------------------------
const std::string& ForwardPass::name() const
{
    return _name;
}