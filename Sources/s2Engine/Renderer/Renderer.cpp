// Renderer.cpp
//
#include "Renderer.h"
#include "RenderCommand.h"
#include "RenderCore/RenderCommands.h"
#include "RenderCore/ClearState.h"
#include "RenderCore/DrawState.h"
#include "RenderCore/PrimitiveType.h"
#include "RenderCore/VertexData.h"

#include <vector>

namespace s2 {
namespace Renderer {

// ------------------------------------------------------------------------------------------------
Renderer::Renderer( const RenderCore::Context* ctx, const RenderPipeline& pipeline )
    : _gpuContext( ctx )
{
    if( !ctx )
        throw std::runtime_error( "Renderer initialization failed: GPU context is null" );

    _pipeline = pipeline;
    _pipeline.initialize( _resourceManager );
}

// ------------------------------------------------------------------------------------------------
void Renderer::beginFrame( const FrameData& frameData )
{
    if( _state == State::FrameStarted )
        throw std::runtime_error(
            "Renderer::beginFrame() called twice without endFrame(). "
            "Call endFrame() before starting a new frame." );

    if( !frameData.mainTarget )
        throw std::runtime_error( "Renderer::beginFrame failed: main render target is null" );

    _frameData = frameData;
    _state     = State::FrameStarted;
}

// ------------------------------------------------------------------------------------------------
void Renderer::clear( const ClearCommand& command )
{
    if( _state != State::FrameStarted )
        throw std::runtime_error(
            "Renderer::clear() called outside beginFrame/endFrame. "
            "Call beginFrame() first." );

    _commandBuffer.addClear( command );
}

// ------------------------------------------------------------------------------------------------
void Renderer::render( const RenderCommand& command )
{
    if( _state != State::FrameStarted )
        throw std::runtime_error(
            "Renderer::render() called outside beginFrame/endFrame. "
            "Call beginFrame() first." );

    _commandBuffer.addRender( command );
}

// ------------------------------------------------------------------------------------------------
void Renderer::endFrame()
{
    if( _state != State::FrameStarted )
        throw std::runtime_error(
            "Renderer::endFrame() called without matching beginFrame(). "
            "Call beginFrame() before endFrame()." );

    _commandBuffer.sort();
    _pipeline.execute( _commandBuffer, _frameData, _gpuContext );
    _commandBuffer.clear();

    // Notify all listeners while the GL context is still current.
    // This is the correct point for any GL readback (e.g., Picker::onFrameDone).
    onFrameDone( _frameData/*,_stats*/ );

    _stats = Stats{};
    _state = State::Ready;
}

} // namespace Renderer
} // namespace s2