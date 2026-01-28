// Picker.h
//
#ifndef PICKER_H
#define PICKER_H

#include "RenderCore_API.h"

#include "PickerConstants.h"
#include "FrameBuffer.h"
#include "Texture.h"


namespace RenderCore {

class RenderTarget;

class Picker 
{
	static constexpr FrameBuffer::AttachmentPoint kDefaultAttachmentPoint = FrameBuffer::AttachmentPoint::ColorAttachment9;

public:
	Picker() = default;
	explicit Picker( RenderTarget* target, const FrameBuffer::AttachmentPoint& pickAttachmentPoint = kDefaultAttachmentPoint ) { attachTo( target, pickAttachmentPoint ); }
	~Picker() { reset(); }

	void      attachTo( RenderTarget *target, const FrameBuffer::AttachmentPoint &pickAttachmentPoint = kDefaultAttachmentPoint );
	void      detach();
	bool	  clear();
	
	//RenderTarget*				 renderTarget()    const { return _target; }
	FrameBuffer::AttachmentPoint attachmentPoint() const { return _attachmentPoint; }
	//bool                         isAttached()      const { return _surface != nullptr; }
	PickerConstants::Value       pickValueAt( int32_t x, int32_t y ) const; 

private:
	void      reset();

private:
	RenderTarget*				 _target { nullptr };
	FrameBuffer::AttachmentPoint _attachmentPoint { kDefaultAttachmentPoint };
	Texture2DPtr	             _pickTexture; 
};

}
#endif