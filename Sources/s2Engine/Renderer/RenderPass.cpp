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

#pragma region Helper Functions
// // ------------------------------------------------------------------------------------------------
// static inline RenderCore::VertexDataPtr createVertexData( const MeshData3D& meshData )
// {
//     auto vertexData = RenderCore::VertexData::New();

//     // Set vertex attributes (positions, normals, UVs, etc.)
//      vertexData->setVertices     ( vector_cast<Math::dvec3,Math::vec3>( meshData.vertices ) );
//      vertexData->setNormals      ( vector_cast<Math::dvec3,Math::vec3>( meshData.normals  ) );
//      vertexData->setTextureCoords( vector_cast<Math::dvec2,Math::vec2>( meshData.uvCoords ) );
//      vertexData->setIndices      ( meshData.indices );
//      vertexData->setColors       ( std::vector<Color>( meshData.vertices.size(), Color::red() ) );

//     return vertexData;
// }
#pragma endregion

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

    // 1. Execute clear commands
    for( const auto& clearCmd : queue.clearCommands() )
        renderCommands.clear( *frameData.mainTarget, GPUStateMapper::map( clearCmd ) );

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

        ds.renderState = GPUStateMapper::map( renderCmd );
        
        // For simplicity, use a default shader. In a real implementation, this would be determined by the material and render command.
        ds.shader                       = RenderCore::DefaultShaders.BlinnPhong;        
        ds.shader->setUniformValue<Math::vec4>( "u_LightPosition"          , { 0.f, 0.f, 1.f, 1.f } );
        ds.shader->setUniformValue<Math::vec4>( "u_LightAmbient"           , { .01f,.01f,.01f,1.f } );
        ds.shader->setUniformValue<Math::vec4>( "u_LightDiffuse"           , { 1.f,1.f,1.f,1.f } );
        ds.shader->setUniformValue<Math::vec4>( "u_LightSpecular"          , { 1.f,1.f,1.f,1.f } );
        ds.shader->setUniformValue<float>(      "u_LightShininess"         , 160.f );
   
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
        
        
        // @todo: retrieve vertex data from resourcepool by resourceID in RenderCommand

        // // Create vertex data from mesh data
        // auto vertexData = createVertexData( renderCmd.meshData );

        // // Execute draw call
        // renderCommands.draw( *frameData.mainTarget, primitiveType, vertexData, ds );

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