// RenderPass.cpp
//
#include "RenderPass.h"
#include "CommandBuffer.h"
#include "FrameData.h"
#include "RenderCommand.h"
#include "GPUStateMapper.h"

#include "Core/VectorCast.h"

#include "RenderCore/RenderCommands.h"
#include "RenderCore/Context.h"
#include "RenderCore/ClearState.h"
#include "RenderCore/DrawState.h"
#include "RenderCore/PrimitiveType.h"
#include "RenderCore/VertexData.h"

namespace s2 {
namespace Renderer {

// ------------------------------------------------------------------------------------------------
// ForwardPass Implementation
// ------------------------------------------------------------------------------------------------
void ForwardPass::initialize()
{
    // Initialize any resources needed for forward rendering
    // (e.g., default materials, fullscreen quad, etc.)
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
		ds.shader      = renderCmd.material.shader 
            ? renderCmd.material.shader 
			: RenderCore::DefaultShaders.Simple; // Fallback shader if material doesn't specify one

		for( auto& [name, value] : renderCmd.material.properties )
            ds.shader->setUniformValue( name, value );
          
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

        // @todo: retrieve vertex data from resourcepool by resourceID in RenderCommand ?
        
        
        // Execute draw call
        renderCommands.draw( *frameData.mainTarget, primitiveType, renderCmd.model.vertexData, ds );
        
        // Update statistics
        // context.stats.drawCalls++;
        // context.stats.vertices += renderCmd.meshData.vertices.size();
        // context.stats.triangles += renderCmd.meshData.indices.size() / 3;
    }
}

// ------------------------------------------------------------------------------------------------
const std::string& ForwardPass::name() const
{
    return _name;
}

} // namespace Renderer
} // namespace s2