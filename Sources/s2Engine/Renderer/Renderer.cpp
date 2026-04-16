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

    _pipeline = RenderPipeline::createDefaultPipeline();
    _pipeline.initialize( &_resourceManager );
}

// ------------------------------------------------------------------------------------------------
void Renderer::setPipeline( RenderPipeline pipeline )
{
	if( _state == State::FrameStarted )
		throw std::runtime_error( "Renderer::setPipeline() cannot be called while a frame is in progress." );

    _pipeline = std::move( pipeline );
	_pipeline.initialize( &_resourceManager );
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
    _pipeline.execute( _commandBuffer, _frameData, _gpuContext->rendererBackend() );
    _commandBuffer.clear();

    // Notify all listeners while the GL context is still current.
    // This is the correct point for any GL readback (e.g., Picker::onRenderCompleted).
    onRenderCompleted( _frameData/*,_stats*/ );

    _stats = Stats{};
    _state = State::Ready;
}

} // namespace Renderer
} // namespace s2