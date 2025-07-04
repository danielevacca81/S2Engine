// SwapChain.cpp
// 
#include "SwapChain.h"

#include "RenderTarget.h"
#include "Renderer.h"

using namespace RenderCore;


// ------------------------------------------------------------------------------------------------
void SwapChain::swapToScreen( const RenderTarget& renderTarget )
{
	PrimitiveBufferPtr fullscreenQuad = PrimitiveBuffer::New();
	fullscreenQuad->setVertices( { {-1,1,0},{-1,-1,0},{1,1,0},{1,-1,0} } );
	fullscreenQuad->setTextureCoords( { {0,1},{0,0},{1,1},{1,0} } );

	DrawState fullscreenQuadDrawState = DrawState( Resources::DefaultShaders.FullscreenQuad );
	fullscreenQuadDrawState.renderState.depthTest.enabled   = false;
	fullscreenQuadDrawState.renderState.faceCulling.enabled = false;
	fullscreenQuadDrawState.viewState.viewport              = Math::irect( 0, 0, renderTarget.width(), renderTarget.height() );		
	fullscreenQuadDrawState.textureUnits[0].setSampler( Resources::DefaultSamplers.LinearClamp );
	fullscreenQuadDrawState.textureUnits[0].setTexture( renderTarget.attachment( FrameBuffer::ColorAttachment0 ) );


	Renderer::draw( Renderer::defaultFrameBufferObject(), PrimitiveType::TriangleStrip, fullscreenQuad, fullscreenQuadDrawState );
}