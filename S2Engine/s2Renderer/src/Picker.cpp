// Picker.cpp
//
#include "Picker.h"

#include "TextureDescription.h"
#include "Texture.h"
#include "ClearState.h"
#include "Surface.h"

#include <cassert>

using namespace Renderer;

// ------------------------------------------------------------------------------------------------
const  Math::ivec4    Picker::kClearColor( Picker::kClearValue );

// ------------------------------------------------------------------------------------------------
Picker::Picker()
:  _surface( nullptr )
,  _attachmentPoint(FrameBuffer::AttachmentPoint::ColorAttachment1 )
{
}

// ------------------------------------------------------------------------------------------------
void Picker::reset()
{
	detachFromSurface();
	_pickTexture = nullptr;
}

// ------------------------------------------------------------------------------------------------
void Picker::attachToSurface( Surface *surface, 
							  const FrameBuffer::AttachmentPoint pickAttachmentPoint )
{	
	if( surface == _surface && ( !_surface || pickAttachmentPoint == _attachmentPoint ) )
		return;

	detachFromSurface();

	_surface = surface;
	_attachmentPoint = pickAttachmentPoint;

	if( !_surface )
		return;

	if( !_pickTexture )
		_pickTexture = Texture2D::New( TextureDescription( _surface->width(), _surface->height(),TextureFormat::Red32i ) );

	_surface->attach( _attachmentPoint, _pickTexture );
}

// ------------------------------------------------------------------------------------------------
void Picker::detachFromSurface()
{	
	if( !_surface )
		return;

	_surface->removeAttachment( _attachmentPoint );
	_surface = nullptr;
}

// ------------------------------------------------------------------------------------------------
/**
	the picker must be attached to a surface (Picker::attachToSurface) for this call to succeed
*/
bool Picker::clearPickBuffer()
{
	if( !_surface ) 
		return false;

	const auto idx = _surface->colorAttachmentDrawBufferIndex( _attachmentPoint );
	if( idx == -1 )
		return false;

	Renderer::ClearState cs;
	cs.buffers = ClearBuffers::ColorBuffer;
	cs.colorSeparate.push_back( {idx, std::any(kClearColor)} );
	_surface->clear( cs );
	return true;
}

// ------------------------------------------------------------------------------------------------
/**
	the picker must be attached to a surface (Picker::attachToSurface) for this call to succeed
*/
Picker::Value Picker::getValueAt( int x, int y ) const
{
	// alternativa, usa:  class S2RENDERER_API ReadPixelBuffer (?)

	assert( _surface );
	if( !_surface || x < 0 || x >= _surface->width() || y < 0 ||  y >= _surface->height() )
		return kClearValue;

	Value retval;
    _surface->readPixels( _attachmentPoint, ImageFormat::RedInteger, ImageDataType::Int, Math::Rectangle(x,y,1,1), &retval );

	return retval;
}



