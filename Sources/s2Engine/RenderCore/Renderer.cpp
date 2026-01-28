// Renderer.cpp
//
#include "Renderer.h"

#include "OpenGL.h"
#include "OpenGLCheck.h"
#include "OpenGLWrap.h"
#include "Context.h"

using namespace RenderCore;

//-------------------------------------------------------------------------------------------------
static inline void draw( const PrimitiveType& primitive, const VertexArrayPtr& va )
{
	va->bind();

	if( va->isIndexed() )
	{
		glDrawRangeElements( glWrap( primitive ),
							 0,
							 va->maxArrayIndex(),
							 va->indexBuffer().count(),
							 glWrap( va->indexBuffer().dataType() ),
							 DATA_PTR( 0 ) );
		glCheck;
	}
	else
	{
		glDrawArrays( glWrap( primitive ), 0, va->maxArrayIndex() + 1 );
		glCheck;
	}
}

// ------------------------------------------------------------------------------------------------
//uint32_t Renderer::defaultFrameBufferObject()
//{
//	return Context::current()->_defaultFBO;
//}

// ------------------------------------------------------------------------------------------------
void Renderer::clear( const FrameBufferPtr &fbo, const ClearState& cs )
{
	if( !fbo )
		return; // assert

	fbo->bind();
	Context::current()->_stateManager.setClearState( cs );
}

// ------------------------------------------------------------------------------------------------
void Renderer::draw( const FrameBufferPtr& fbo, const PrimitiveType& primitiveType, const VertexArrayPtr& va, const DrawState& ds )
{
	if( !fbo )
		return;

	if( !va )
		return;

	//assert( fb->context().get() == this );
	//assert( isCurrent() );


	fbo->bind();
	Context::current()->_stateManager.setDrawState( ds );

	::draw( primitiveType, va );
}

// ------------------------------------------------------------------------------------------------
void Renderer::draw( const FrameBufferPtr& fbo, const PrimitiveType& primitiveType, const PrimitiveBufferPtr& p, const DrawState& ds )
{
	if( !fbo )
		return;

	if( !p )
		return;
	//assert( fb->context().get() == this );
	//assert( isCurrent() );

	fbo->bind();
	Context::current()->_stateManager.setDrawState( ds );

	::draw( primitiveType, p->_vao );
}

// ------------------------------------------------------------------------------------------------
void Renderer::draw( const FrameBufferPtr& fbo, const PrimitiveBatch& batch, const DrawState& ds )
{
	if( !fbo )
		return;

	//assert( fb->context().get() == this );
	//assert( isCurrent() );

	auto b = batch.batch();

	auto pBuffer = PrimitiveBuffer::New();
	pBuffer->setVertices( b.vertices );
	pBuffer->setTextureCoords( b.textureCoords );
	pBuffer->setColors( b.colors );
	pBuffer->setNormals( b.normals );
	pBuffer->setIndices( b.indices );

	auto ds2 = ds;
	ds2.renderState.primitiveRestart.enabled = true;
	ds2.renderState.primitiveRestart.index = batch.primitiveRestartIndex();


	fbo->bind();
	Context::current()->_stateManager.setDrawState( ds );

	draw( fbo, batch.primitiveType(), pBuffer, ds2 );
}

// ------------------------------------------------------------------------------------------------
void Renderer::draw( uint32_t targetFBO, const PrimitiveType& primitiveType, const PrimitiveBufferPtr& primitive, const DrawState& ds )
{
	if( !primitive )
		return;

	glBindFramebuffer( GL_FRAMEBUFFER, targetFBO );
	glCheck;

	Context::current()->_stateManager.setDrawState( ds );
	::draw( primitiveType, primitive->_vao );
}

// ------------------------------------------------------------------------------------------------
Pixmap<uint8_t> Renderer::readPixels( const FrameBufferPtr& fbo, uint32_t width, uint32_t height )
{
	if( !fbo )
		return {};

	if( width == 0 || height == 0 )
		return {};

	const int rowAlignment       = 4;
	const ImageFormat format     = ImageFormat::RedGreenBlueAlpha;
	const ImageDataType dataType = ImageDataType::UnsignedByte;
	const int sizeInBytes        = computeRequiredSizeInBytes( width, height, format, dataType, rowAlignment );

	fbo->bind();

	ReadPixelBuffer pixelBuffer = ReadPixelBuffer( sizeInBytes, ReadPixelBuffer::UsageHint::Static );
	pixelBuffer.bind();

	glReadBuffer( FrameBuffer::ColorAttachment0 );
	glReadPixels( 0, 0, width, height, glWrap( format ), glWrap( dataType ), DATA_PTR( 0 ) ); // todo: remove direct calls to OpenGL
	glCheck;

	Pixmap<uint8_t> img( width, height, 4, (uint8_t*) pixelBuffer.mapData() );
	pixelBuffer.unmapData();
	pixelBuffer.unbind();
	fbo->unbind();
	
	return img;
}

// ------------------------------------------------------------------------------------------------
void Renderer::blit( const FrameBufferPtr& srcFBO, const FrameBufferPtr& dstFBO, const Math::irect& srcRect, const Math::irect& dstRect )
{
	if( !srcFBO )
		return; // assert( srcFBO );
	
	const uint32_t srcFBOId = srcFBO->id();
	const uint32_t dstFBOId = dstFBO ? dstFBO->id() : 0; // 0 means default FBO
	const auto destRect     = dstRect.isEmpty() ? srcRect : dstRect; // if dstRect is empty, use srcRect

#if 0	// ( opengl > 4.5)
	glBlitNamedFramebuffer( srcFBOId, dstFBOId,
							srcRect.left(), srcRect.bottom(), srcRect.right(), srcRect.top(), 
							dstRect.left(), dstRect.bottom(), dstRect.right(), dstRect.top(),
							GL_COLOR_BUFFER_BIT, GL_NEAREST );
#else
	glBindFramebuffer( GL_READ_FRAMEBUFFER, srcFBOId );
	glBindFramebuffer( GL_DRAW_FRAMEBUFFER, dstFBOId );
	glBlitFramebuffer( srcRect.left(), srcRect.bottom(), srcRect.right(), srcRect.top(),
					   destRect.left(), destRect.bottom(), destRect.right(), destRect.top(),
					   GL_COLOR_BUFFER_BIT, GL_NEAREST );

	glBindFramebuffer( GL_FRAMEBUFFER, 0 ); // unbind any FBO
#endif


	glCheck;
}

// ------------------------------------------------------------------------------------------------
void Renderer::drawFullscreenQuad( const Texture2DPtr& srcTexture )
{
	if( !srcTexture )
		return;

	DrawState fullscreenQuadDrawState;
	{
		fullscreenQuadDrawState.shader                          = DefaultShaders.FullscreenQuad;
		fullscreenQuadDrawState.renderState.depthTest.enabled   = false;
		fullscreenQuadDrawState.renderState.faceCulling.enabled = false;
		fullscreenQuadDrawState.viewState.viewport              = Math::irect( 0, 0, srcTexture->description().width(), srcTexture->description().height() );
		fullscreenQuadDrawState.textureUnits[0].set( srcTexture, DefaultSamplers.LinearClamp );
	}

	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	glCheck;
	
	Context::current()->_stateManager.setDrawState( fullscreenQuadDrawState );
	glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
}
