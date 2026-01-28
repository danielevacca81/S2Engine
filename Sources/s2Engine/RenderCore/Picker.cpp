// Picker.cpp
//
#include "Picker.h"

#include "TextureDescription.h"
#include "Texture.h"
#include "ClearState.h"
#include "RenderTarget.h"

#include <cassert>

using namespace RenderCore;

// ------------------------------------------------------------------------------------------------
const Math::ivec4 gClearColor( PickerConstants::kClearValue );

// ------------------------------------------------------------------------------------------------
void Picker::reset()
{
	detach();
	_pickTexture = nullptr;
}

// ------------------------------------------------------------------------------------------------
void Picker::attachTo( RenderTarget *target, const FrameBuffer::AttachmentPoint &pickAttachmentPoint )
{	
	if( target == _target && ( !_target || pickAttachmentPoint == _attachmentPoint ) )
		return;

	detach();

	_target = target;
	_attachmentPoint = pickAttachmentPoint;

	if( !_target )
		return;

	if( !_pickTexture )
		_pickTexture = Texture2D::New( TextureDescription( _target->width(), _target->height(),TextureFormat::Red32i ) );

	_target->attach( _attachmentPoint, _pickTexture );
}

// ------------------------------------------------------------------------------------------------
void Picker::detach()
{	
	if( !_target )
		return;

	_target->removeAttachment( _attachmentPoint );
	_target = nullptr;
}

// ------------------------------------------------------------------------------------------------
/**
	the picker must be attached to a surface (PickerConstants::attachToSurface) for this call to succeed
*/
bool Picker::clear()
{
	if( !_target )
		return false;

	const auto idx = _target->colorAttachmentDrawBufferIndex( _attachmentPoint );
	if( idx == -1 )
		return false;

	ClearState cs;
	cs.buffers = ClearBuffers::ColorBuffer;
	cs.colorSeparate.enabled = true;
	cs.colorSeparate.color[idx] = gClearColor;

	_target->clear( cs );
	return true;
}

// ------------------------------------------------------------------------------------------------
/**
	the picker must be attached to a surface (PickerConstants::attachToSurface) for this call to succeed
*/
PickerConstants::Value Picker::pickValueAt( int32_t x, int32_t y ) const
{
	// alternativa, usa:  class S2ENGINE_API ReadPixelBuffer (?)

	assert( _target );
	if( !_target || x < 0 || x >= _target->width() || y < 0 ||  y >= _target->height() )
		return PickerConstants::kClearValue; // out of bounds

	PickerConstants::Value pickedPixel;
	_target->readPixels( _attachmentPoint, ImageFormat::RedInteger, ImageDataType::Int, Math::irect(x,y,1,1), &pickedPixel );

	return pickedPixel;
}