// Renderer.cpp
//
#include "Renderer.h"

#include "RenderCommand.h"
#include "RenderCore/Context.h"

#include <vector>

namespace s2 {
namespace Renderer {

// ------------------------------------------------------------------------------------------------
Renderer::Renderer( const RenderCore::Context* ctx )
    : _gpuContext( ctx )
{
    if( !ctx )
        throw std::runtime_error( "Renderer initialization failed: GPU context is null" );
}

// ------------------------------------------------------------------------------------------------
void Renderer::begin( const FrameData& frameData )
{
    if( _state == State::FrameStarted )
        throw std::runtime_error(
            "Renderer::begin() called twice without endFrame(). "
            "Call endFrame() before starting a new frame." );

    if( !frameData.renderTarget )
        throw std::runtime_error( "Renderer::begin() failed: main render target is null" );

    if( frameData.renderPasses.isEmpty() )
		throw std::runtime_error( "Renderer::begin() failed: no rendering passes submitted for this frame." );

    _frameData = frameData;
    _state     = State::FrameStarted;
}

// ------------------------------------------------------------------------------------------------
void Renderer::submit( const ClearCommand& command )
{
    if( _state != State::FrameStarted )
        throw std::runtime_error(
            "Renderer::submit() called outside beginFrame/endFrame. "
            "Call beginFrame() first." );

    _commandBuffer.addClear( command );
}

// ------------------------------------------------------------------------------------------------
void Renderer::submit( const RenderCommand& command )
{
    if( _state != State::FrameStarted )
        throw std::runtime_error(
            "Renderer::render() called outside beginFrame/endFrame. "
            "Call beginFrame() first." );

    _commandBuffer.addRender( command );
}

// ------------------------------------------------------------------------------------------------
void Renderer::execute()
{
    if( _state != State::FrameStarted )
        throw std::runtime_error(
            "Renderer::endFrame() called without matching beginFrame(). "
            "Call beginFrame() before endFrame()." );

    _commandBuffer.sort();
    _frameData.renderPasses.execute( _gpuContext->rendererBackend(), _resourceManager, _commandBuffer, _frameData );

    _commandBuffer.clear();

    // Notify all listeners while the GL context is still current.
    // This is the correct point for any GL readback (e.g., Picker::onRenderCompleted).
    onRenderCompleted( _frameData/*,_stats*/ );

    _stats = Stats{};
    _state = State::Ready;
}

} // namespace Renderer
} // namespace s2