// Picker.cpp
//
#include "Picker.h"

#include "TextureDescription.h"
#include "Texture.h"
#include "ClearState.h"
#include "RenderTarget.h"
#include "Context.h"
#include "PickerConstants.h"
#include "RenderCommands.h"

#include <cassert>

using namespace s2::RenderCore;

// ------------------------------------------------------------------------------------------------
const Math::ivec4 gClearColor( PickerConstants::kClearValue );

// ------------------------------------------------------------------------------------------------
void Picker::reset()
{
	detach();
	_pickTexture = nullptr;
}

// ------------------------------------------------------------------------------------------------
// Attach the picker to a render target and specify the attachment point to be used for picking.
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
// Clear the pick attachment to the clear value.
// This is useful to reset the pick buffer before rendering a new frame.
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

	// Use commands from current context
	Context* ctx = Context::current();
	if( !ctx )
		return false;

	ctx->commands().clear( *_target, cs );
	return true;
}

// ------------------------------------------------------------------------------------------------
// Pick the value at the specified coordinates. 
// The value is read from the pick attachment of the render target.
// If the picker is not attached to a render target or if the coordinates are out of bounds,
// the clear value is returned.
PickerConstants::Value Picker::pickValueAt( int32_t x, int32_t y ) const
{
	// the picker must be attached to a render target
	assert( _target );

	if( !_target || x < 0 || x >= int32_t( _target->width() ) || y < 0 ||  y >= int32_t( _target->height() ) )
		return PickerConstants::kClearValue; // out of bounds or not attached to a render target, return clear value

	// Use commands from current context
	Context* ctx = Context::current();
	if( !ctx )
		return PickerConstants::kClearValue;

	PickerConstants::Value pickedPixel;
	ctx->commands().readPixels( *_target, _attachmentPoint, ImageFormat::RedInteger, Math::irect(x,y,1,1), &pickedPixel );

	return pickedPixel;
}