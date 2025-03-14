// SurfacePicker.h
//
#ifndef SURFACEPICKERPICKER_H
#define SURFACEPICKERPICKER_H

#include "RenderCore_API.h"

#include "PickerConstants.h"
#include "FrameBuffer.h"
#include "Texture.h"

#include "Math/Math.h"

#include <cstdint>
#include <limits>

namespace RenderCore {

class Surface;

class SurfacePicker 
{
public:
	SurfacePicker();
	virtual ~SurfacePicker() { reset(); }

	Surface*					 surface()         const { return _surface; }
	FrameBuffer::AttachmentPoint attachmentPoint() const { return _attachmentPoint; }
	bool                         isAttached()      const { return _surface != nullptr; }

private:
	void      reset();
	void      attachToSurface( Surface *surface, const FrameBuffer::AttachmentPoint &pickAttachmentPoint );
	void      detachFromSurface();

	bool	  clearPickBuffer();
	PickerConstants::Value getValueAt( int x, int y ) const; 

private:
	Surface*					    _surface;
	FrameBuffer::AttachmentPoint	_attachmentPoint;
	Texture2DPtr	                _pickTexture; 

	friend class Surface;
};

}
#endif