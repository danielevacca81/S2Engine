// RenderPass.cpp
//
#include "RenderPass.h"

#include "CommandBuffer.h"
#include "FrameData.h"
#include "RenderCommand.h"

#include "RenderCore/RenderCommands.h"
#include "RenderCore/Context.h"
#include "RenderCore/ClearState.h"
#include "RenderCore/DrawState.h"
#include "RenderCore/PrimitiveType.h"
#include "RenderCore/VertexData.h"

#include <cassert>
#include <iostream>

using namespace s2::Renderer;

// ------------------------------------------------------------------------------------------------
void ForwardPass::initialize( ResourceManager& resourceManager )
{
    _resourceManager = &resourceManager;
    assert( _resourceManager && "ForwardPass initialization failed: ResourceManager is null" );
}

// ------------------------------------------------------------------------------------------------
void ForwardPass::execute( const CommandBuffer& queue, FrameData& frameData )
{
    if( !frameData.mainTarget )
        return; // No render target set

    // Get RenderCore context
    auto* gpuContext = RenderCore::Context::current();
    if( !gpuContext )
        return;

    auto& renderCommands = gpuContext->commands();

    // ===== 1. Execute Clear Commands =====
    for( const auto& clearCmd : queue.clearCommands() )
        renderCommands.clear( *frameData.mainTarget, getClearState( clearCmd ) );

    // ===== 2. Execute Render Commands =====
    for( const auto& renderCmd : queue.renderCommands() )
    {
        // Setup draw state
        RenderCore::DrawState drawState = createDrawState( renderCmd, frameData );
        
        // Get shader (with fallback to default)
        auto shader = getShader( renderCmd );
        drawState.shader = shader;

        // ===== DSA: Set uniforms BEFORE drawing =====
        setupShaderUniforms( shader, renderCmd, frameData );
        
        // Apply material properties (DSA - no binding)
        renderCmd.material.applyPropertiesToShader( *shader );
        
        // Apply textures (Bindless - no TextureUnit!)
        renderCmd.material.applyTexturesToShader( *shader, *_resourceManager );

        // Get mesh
        auto mesh = _resourceManager->getMesh( renderCmd.mesh );
        if( !mesh )
            continue; // Skip if mesh not found

        // Determine primitive type
        RenderCore::PrimitiveType primitiveType = getPrimitiveType( renderCmd.renderMode );

        // Execute draw call (DSA-aware)
        renderCommands.draw( *frameData.mainTarget, primitiveType, mesh, drawState );

        // Update statistics
        updateStats( mesh );
    }

#ifdef _DEBUG
    printStats();
#endif

    _stats = {}; // Reset stats for next frame
}

// ------------------------------------------------------------------------------------------------
const std::string& ForwardPass::name() const
{
    return _name;
}

// ================================================================================================
// PRIVATE HELPERS
// ================================================================================================

RenderCore::DrawState ForwardPass::createDrawState( 
    const RenderCommand& renderCmd, 
    const FrameData& frameData ) const
{
    RenderCore::DrawState drawState;

    // Transform matrices
    drawState.transform.modelMatrix = renderCmd.modelMatrix;
    drawState.transform.viewMatrix = frameData.cameraViewMatrix;
    drawState.transform.projectionMatrix = frameData.cameraProjectionMatrix;

    // Viewport
    drawState.viewport.rect = frameData.mainTarget->size();

    // Render state from material
    drawState.renderState = getRenderState( renderCmd );

    return drawState;
}

// ------------------------------------------------------------------------------------------------
RenderCore::ShaderPtr ForwardPass::getShader( const RenderCommand& renderCmd ) const
{
    // Use material shader or fallback to default
    if( renderCmd.material.shader == InvalidHandle )
        return RenderCore::DefaultShaders.Simple;

    auto shader = _resourceManager->getShader( renderCmd.material.shader );
    return shader ? shader : RenderCore::DefaultShaders.Simple;
}

// ------------------------------------------------------------------------------------------------
void ForwardPass::setupShaderUniforms( 
    const RenderCore::ShaderPtr& shader,
    const RenderCommand& renderCmd,
    const FrameData& frameData ) const
{
    assert( shader && "Shader must be valid" );

    // ===== Standard Transform Uniforms (DSA) =====
    shader->setUniform( "modelMatrix", renderCmd.modelMatrix );
    shader->setUniform( "viewMatrix", frameData.cameraViewMatrix );
    shader->setUniform( "projectionMatrix", frameData.cameraProjectionMatrix );
    
    // Derived matrices
    const auto modelView = frameData.cameraViewMatrix * renderCmd.modelMatrix;
    const auto modelViewProjection = frameData.cameraProjectionMatrix * modelView;
    const auto normalMatrix = Math::transpose( Math::inverse( Math::fmat3( modelView ) ) );

    shader->setUniform( "modelViewMatrix", modelView );
    shader->setUniform( "modelViewProjectionMatrix", modelViewProjection );
    shader->setUniform( "normalMatrix", normalMatrix );

    // NOTA: Material properties e textures sono applicate separatamente
    // tramite renderCmd.material.applyPropertiesToShader() e 
    // renderCmd.material.applyTexturesToShader() nel metodo execute()
}

// ------------------------------------------------------------------------------------------------
RenderCore::PrimitiveType ForwardPass::getPrimitiveType( RenderMode mode ) const
{
    switch( mode )
    {
    case RenderMode::Points:    return RenderCore::PrimitiveType::Points;
    case RenderMode::Lines:     return RenderCore::PrimitiveType::Lines;
    case RenderMode::Triangles: return RenderCore::PrimitiveType::Triangles;
    default:                    return RenderCore::PrimitiveType::Triangles;
    }
}

// ------------------------------------------------------------------------------------------------
void ForwardPass::updateStats( const RenderCore::VertexDataPtr& mesh )
{
    _stats.drawCalls++;
    _stats.vertices += mesh->vertexCount();
    _stats.triangles += mesh->indexCount() / 3;
}

// ------------------------------------------------------------------------------------------------
void ForwardPass::printStats() const
{
#ifdef _DEBUG
    std::cout << "ForwardPass executed: "
              << _stats.drawCalls << " draw calls, "
              << _stats.triangles << " triangles, "
              << _stats.vertices << " vertices." << std::endl;
#endif
}