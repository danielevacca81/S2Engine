// RenderEngine.cpp
//
#include "RenderEngine.h"

#include "OpenGL.h"
#include "OpenGLCheck.h"
#include "OpenGLWrap.h"
#include "Context.h"

using namespace RenderSystem;

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
uint32_t RenderEngine::defaultFrameBufferObject()
{
	return Context::current()->_defaultFBO;
}

// ------------------------------------------------------------------------------------------------
void RenderEngine::clear( const FrameBufferPtr &fbo, const ClearState& cs )
{
	if( !fbo )
		return; // assert

	fbo->bind();
	Context::current()->_stateManager.setClearState( cs );
}

// ------------------------------------------------------------------------------------------------
void RenderEngine::draw( const FrameBufferPtr& fbo, const PrimitiveType& primitiveType, const VertexArrayPtr& va, const DrawState& ds )
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
void RenderEngine::draw( const FrameBufferPtr& fbo, const PrimitiveType& primitiveType, const PrimitiveBufferPtr& p, const DrawState& ds )
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
void RenderEngine::draw( const FrameBufferPtr& fbo, const PrimitiveBatch& batch, const DrawState& ds )
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
void RenderEngine::draw( uint32_t targetFBO, const PrimitiveType& primitiveType, const PrimitiveBufferPtr& primitive, const DrawState& ds )
{
	if( !primitive )
		return;

	glBindFramebuffer( GL_FRAMEBUFFER, targetFBO );
	glCheck;

	Context::current()->_stateManager.setDrawState( ds );
	::draw( primitiveType, primitive->_vao );
}

// ------------------------------------------------------------------------------------------------
Pixmap<uint8_t> RenderEngine::readPixels( const FrameBufferPtr& fbo, uint32_t width, uint32_t height )
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
	glReadPixels( 0, 0, width, height, glWrap( format ), glWrap( dataType ), DATA_PTR( 0 ) ); // todo: remove direct calls to OpenGL from the surface class. Use of OpenGL is delegated to lower level classes (ex: Context)
	glCheck;

	Pixmap<uint8_t> img( width, height, 4, (uint8_t*) pixelBuffer.mapData() );
	pixelBuffer.unmapData();
	pixelBuffer.unbind();
	fbo->unbind();
	
	return img;
}
