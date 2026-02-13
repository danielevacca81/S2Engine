// RenderCommands.cpp
//
#include "RenderCommands.h"

#include "Context.h"
#include "RenderTarget.h"
#include "OpenGL.h"
#include "OpenGLCheck.h"
#include "OpenGLWrap.h"

using namespace s2::RenderCore;

// ------------------------------------------------------------------------------------------------
RenderCommands::RenderCommands( Context& context )
	: _context( context )
{
}

// ================================================================================================
// CLEAR OPERATIONS
// ================================================================================================

void RenderCommands::clear( const RenderTarget& target, const ClearState& cs )
{
	clear( target.fbo(), cs );
}

// ------------------------------------------------------------------------------------------------
void RenderCommands::clear( const FrameBufferPtr& fbo, const ClearState& cs )
{
	if( !fbo )
		return;

	fbo->bind();
	_context._stateManager.setClearState( cs );
}

// ================================================================================================
// DRAW OPERATIONS
// ================================================================================================

void RenderCommands::draw( const RenderTarget& target, const PrimitiveType& primitiveType,
						  const VertexArrayPtr& va, const DrawState& ds )
{
	draw( target.fbo(), primitiveType, va, sanitizeDrawState( ds, target ) );
}

// ------------------------------------------------------------------------------------------------
void RenderCommands::draw( const RenderTarget& target, const PrimitiveType& primitiveType,
						  const VertexDataPtr& primitive, const DrawState& ds )
{
	draw( target.fbo(), primitiveType, primitive, sanitizeDrawState( ds, target ) );
}

// ------------------------------------------------------------------------------------------------
void RenderCommands::draw( const RenderTarget& target, const PrimitiveBatch& batch, const DrawState& ds )
{
	if( !target.fbo() )
		return;

	auto &b = batch.batch();

	auto pBuffer = VertexData::New();
	pBuffer->setVertices( b.vertices );
	pBuffer->setTextureCoords( b.textureCoords );
	pBuffer->setColors( b.colors );
	pBuffer->setNormals( b.normals );
	pBuffer->setIndices( b.indices );

	auto ds2 = sanitizeDrawState( ds, target );
	ds2.renderState.primitiveRestart.enabled = true;
	ds2.renderState.primitiveRestart.index = batch.primitiveRestartIndex();

	draw( target.fbo(), batch.primitiveType(), pBuffer, ds2 );
}

// ------------------------------------------------------------------------------------------------
void RenderCommands::draw( const FrameBufferPtr& fbo, const PrimitiveType& primitiveType,
						  const VertexArrayPtr& va, const DrawState& ds )
{
	if( !fbo || !va )
		return;

	fbo->bind();
	_context._stateManager.setDrawState( ds );
	executeDrawCall( primitiveType, va );
}

// ------------------------------------------------------------------------------------------------
void RenderCommands::draw( const FrameBufferPtr& fbo, const PrimitiveType& primitiveType,
						  const VertexDataPtr& primitive, const DrawState& ds )
{
	if( !fbo || !primitive )
		return;

	fbo->bind();
	_context._stateManager.setDrawState( ds );
	executeDrawCall( primitiveType, primitive->_vao );
}

// ================================================================================================
// READ OPERATIONS
// ================================================================================================

Pixmap<uint8_t> RenderCommands::readPixels( const RenderTarget& target )
{
	return readPixels( target.fbo(), target.width(), target.height() );
}

// ------------------------------------------------------------------------------------------------
Pixmap<uint8_t> RenderCommands::readPixels( const FrameBufferPtr& fbo, uint32_t width, uint32_t height )
{
	if( !fbo || width == 0 || height == 0 )
		return {};

	const int rowAlignment = 4;
	const ImageFormat format = ImageFormat::RedGreenBlueAlpha;
	const ImageDataType dataType = ImageDataType::UnsignedByte;
	const int sizeInBytes = computeRequiredSizeInBytes( width, height, format, dataType, rowAlignment );

	fbo->bind();

	ReadPixelBuffer pixelBuffer( sizeInBytes, ReadPixelBuffer::UsageHint::Static );
	pixelBuffer.bind();

	glReadBuffer( FrameBuffer::ColorAttachment0 );
	glReadPixels( 0, 0, width, height, glWrap( format ), glWrap( dataType ), DATA_PTR( 0 ) );
	glCheck;

	Pixmap<uint8_t> img( width, height, 4, (uint8_t*) pixelBuffer.mapData() );
	pixelBuffer.unmapData();
	pixelBuffer.unbind();
	fbo->unbind();

	return img;
}

// ------------------------------------------------------------------------------------------------
void RenderCommands::readPixels( const RenderTarget& target,
								const FrameBuffer::AttachmentPoint& attachPoint,
								const ImageFormat& pixelFormat,
								const Math::irect& roi,
								void* pixels )
{
	target.fbo()->bind();
	target.fbo()->readPixels( attachPoint, pixelFormat, ImageDataType::UnsignedByte, roi, pixels );
}

// ================================================================================================
// BLIT OPERATIONS
// ================================================================================================
// ---------------------------------------------------------------------------------
// Blit from source to destination render target. If srcRect or dstRect are empty, the entire source/destination is used.
void RenderCommands::blit( const RenderTarget& source, const RenderTarget& destination,
						  const Math::irect& srcRect, const Math::irect& dstRect )
{
	Math::irect src = srcRect.isEmpty() ? Math::irect( 0, 0, source.width(), source.height() ) : srcRect;
	Math::irect dst = dstRect.isEmpty() ? Math::irect( 0, 0, destination.width(), destination.height() ) : dstRect;

	blit( source.fbo(), destination.fbo(), src, dst );
}

// ------------------------------------------------------------------------------------------------
// Blit from source FBO to destination FBO. If srcRect or dstRect are empty, the entire source/destination is used.
void RenderCommands::blit( const FrameBufferPtr& srcFBO, const FrameBufferPtr& dstFBO,
						  const Math::irect& srcRect, const Math::irect& dstRect )
{
	if( !srcFBO )
		return;

	const uint32_t srcFBOId = srcFBO->id();
	const uint32_t dstFBOId = dstFBO ? dstFBO->id() : 0; // 0 = default FBO
	const auto destRect = dstRect.isEmpty() ? srcRect : dstRect;

	glBindFramebuffer( GL_READ_FRAMEBUFFER, srcFBOId );
	glBindFramebuffer( GL_DRAW_FRAMEBUFFER, dstFBOId );
	glBlitFramebuffer( srcRect.left(), srcRect.bottom(), srcRect.right(), srcRect.top(),
					   destRect.left(), destRect.bottom(), destRect.right(), destRect.top(),
					   GL_COLOR_BUFFER_BIT, GL_NEAREST );
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	glCheck;
}

// ------------------------------------------------------------------------------------------------
// Blit from source render target to the default framebuffer (screen). If srcRect is empty, the entire source is used.
void RenderCommands::blitToScreen( const RenderTarget& source, const Math::irect& srcRect )
{
	const Math::irect src = srcRect.isEmpty() ? Math::irect( 0, 0, source.width(), source.height() ) : srcRect;
	blit( source.fbo(), nullptr, src, {} );
}

// ================================================================================================
// UTILITY OPERATIONS
// ================================================================================================

void RenderCommands::drawFullscreenQuad( const Texture2DPtr& srcTexture )
{
	if( !srcTexture )
		return;

	DrawState fullscreenQuadDrawState;
	fullscreenQuadDrawState.shader                          = DefaultShaders.FullscreenQuad;
	fullscreenQuadDrawState.renderState.depthTest.enabled   = false;
	fullscreenQuadDrawState.renderState.faceCulling.enabled = false;
	fullscreenQuadDrawState.viewport.rect                   = Math::irect( 0, 0, 
																		   srcTexture->description().width(),
																		   srcTexture->description().height() );
	fullscreenQuadDrawState.textureUnits[0].set( srcTexture, DefaultSamplers.LinearClamp );

	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	glCheck;

	_context._stateManager.setDrawState( fullscreenQuadDrawState );
	glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
}

// ================================================================================================
// PRIVATE HELPERS
// ================================================================================================

void RenderCommands::executeDrawCall( const PrimitiveType& primitive, const VertexArrayPtr& va )
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
DrawState RenderCommands::sanitizeDrawState( const DrawState& ds, const RenderTarget& target ) const
{
	DrawState out( ds );

	// Sanitize viewport
	if( out.viewport.rect.isEmpty() )
		out.viewport.rect = Math::irect( 0, 0, target.width(), target.height() );

	return out;
}