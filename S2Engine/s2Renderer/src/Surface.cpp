// Surface.cpp
//
#include "Surface.h"

#include "Renderer.h"
#include "OpenGL.h"
#include "OpenGLWrap.h"
#include "Texture.h"
#include "TextureUnit.h"
#include "Sampler.h"
#include "BuiltIn.h"

#include <iostream>

using namespace Renderer;

// @todo: link to context?

// ------------------------------------------------------------------------------------------------
SurfacePtr Surface::New()
{
	return std::make_shared<Surface>();
}

// ------------------------------------------------------------------------------------------------
Surface::Surface()
	: Surface( { {FrameBuffer::ColorAttachment0,TextureFormat::RedGreenBlueAlpha8}, 
			     {FrameBuffer::DepthAttachment, TextureFormat::Depth24} } )
{
}

// ------------------------------------------------------------------------------------------------
Surface::Surface( std::initializer_list<AttachmentDescription> init )
: _width( 64 )
, _height( 64 )
, _fbo( FrameBuffer::New() )
, _pickerAttachmentPoint( FrameBuffer::AttachmentPoint::ColorAttachment1 )
{
	_state.setViewport( Viewport( 0,0, _width, _height ) );

	for( auto &i: init )
		createAttachment( i.attachPoint, i.textureFormat );
}

// ------------------------------------------------------------------------------------------------
Surface::~Surface()
{
	setPicker( nullptr );
}

// ------------------------------------------------------------------------------------------------
void Surface::setLabel( const std::string &label )
{
#if _DEBUG	
	_fbo->setLabel( label );

	for( int i= FrameBuffer::ColorAttachment0; i<= FrameBuffer::DepthStencilAttachment; ++i )
		if( Texture2DPtr a = _fbo->attachment( FrameBuffer::AttachmentPoint(i) ) )
			a->setLabel( genLabelAttachment( FrameBuffer::AttachmentPoint(i) ) );
#endif
}

// ------------------------------------------------------------------------------------------------
void Surface::clear( const ClearState &cs )
{
	_fbo->bind();
	_state.setClearState( cs );
}

// ------------------------------------------------------------------------------------------------
void Surface::draw( const PrimitiveType &primitive, const VertexArrayPtr &vao, const DrawingState &ds )
{
	if( !vao )
		return;

	_fbo->bind();
	_state.setDrawState( ds );

	RenderingHelper::draw( primitive, vao );
}

// ------------------------------------------------------------------------------------------------
void Surface::draw( const PrimitiveType &primitiveType, const PrimitiveBufferPtr &p, const DrawingState &ds )
{
	if( !p )
		return;

	_fbo->bind();
	_state.setDrawState( ds );

	RenderingHelper::draw( primitiveType, p );
}

// ------------------------------------------------------------------------------------------------
void Surface::resize( int width, int height )
{
	if( _width == width && _height == height )
		return;

	_width  = width;
	_height = height;

	_state.setViewport( Viewport( 0,0, _width, _height ) );
	 
	for( int i= FrameBuffer::ColorAttachment0; i<= FrameBuffer::DepthStencilAttachment; ++i )
		if( Texture2DPtr a = _fbo->attachment( FrameBuffer::AttachmentPoint(i) ) )
			a->resize( _width, _height );
}

// ------------------------------------------------------------------------------------------------
void Surface::attach( const FrameBuffer::AttachmentPoint attachPoint, const Texture2DPtr &texture )
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
void Surface::createAttachment(  const FrameBuffer::AttachmentPoint attachPoint, const TextureFormat textureFormat )
{
	_fbo->attach( attachPoint,  Texture2D::New( TextureDescription( _width, _height, textureFormat ) ) );

#if _DEBUG
	_fbo->attachment( attachPoint )->setLabel( genLabelAttachment( attachPoint ) );
#endif
}

// ------------------------------------------------------------------------------------------------
void Surface::removeAttachment( const FrameBuffer::AttachmentPoint attachPoint )
{
	_fbo->attach( attachPoint, nullptr );
}

// ------------------------------------------------------------------------------------------------
int Surface::colorAttachmentDrawBufferIndex( const FrameBuffer::AttachmentPoint attachPoint ) const
{
	_fbo->bind();
	return _fbo->colorAttachmentDrawBufferIndex( attachPoint );
}

// ------------------------------------------------------------------------------------------------
Texture2DPtr Surface::attachment( const FrameBuffer::AttachmentPoint &a ) const
{
	return _fbo->attachment( a );
}

// ------------------------------------------------------------------------------------------------
void Surface::setPicker( const PickerPtr &picker, const FrameBuffer::AttachmentPoint attachPoint )
{	
	if( picker == _picker && (!_picker || attachPoint == _pickerAttachmentPoint ) )
		return;

	if( _picker )
		_picker->detachFromSurface();

	if( picker )
		picker->detachFromSurface();

	_picker = picker;
	_pickerAttachmentPoint = attachPoint;
}

// ------------------------------------------------------------------------------------------------
bool Surface::setPickingEnabled( const bool enable )
{
	if( !_picker )
		return !enable;

	if( enable != _picker->isAttached() )
		if( enable )
			_picker->attachToSurface( this, _pickerAttachmentPoint );
		else
			_picker->detachFromSurface();
	
	return true;	
}

// ------------------------------------------------------------------------------------------------
bool Surface::clearPickBuffer()
{
	if( !_picker )
		return false;

	if( !_picker->isAttached() )
		_picker->attachToSurface( this, _pickerAttachmentPoint );

	_picker->clearPickBuffer();
	return true;
}

// ------------------------------------------------------------------------------------------------
Picker::Value Surface::pickValueAt( int x, int y ) 
{
	if( !_picker )
		return Picker::kClearValue;

	if( !_picker->isAttached() )
		_picker->attachToSurface( this, _pickerAttachmentPoint );

	return _picker->getValueAt( x, y );
}

// ------------------------------------------------------------------------------------------------
void Surface::swap( unsigned int targetFBO, const FrameBuffer::AttachmentPoint &att )
{
	glBindFramebuffer( GL_FRAMEBUFFER, targetFBO );
	glCheck;

	DrawingState ds( BuiltIn::shaderFullscreenQuad );
	ds.textureUnits[ 0 ].setTexture( _fbo->attachment( att ) );
	ds.textureUnits[ 0 ].setSampler( BuiltIn::samplerLinearClamp );

	_state.setDrawState( ds );

	PrimitiveBufferPtr      quad = PrimitiveBuffer::New(); 
	std::vector<Math::vec3> vertices  = { {-1,1,0},{-1,-1,0},{1,1,0},{1,-1,0} }; 
	std::vector<Math::vec2> texCoords = { {0,1},{0,0},{1,1},{1,0} }; 
 
	quad->setVertices( vertices ); 
	quad->setTextureCoords( texCoords ); 
 
	RenderingHelper::draw( PrimitiveType::TriangleStrip, quad ); 
}

// ------------------------------------------------------------------------------------------------
ImageBuffer<unsigned char> Surface::grabImage() const
{
	// @todo: ok for multisample buffers?
	
	//auto img = _fbo->attachment( FrameBuffer::ColorAttachment0 )->readData();
	const int rowAlignment       = 4;
	const ImageFormat format     = ImageFormat::RedGreenBlueAlpha;
	const ImageDataType dataType = ImageDataType::UnsignedByte;
	const int sizeInBytes        = computeRequiredSizeInBytes( _width, _height, format, dataType, rowAlignment );

	_fbo->bind();
	//glPixelStorei( GL_PACK_ALIGNMENT, rowAlignment );
	
	ReadPixelBuffer pixelBuffer = ReadPixelBuffer( sizeInBytes, ReadPixelBuffer::UsageHint::Static );
	pixelBuffer.bind();
	
	glReadPixels( 0,0, _width, _height, glWrap( format ), glWrap( dataType ), BUFFER_OFFSET(0) );
	glCheck;
	//unbind();
	//glCheck;

	ImageBuffer<uint8_t> img( _width, _height, 4, (uint8_t*)pixelBuffer.mapData() );
	pixelBuffer.unmapData();
	pixelBuffer.unbind();
	_fbo->unbind();

	return img;
}

// ------------------------------------------------------------------------------------------------
/**
	This is a convenience function
*/
void Surface::readPixels( const FrameBuffer::AttachmentPoint attachPoint, const ImageFormat pixelFormat, 
						  const Math::Rectangle &roi, float   *pixels ) const       

{ 
	_fbo->bind();
	_fbo->readPixels( attachPoint, pixelFormat, roi, pixels );
} 

// ------------------------------------------------------------------------------------------------
/**
This is a convenience function
*/
void Surface::Surface::readPixels( const FrameBuffer::AttachmentPoint attachPoint, const ImageFormat pixelFormat, 
								   const Math::Rectangle &roi, uint8_t *pixels ) const
{ 
	_fbo->bind();
	_fbo->readPixels( attachPoint, pixelFormat, roi, pixels );
} 

// ------------------------------------------------------------------------------------------------
void Surface::Surface::Surface::readPixels( const FrameBuffer::AttachmentPoint attachPoint, const ImageFormat pixelFormat, 
											const ImageDataType pixelType, const Math::Rectangle &roi, void *pixels )
{ 
	_fbo->bind();
	_fbo->readPixels( attachPoint, pixelFormat, pixelType, roi, pixels );
} 

// ------------------------------------------------------------------------------------------------
inline std::string Surface::genLabelAttachment( const FrameBuffer::AttachmentPoint attachPoint ) const
{
	std::string label;
#if _DEBUG
	label = _fbo->label();
	if( attachPoint == FrameBuffer::DepthAttachment )
		label += "_DepthAttachment";
	else if( attachPoint == FrameBuffer::DepthStencilAttachment )
		label += "_DepthStencilAttachment";
	else if( attachPoint > FrameBuffer::DepthStencilAttachment )
		label += "_UnknownAttachment";
	else 
		label += "_ColorAttachmen" + std::to_string( attachPoint );
#endif
	return label;
}

