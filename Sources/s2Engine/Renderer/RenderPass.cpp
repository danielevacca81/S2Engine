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

#include <iostream>

using namespace s2::Renderer;

// ------------------------------------------------------------------------------------------------
void ForwardPass::initialize( ResourceManager& resourceManager )
{
    // Initialize any resources needed for forward rendering
    // (e.g., default materials, fullscreen quad, etc.)
    _resourceManager = &resourceManager;
	assert( _resourceManager && "ForwardPass initialization failed: ResourceManager is null" );
}

// ------------------------------------------------------------------------------------------------
void ForwardPass::execute( const CommandBuffer& queue, FrameData& frameData )
{
    if( !frameData.mainTarget )
        return; // No render target set

    // Get RenderCore context (assume it's stored somewhere accessible)
    // For now, use the current context
    auto* gpuContext = RenderCore::Context::current();
    if( !gpuContext )
        return;

    auto& renderCommands = gpuContext->commands();

    // 1. Execute all clear commands
    for( const auto& clearCmd : queue.clearCommands() )
        renderCommands.clear( *frameData.mainTarget, getClearState( clearCmd ) );

    // 2. Execute render commands
    for( const auto& renderCmd : queue.renderCommands() )
    {
        // Translate high-level RenderCommand to low-level DrawState
        RenderCore::DrawState ds;
        ds.transform.modelMatrix        = renderCmd.modelMatrix;
        ds.transform.viewMatrix         = frameData.cameraViewMatrix;
        ds.transform.projectionMatrix   = frameData.cameraProjectionMatrix;
        ds.viewport.rect                = frameData.mainTarget->size();
        ds.viewport.scissorTest.enabled = false; // @todo: add scissor rect to RenderCommand if needed

		// set material properties and shader uniforms
        ds.renderState = getRenderState( renderCmd );

		// Retrieve shader from resource manager or use default when not specified or not valid
        ds.shader      = [&]
        {
            if( renderCmd.material.shader == InvalidHandle )
                return RenderCore::DefaultShaders.Simple;

            auto s = _resourceManager->getShader( renderCmd.material.shader );
            return s == nullptr
                ? RenderCore::DefaultShaders.Simple
                : s;
        }();

		// Apply material properties to shader uniforms
		// @note: performance optimization - we could cache the mapping of material properties
        // to shader uniforms for each shader to avoid redundant lookups and conversions every frame
		renderCmd.material.applyPropertiesToShader( *ds.shader );

		// Retrieve mesh from resource manager
		auto mesh = _resourceManager->getMesh( renderCmd.mesh );

		// bind textures to drawstate texture units
        for( const auto& [unit, textureHandle] : renderCmd.material.textures )
        {
            auto texture = _resourceManager->getTexture( textureHandle );
            if( texture )
				ds.textureUnits[unit].set( texture, RenderCore::DefaultSamplers.LinearClamp );
        }
        

          
        // Determine primitive type
        RenderCore::PrimitiveType primitiveType = [renderCmd]
         {
            switch( renderCmd.renderMode )
            {
            case s2::Renderer::RenderMode::Points:    return RenderCore::PrimitiveType::Points;
            case s2::Renderer::RenderMode::Lines:     return RenderCore::PrimitiveType::Lines;
            case s2::Renderer::RenderMode::Triangles: return RenderCore::PrimitiveType::Triangles;
            default:                                  return RenderCore::PrimitiveType::Triangles; // Fallback
            }
        }();

        
        // Execute draw call
        renderCommands.draw( *frameData.mainTarget, primitiveType, mesh, ds );

        // Update statistics
        _stats.drawCalls++;
        _stats.vertices +=  mesh->vertexCount();
        _stats.triangles += mesh->indexCount() / 3;
    }
#if 0
	std::cout << "ForwardPass executed: "
        << _stats.drawCalls << " draw calls, "
        << _stats.triangles << " triangles, "
		<< _stats.vertices << " vertices." << std::endl;
#endif

	_stats = Stats {}; // Reset stats for the next frame
}

// ------------------------------------------------------------------------------------------------
const std::string& ForwardPass::name() const
{
    return _name;
}