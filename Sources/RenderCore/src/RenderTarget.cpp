// RenderTarget.cpp
//
#include "RenderTarget.h"

#include "Context.h"
#include "Texture.h"
#include "Renderer.h"


using namespace RenderCore;

// ------------------------------------------------------------------------------------------------
RenderTarget::RenderTarget()
	: RenderTarget( { {FrameBuffer::ColorAttachment0,TextureFormat::RedGreenBlueAlpha8}, 
			     {FrameBuffer::DepthAttachment, TextureFormat::Depth24} } )
{
	assert( Context::current() );
}

// ------------------------------------------------------------------------------------------------
RenderTarget::RenderTarget( const std::initializer_list<AttachmentDescription> &init )
: _width( 64 )
, _height( 64 )
, _fbo( FrameBuffer::New() )
{
	for( auto &i: init )
		createAttachment( i.attachPoint, i.textureFormat );
}

// ------------------------------------------------------------------------------------------------
void RenderTarget::setLabel( const std::string &label )
{
#if _DEBUG	
	_fbo->setLabel( label );

	for( int i= FrameBuffer::ColorAttachment0; i<= FrameBuffer::DepthStencilAttachment; ++i )
		if( Texture2DPtr a = _fbo->attachment( FrameBuffer::AttachmentPoint(i) ) )
			a->setLabel( genLabelAttachment( FrameBuffer::AttachmentPoint(i) ) );
#endif
}

// ------------------------------------------------------------------------------------------------
void RenderTarget::resize( int32_t width, int32_t height )
{
	constexpr int32_t kMinSize = 0;
#if 0
	if( width < kMinSize || height < kMinSize )
		throw new ArgumentOutOfRangeException("RenderTarget", "The surface width and height must be greater than or equal to zero.");
#endif

	width  = std::max( width,  kMinSize );
	height = std::max( height, kMinSize );

	if( _width == width && _height == height )
		return;

	_width  = width;
	_height = height;
 
	for( int i= FrameBuffer::ColorAttachment0; i<= FrameBuffer::DepthStencilAttachment; ++i )
		if( Texture2DPtr a = _fbo->attachment( FrameBuffer::AttachmentPoint(i) ) )
			a->resize( _width, _height );
}

// ------------------------------------------------------------------------------------------------
void RenderTarget::attach( const FrameBuffer::AttachmentPoint &attachPoint, const Texture2DPtr &texture )
{
	if( texture )
	{
		const auto des = texture->description();
		if( des.width() != _width || des.height() != _height )
			texture->resize( _width, _height );
	}

	_fbo->attach( attachPoint, texture );
}

// ------------------------------------------------------------------------------------------------
void RenderTarget::createAttachment(  const FrameBuffer::AttachmentPoint& attachPoint, const TextureFormat& textureFormat )
{
	_fbo->attach( attachPoint,  Texture2D::New( TextureDescription( _width, _height, textureFormat ) ) );

#if _DEBUG
	_fbo->attachment( attachPoint )->setLabel( genLabelAttachment( attachPoint ) );
#endif
}

// ------------------------------------------------------------------------------------------------
void RenderTarget::removeAttachment( const FrameBuffer::AttachmentPoint& attachPoint )
{
	_fbo->attach( attachPoint, nullptr );
}

// ------------------------------------------------------------------------------------------------
uint32_t RenderTarget::colorAttachmentDrawBufferIndex( const FrameBuffer::AttachmentPoint& attachPoint ) const
{
	_fbo->bind();
	return _fbo->colorAttachmentDrawBufferIndex( attachPoint );
}

// ------------------------------------------------------------------------------------------------
Texture2DPtr RenderTarget::attachment( const FrameBuffer::AttachmentPoint &a ) const
{
	return _fbo->attachment( a );
}

// ------------------------------------------------------------------------------------------------
void RenderTarget::clear( const ClearState &cs ) const
{
	Renderer::clear( _fbo, cs );
}

// ------------------------------------------------------------------------------------------------
static inline DrawState sanitizeDrawState( const DrawState& ds, RenderTarget const* s )
{
	DrawState out( ds );

	// sanitize Viewport
	if( out.viewState.viewport.isEmpty() )
		out.viewState.viewport = Math::irect ( 0, 0, s->width(), s->height() );

	// ...

	return out;
}

// ------------------------------------------------------------------------------------------------
void RenderTarget::draw( const PrimitiveType &primitiveType, const VertexArrayPtr &vao, const DrawState &ds )           const { Renderer::draw( _fbo, primitiveType , vao, sanitizeDrawState( ds, this ) );}
void RenderTarget::draw( const PrimitiveType &primitiveType, const PrimitiveBufferPtr &primitive, const DrawState &ds ) const { Renderer::draw( _fbo, primitiveType, primitive, sanitizeDrawState( ds, this ) ); }
void RenderTarget::draw( const PrimitiveBatch& batch, const DrawState& ds )                                             const { Renderer::draw( _fbo, batch, sanitizeDrawState( ds, this ) );}

// ------------------------------------------------------------------------------------------------
Pixmap<uint8_t> RenderTarget::grabImage() const
{
	// @todo: ok for multisample buffers?
	return Renderer::readPixels( _fbo, _width, _height );
}

// ------------------------------------------------------------------------------------------------
/**
	This is a convenience function
*/
void RenderTarget::readPixels( const FrameBuffer::AttachmentPoint &attachPoint,
							   const ImageFormat &pixelFormat, 
							   const Math::irect &roi, float   *pixels ) const       

{ 
	_fbo->bind();
	_fbo->readPixels( attachPoint, pixelFormat, roi, pixels );
} 

// ------------------------------------------------------------------------------------------------
// This is a convenience function
void RenderTarget::RenderTarget::readPixels( const FrameBuffer::AttachmentPoint &attachPoint, 
											 const ImageFormat &pixelFormat, 
											 const Math::irect&roi, uint8_t *pixels ) const
{ 
	_fbo->bind();
	_fbo->readPixels( attachPoint, pixelFormat, roi, pixels );
} 

// ------------------------------------------------------------------------------------------------
void RenderTarget::RenderTarget::RenderTarget::readPixels( const FrameBuffer::AttachmentPoint &attachPoint, 
														   const ImageFormat &pixelFormat, 
														   const ImageDataType &pixelType, 
														   const Math::irect&roi, void *pixels )
{ 
	_fbo->bind();
	_fbo->readPixels( attachPoint, pixelFormat, pixelType, roi, pixels );
} 

// ------------------------------------------------------------------------------------------------
inline std::string RenderTarget::genLabelAttachment( const FrameBuffer::AttachmentPoint &attachPoint ) const
{
	std::string label;
#if _DEBUG
	label = _fbo->label();
	if( attachPoint == FrameBuffer::DepthAttachment )             label += "_DepthAttachment";
	else if( attachPoint == FrameBuffer::DepthStencilAttachment ) label += "_DepthStencilAttachment";
	else if( attachPoint > FrameBuffer::DepthStencilAttachment )  label += "_UnknownAttachment";
	else                                                          label += "_ColorAttachment" + std::to_string( attachPoint );
#endif
	return label;
}