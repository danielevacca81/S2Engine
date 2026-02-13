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

	// Initialize render pipeline
	_pipeline = pipeline;
	_pipeline.initialize();
}

// ------------------------------------------------------------------------------------------------
void Renderer::beginFrame( const FrameData& frameData )
{
	if( _state == State::FrameStarted )
	{
		throw std::runtime_error(
			"Renderer::beginFrame() called twice without endFrame(). "
			"Call endFrame() before starting a new frame."
		);
	}

	// check frameData validity (e.g., mainTarget not null)
	if( !frameData.mainTarget )
		throw std::runtime_error( "Renderer::beginFrame failed: main render target is null" );


	_frameData = frameData;

	_state = State::FrameStarted;
}

// ------------------------------------------------------------------------------------------------
void Renderer::submit( const ClearCommand& command )
{
	if( _state != State::FrameStarted )
	{
		throw std::runtime_error(
			"Renderer::submit() called outside beginFrame/endFrame. "
			"Call beginFrame() first."
		);
	}

	// Store command for batched execution in endFrame()
	 _commandBuffer.addClear( command );
}

// ------------------------------------------------------------------------------------------------
void Renderer::submit( const RenderCommand& command )
{
	// TODO: Validate command (e.g., material and mesh data not null)
	if( _state != State::FrameStarted )
	{
		throw std::runtime_error(
			"Renderer::submit() called outside beginFrame/endFrame. "
			"Call beginFrame() first."
		);
	}


	// Store command for batched execution in endFrame()
	_commandBuffer.addRender( command );
}

// ------------------------------------------------------------------------------------------------
void Renderer::endFrame()
{
	if( _state != State::FrameStarted )
	{
		throw std::runtime_error(
			"Renderer::endFrame() called without matching beginFrame(). "
			"Call beginFrame() before endFrame()."
		);
	}
	_commandBuffer.sort(); // Sort commands for optimal rendering (e.g., by material, depth, etc.)
	
	_pipeline.execute( _commandBuffer, _frameData /*, *_gpuContext */); // Execute render passes in the pipeline with the current frame data and command buffer


	_commandBuffer.clear(); // Clear command buffer for next frame
	_stats = Stats {}; // Reset statistics for next frame
	
	_state = State::Ready;
}

}
}