// Surface.cpp
//
#include "Surface.h"

#include "OpenGL.h"
#include "OpenGLWrap.h"
#include "OpenGLCheck.h"
#include "Texture.h"
#include "RenderEngine.h"


using namespace RenderSystem;

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
, _picker( std::make_shared<Picker>() )
{
	for( auto &i: init )
		createAttachment( i.attachPoint, i.textureFormat );
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
void Surface::resize( int32_t width, int32_t height )
{
	constexpr int32_t kMinSize = 0;
#if 0
	if( width < kMinSize || height < kMinSize )
		throw new ArgumentOutOfRangeException("Surface", "The surface width and height must be greater than or equal to zero.");
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
void Surface::attach( const FrameBuffer::AttachmentPoint &attachPoint, const Texture2DPtr &texture )
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
void Surface::createAttachment(  const FrameBuffer::AttachmentPoint& attachPoint, const TextureFormat& textureFormat )
{
	_fbo->attach( attachPoint,  Texture2D::New( TextureDescription( _width, _height, textureFormat ) ) );

#if _DEBUG
	_fbo->attachment( attachPoint )->setLabel( genLabelAttachment( attachPoint ) );
#endif
}

// ------------------------------------------------------------------------------------------------
void Surface::removeAttachment( const FrameBuffer::AttachmentPoint& attachPoint )
{
	_fbo->attach( attachPoint, nullptr );
}

// ------------------------------------------------------------------------------------------------
uint32_t Surface::colorAttachmentDrawBufferIndex( const FrameBuffer::AttachmentPoint& attachPoint ) const
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
//void Surface::setPicker( const PickerPtr &picker, const FrameBuffer::AttachmentPoint attachPoint )
//{	
//	if( picker == _picker && (!_picker || attachPoint == _pickerAttachmentPoint ) )
//		return;
//
//	if( _picker )
//		_picker->detachFromSurface();
//
//	if( picker )
//		picker->detachFromSurface();
//
//	_picker = picker;
//	_pickerAttachmentPoint = attachPoint;
//}

// ------------------------------------------------------------------------------------------------
bool Surface::setPickingEnabled( const bool enable )
{
	if( !_picker )
		return !enable;

	if( enable != _picker->isAttached() )
		if( enable )
			_picker->attachToSurface( this, kPickerAttachmentPoint );
		else
			_picker->detachFromSurface();
	
	return true;	
}

// ------------------------------------------------------------------------------------------------
bool Surface::isPickingEnabled() const
{
	return _picker && /*_picker->isAttached() &&*/ _picker->surface() == this;
}

// ------------------------------------------------------------------------------------------------
bool Surface::clearPickBuffer()
{
	if( !_picker )
		return false;

	if( !_picker->isAttached() )
		_picker->attachToSurface( this, kPickerAttachmentPoint );

	_picker->clearPickBuffer();
	return true;
}

// ------------------------------------------------------------------------------------------------
Picker::Value Surface::pickValueAt( int32_t x, int32_t y )
{
	if( !_picker )
		return Picker::kClearValue;

	if( !_picker->isAttached() )
		_picker->attachToSurface( this, kPickerAttachmentPoint );

	return _picker->getValueAt( x, y );
}

// ------------------------------------------------------------------------------------------------
void Surface::clear( const ClearState &cs ) const
{
	RenderEngine::clear( _fbo, cs );
}

// ------------------------------------------------------------------------------------------------
static inline DrawState sanitizeDrawState( const DrawState& ds, Surface const* s )
{
	DrawState out( ds );

	// sanitize Viewport
	if( out.viewState.viewport.isEmpty() )
		out.viewState.viewport = Math::irect ( 0, 0, s->width(), s->height() );

	// ...

	return out;
}

// ------------------------------------------------------------------------------------------------
void Surface::draw( const PrimitiveType &primitiveType, const VertexArrayPtr &vao, const DrawState &ds )           const { RenderEngine::draw( _fbo, primitiveType , vao, sanitizeDrawState( ds, this ) );}
void Surface::draw( const PrimitiveType &primitiveType, const PrimitiveBufferPtr &primitive, const DrawState &ds ) const { RenderEngine::draw( _fbo, primitiveType, primitive, sanitizeDrawState( ds, this ) ); }
void Surface::draw( const PrimitiveBatch& batch, const DrawState& ds )                                             const { RenderEngine::draw( _fbo, batch, sanitizeDrawState( ds, this ) );}

// ------------------------------------------------------------------------------------------------
void Surface::swap( uint32_t targetFBO, const FrameBuffer::AttachmentPoint &att ) const
{
	DrawState ds( Resources::DefaultShaders.FullscreenQuad );
	ds.textureUnits[ 0 ].setTexture( _fbo->attachment( att ) );
	ds.textureUnits[ 0 ].setSampler( Resources::DefaultSamplers.LinearClamp );
	ds.renderState.depthTest.enabled   = false;
	ds.renderState.faceCulling.enabled = false;
	ds.viewState.viewport = Math::irect( 0,0, _width, _height );

	PrimitiveBufferPtr      quad = PrimitiveBuffer::New(); 
	std::vector<Math::vec3> vertices  = { {-1,1,0},{-1,-1,0},{1,1,0},{1,-1,0} }; 
	std::vector<Math::vec2> texCoords = { {0,1},{0,0},{1,1},{1,0} }; 
 
	quad->setVertices( vertices ); 
	quad->setTextureCoords( texCoords ); 

	RenderEngine::draw( targetFBO, PrimitiveType::TriangleStrip, quad, ds );
}

// convenience method. same as swap( Context::current()->dafaultFBO(), FrameBuffer::ColorAttachment0 );
// ------------------------------------------------------------------------------------------------
void Surface::blit() const
{
	//
	swap( RenderEngine::defaultFrameBufferObject(), FrameBuffer::ColorAttachment0 );
}

// ------------------------------------------------------------------------------------------------
Pixmap<uint8_t> Surface::grabImage() const
{
	// @todo: ok for multisample buffers?
	return RenderEngine::readPixels( _fbo, _width, _height );
}

// ------------------------------------------------------------------------------------------------
/**
	This is a convenience function
*/
void Surface::readPixels( const FrameBuffer::AttachmentPoint attachPoint, const ImageFormat pixelFormat, 
						  const Math::irect &roi, float   *pixels ) const       

{ 
	_fbo->bind();
	_fbo->readPixels( attachPoint, pixelFormat, roi, pixels );
} 

// ------------------------------------------------------------------------------------------------
/**
This is a convenience function
*/
void Surface::Surface::readPixels( const FrameBuffer::AttachmentPoint attachPoint, const ImageFormat pixelFormat, 
								   const Math::irect&roi, uint8_t *pixels ) const
{ 
	_fbo->bind();
	_fbo->readPixels( attachPoint, pixelFormat, roi, pixels );
} 

// ------------------------------------------------------------------------------------------------
void Surface::Surface::Surface::readPixels( const FrameBuffer::AttachmentPoint attachPoint, const ImageFormat pixelFormat, 
											const ImageDataType pixelType, const Math::irect&roi, void *pixels )
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
		label += "_ColorAttachment" + std::to_string( attachPoint );
#endif
	return label;
}

