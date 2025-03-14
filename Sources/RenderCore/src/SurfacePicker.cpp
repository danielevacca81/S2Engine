// SurfacePicker.cpp
//
#include "SurfacePicker.h"

#include "TextureDescription.h"
#include "Texture.h"
#include "ClearState.h"
#include "Surface.h"

#include <cassert>

using namespace RenderCore;

// ------------------------------------------------------------------------------------------------
const Math::ivec4 gClearColor( PickerConstants::kClearValue );

// ------------------------------------------------------------------------------------------------
SurfacePicker::SurfacePicker()
:  _surface( nullptr )
,  _attachmentPoint(FrameBuffer::AttachmentPoint::ColorAttachment1 )
{
}

// ------------------------------------------------------------------------------------------------
void SurfacePicker::reset()
{
	detachFromSurface();
	_pickTexture = nullptr;
}

// ------------------------------------------------------------------------------------------------
void SurfacePicker::attachToSurface( Surface *surface, const FrameBuffer::AttachmentPoint &pickAttachmentPoint )
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
void SurfacePicker::detachFromSurface()
{	
	if( !_surface )
		return;

	_surface->removeAttachment( _attachmentPoint );
	_surface = nullptr;
}

// ------------------------------------------------------------------------------------------------
/**
	the picker must be attached to a surface (PickerConstants::attachToSurface) for this call to succeed
*/
bool SurfacePicker::clearPickBuffer()
{
	if( !_surface ) 
		return false;

	const auto idx = _surface->colorAttachmentDrawBufferIndex( _attachmentPoint );
	if( idx == -1 )
		return false;

	ClearState cs;
	cs.buffers = ClearBuffers::ColorBuffer;
	cs.colorSeparate.enabled = true;
	cs.colorSeparate.color[idx] = gClearColor;

	_surface->clear( cs );
	return true;
}

// ------------------------------------------------------------------------------------------------
/**
	the picker must be attached to a surface (PickerConstants::attachToSurface) for this call to succeed
*/
PickerConstants::Value SurfacePicker::getValueAt( int x, int y ) const
{
	// alternativa, usa:  class RENDERCORE_API ReadPixelBuffer (?)

	assert( _surface );
	if( !_surface || x < 0 || x >= _surface->width() || y < 0 ||  y >= _surface->height() )
		return PickerConstants::kClearValue;

	PickerConstants::Value retval;
    _surface->readPixels( _attachmentPoint, ImageFormat::RedInteger, ImageDataType::Int, Math::irect(x,y,1,1), &retval );

	return retval;
}