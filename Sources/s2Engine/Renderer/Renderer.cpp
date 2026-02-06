// MeshRenderer.cpp
//
#include "MeshRenderer.h"

namespace s2 {
namespace Renderer {

MeshRenderer::MeshRenderer()
{
}

void MeshRenderer::setDefaultProgram( RenderCore::ProgramPtr program )
{
	_defaultProgram = program;
	_materialBinder.reset();
}

void MeshRenderer::setRenderTarget( RenderCore::RenderTarget* renderTarget )
{
	_renderTarget = renderTarget;
}

void MeshRenderer::submit( const RenderCommand& command )
{
	_commands.push_back( command );
}

void MeshRenderer::flush()
{
	if( !_renderTarget )
		return;

	// Optional: sort commands for optimization
	// sortCommands();

	for( auto& cmd : _commands )
	{
		// Use command's DrawState or create one with default program
		auto& drawState = cmd.drawState;

		// If no shader is set in command, use default
		if( !drawState.shader && _defaultProgram )
		{
			drawState.shader = _defaultProgram;
		}

		if( !drawState.shader )
			continue;

		// 1. Apply transformations to ViewState
		applyTransforms( cmd, drawState );

		// 2. Bind material (updates DrawState's shader uniforms and texture units)
		if( cmd.material )
		{
			_materialBinder.bind( *cmd.material, drawState );
		}

		// 3. Apply custom uniforms if provided
		if( cmd.customUniformSetter )
		{
			cmd.customUniformSetter( drawState );
		}

		// 4. Draw using RenderTarget
		if( cmd.vertexData )
		{
			_renderTarget->draw( cmd.primitiveType, cmd.vertexData, drawState );
			_stats.drawCalls++;
		}
	}

	clear();
}

void MeshRenderer::clear()
{
	_commands.clear();
}

void MeshRenderer::resetStatistics()
{
	_stats = Stats {};
	_materialBinder.resetStatistics();
}

void MeshRenderer::applyTransforms( const RenderCommand& cmd, RenderCore::DrawState& drawState )
{
	// Update ViewState in DrawState
	drawState.viewState.modelMatrix = Math::dmat4( cmd.modelMatrix );
	drawState.viewState.viewMatrix = Math::dmat4( cmd.viewMatrix );
	drawState.viewState.projectionMatrix = Math::dmat4( cmd.projectionMatrix );

	// Set viewport if render target is available
	if( _renderTarget )
	{
		drawState.viewState.viewport = Math::irect( 0, 0, _renderTarget->width(), _renderTarget->height() );
	}

	// Set commonly used matrix uniforms
	if( drawState.shader )
	{
		auto& program = drawState.shader;

		// Compute derived matrices
		auto mvp = drawState.viewState.modelViewProjectionMatrix();
		auto mv = drawState.viewState.modelViewMatrix();
		auto normalMatrix = drawState.viewState.normalMatrix();

		// Set uniforms
		program->setUniformValue( "u_ModelMatrix", cmd.modelMatrix );
		program->setUniformValue( "u_ViewMatrix", cmd.viewMatrix );
		program->setUniformValue( "u_ProjectionMatrix", cmd.projectionMatrix );
		program->setUniformValue( "u_MVP", Math::mat4( mvp ) );
		program->setUniformValue( "u_ModelViewMatrix", Math::mat4( mv ) );
		program->setUniformValue( "u_NormalMatrix", Math::mat3( normalMatrix ) );
	}
}

void MeshRenderer::sortCommands()
{
	// Sort by shader first, then material, then depth
	// This minimizes state changes
	std::sort( _commands.begin(), _commands.end(),
			   [] ( const RenderCommand& a, const RenderCommand& b )
	{
		// Sort by shader pointer first
		if( a.drawState.shader.get() != b.drawState.shader.get() )
			return a.drawState.shader.get() < b.drawState.shader.get();

		// Then by material pointer
		if( a.material.get() != b.material.get() )
			return a.material.get() < b.material.get();

		// Then by sort key (typically depth)
		return a.sortKey < b.sortKey;
	}
	);
}

}
}