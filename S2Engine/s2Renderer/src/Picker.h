// Picker.h
//
#ifndef PICKER_ONCE
#define PICKER_ONCE

#include "s2Renderer_API.h"

#include "FrameBuffer.h"
#include "Texture.h"

#include "Core/Color.h"
#include "Core/Math.h"

#include <memory>
#include <cstdint>
#include <limits>

namespace Renderer {

class Surface;

class Picker;
typedef std::shared_ptr<Picker> PickerPtr;

class S2RENDERER_API Picker 
{
public:
	using Value = int32_t;

	static constexpr Value		kMaxValue { std::numeric_limits<Value>::max() };
	static constexpr Value		kInvalidValueBegin { (kMaxValue / 4) * 3 };
	static constexpr Value      kClearValue { kMaxValue };
	static const Math::ivec4    kClearColor; 

public:
	Picker();
	virtual ~Picker() { reset(); }

	Surface*					 surface()         const { return _surface; }
	FrameBuffer::AttachmentPoint attachmentPoint() const { return _attachmentPoint; }
	bool                         isAttached()      const { return _surface != nullptr; }

private:
	friend Surface;

	void      reset();
	void      attachToSurface( Surface *surface, const FrameBuffer::AttachmentPoint pickAttachmentPoint );
	void      detachFromSurface();

	bool	  clearPickBuffer();
	Value     getValueAt( int x, int y ) const; 

private:
	Surface*					    _surface;
	FrameBuffer::AttachmentPoint	_attachmentPoint;
	Texture2DPtr	                _pickTexture; 
};

}
#endif