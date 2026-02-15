// RenderPass.cpp
//
#include "RenderPass.h"
#include "CommandBuffer.h"
#include "FrameData.h"
#include "RenderCommand.h"

#include "Core/VectorCast.h"

#include "RenderCore/RenderCommands.h"
#include "RenderCore/Context.h"
#include "RenderCore/ClearState.h"
#include "RenderCore/DrawState.h"
#include "RenderCore/PrimitiveType.h"
#include "RenderCore/VertexData.h"

#pragma region Helper Functions
// ------------------------------------------------------------------------------------------------
static inline RenderCore::ClearState translateClearCommand( const s2::Renderer::ClearCommand& cmd )
{
    RenderCore::ClearState cs;

    switch( cmd.mode )
    {
    case s2::Renderer::ClearCommand::ClearMode::ColorOnly:
        cs.buffers = RenderCore::ClearBuffers::ColorBuffer;
        cs.color = cmd.color;
        break;

    case s2::Renderer::ClearCommand::ClearMode::DepthOnly:
        cs.buffers = RenderCore::ClearBuffers::DepthBuffer;
        cs.depth = cmd.depth;
        break;

    case s2::Renderer::ClearCommand::ClearMode::ColorAndDepth:
        cs.buffers = RenderCore::ClearBuffers::ColorAndDepthBuffer;
        cs.color = cmd.color;
        cs.depth = cmd.depth;
        break;

    case s2::Renderer::ClearCommand::ClearMode::StencilOnly:
        cs.buffers = RenderCore::ClearBuffers::StencilBuffer;
        cs.stencil = cmd.stencil;
        break;

    case s2::Renderer::ClearCommand::ClearMode::DepthAndStencil:
        cs.buffers = RenderCore::ClearBuffers::StencilAndDepthBuffer;
        cs.depth = cmd.depth;
        cs.stencil = cmd.stencil;
        break;

    case s2::Renderer::ClearCommand::ClearMode::AllBuffers:
        cs.buffers = RenderCore::ClearBuffers::All;
        cs.color = cmd.color;
        cs.depth = cmd.depth;
        cs.stencil = cmd.stencil;
        break;
    }

    return cs;
}

// ------------------------------------------------------------------------------------------------
static inline RenderCore::DrawState translateRenderCommand( const s2::Renderer::RenderCommand& cmd, s2::Renderer::FrameData& frameData )
{
    RenderCore::DrawState ds;

	ds.transform.modelMatrix        = cmd.modelMatrix;
	ds.transform.viewMatrix         = frameData.cameraViewMatrix;
	ds.transform.projectionMatrix   = frameData.cameraProjectionMatrix;
	ds.viewport.rect                = frameData.mainTarget->size();
	ds.viewport.scissorTest.enabled = false; // @todo: add scissor rect to RenderCommand if needed
    

	// todo: set render state (face culling, depth test, blending, etc.) based on material or command properties
	// For now, use default render state. In a real implementation, this would be determined by the material or command properties.
    ds.shader                       = RenderCore::DefaultShaders.BlinnPhong;
    ds.shader->setUniformValue<Math::vec4>( "u_LightPosition"          , { 0.f, 0.f, 1.f, 1.f } );
    ds.shader->setUniformValue<Math::vec4>( "u_LightAmbient"           , { .01f,.01f,.01f,1.f } );
    ds.shader->setUniformValue<Math::vec4>( "u_LightDiffuse"           , { 1.f,1.f,1.f,1.f } );
    ds.shader->setUniformValue<Math::vec4>( "u_LightSpecular"          , { 1.f,1.f,1.f,1.f } );
    ds.shader->setUniformValue<float>(      "u_LightShininess"         , 160.f );
   
    // Merge with context draw state (camera matrices, etc.)
    // ds.uniforms.merge( context.drawState.uniforms );

    return ds;
}

// ------------------------------------------------------------------------------------------------
static inline RenderCore::PrimitiveType translateDrawMode( s2::Renderer::RenderCommand::DrawMode mode )
{
    switch( mode )
    {
    case s2::Renderer::RenderCommand::DrawMode::Points:     return RenderCore::PrimitiveType::Points;
    case s2::Renderer::RenderCommand::DrawMode::Lines:      return RenderCore::PrimitiveType::Lines;
    case s2::Renderer::RenderCommand::DrawMode::Triangles:
    default:                                                return RenderCore::PrimitiveType::Triangles;
    }
}


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

// ================================================================================================
// ForwardPass Implementation
// ================================================================================================

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
    {
        RenderCore::ClearState cs = translateClearCommand( clearCmd );
        renderCommands.clear( *frameData.mainTarget, cs );
    }

    // 2. Execute render commands
    for( const auto& renderCmd : queue.renderCommands() )
    {
        // Translate high-level RenderCommand to low-level DrawState
        RenderCore::DrawState ds = translateRenderCommand( renderCmd, frameData );

        // Determine primitive type
        RenderCore::PrimitiveType primitiveType = translateDrawMode( renderCmd.drawMode );

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