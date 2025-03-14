// Surface.h
//
#ifndef SURFACE_H
#define SURFACE_H

#include "RenderCore_API.h"

#include "VertexArray.h"
#include "PrimitiveType.h"
#include "PrimitiveBuffer.h"
#include "PrimitiveBatch.h"
#include "Texture.h"
#include "SurfacePicker.h"
#include "ClearState.h"
#include "DrawState.h"


#include "Core/Pixmap.h"
#include "Math/Rectangle.h"

#include <initializer_list>

namespace RenderCore {

// CANNOT BE SHARED BETWEEN CONTEXTS
// You'd need a context object to create a Surface from
class RENDERCORE_API Surface
{
public:
	struct AttachmentDescription
	{
		FrameBuffer::AttachmentPoint	attachPoint;
		TextureFormat					textureFormat;

		AttachmentDescription( const FrameBuffer::AttachmentPoint ap, const TextureFormat tf ) 
			: attachPoint( ap ), textureFormat( tf ) {}
	};

public:
	Surface();
	Surface( const std::initializer_list<AttachmentDescription>& init );

	void setLabel( const std::string &label );

	void clear( const ClearState& cs = {} ) const;
	void draw( const PrimitiveType& primitiveType, const VertexArrayPtr& va, const DrawState& ds = {} ) const;
	void draw( const PrimitiveType& primitiveType, const PrimitiveBufferPtr& p, const DrawState& ds = {} ) const ;
	void draw( const PrimitiveBatch& batch, const DrawState& ds = {} ) const;

	uint32_t width()  const { return _width;  }
	uint32_t height() const { return _height; }

	void resize( int32_t width, int32_t height );

	void              attach( const FrameBuffer::AttachmentPoint &attachPoint, const Texture2DPtr &texture );
	void              createAttachment( const FrameBuffer::AttachmentPoint &attachPoint, const TextureFormat &textureFormat );
	void              removeAttachment( const FrameBuffer::AttachmentPoint &attachPoint );
	uint32_t          colorAttachmentDrawBufferIndex( const FrameBuffer::AttachmentPoint &attachPoint ) const;

	Texture2DPtr      attachment( const FrameBuffer::AttachmentPoint &a ) const;

	bool	          setPickingEnabled( const bool enable );
	bool	          isPickingEnabled() const;
	bool              clearPickBuffer();
	PickerConstants::Value     pickValueAt( int32_t x, int32_t y );

	void swap( uint32_t targetFBO, const FrameBuffer::AttachmentPoint &att ) const;
	
	// convenience method. same as swap( Context::current()->dafaultFBO(), FrameBuffer::ColorAttachment0 );
	void blit() const;
	
	// todo: grab image instead of swap?
	Pixmap<uint8_t> grabImage() const;

	void readPixels( const FrameBuffer::AttachmentPoint attachPoint, const ImageFormat pixelFormat, 
					 const Math::irect &roi, float *pixels ) const; // This is a convenience function
	void readPixels( const FrameBuffer::AttachmentPoint attachPoint, const ImageFormat pixelFormat, 
					 const Math::irect &roi, uint8_t *pixels ) const; // This is a convenience function
	void readPixels( const FrameBuffer::AttachmentPoint attachPoint, const ImageFormat pixelFormat, 
					 const ImageDataType pixelType, const Math::irect &roi, void *pixels );

	const FrameBufferPtr &fbo() const { return _fbo; }  // WARNING: to be used under special conditions only or for debugging purpose. 
														// It is preferable to use Surface methods to manipulate the underlying fbo
	
	const std::weak_ptr<SurfacePicker> picker() const { return _picker; }// WARNING: to be used under special conditions only or for debugging purpose. 

private:
	std::string    genLabelAttachment( const FrameBuffer::AttachmentPoint attachPoint ) const;

private:
	uint32_t    					_width = 0;
	uint32_t    					_height = 0;

	FrameBufferPtr					_fbo;
	std::shared_ptr<SurfacePicker>	_picker;

	FrameBuffer::AttachmentPoint	kPickerAttachmentPoint = FrameBuffer::AttachmentPoint::ColorAttachment7;

	friend class Context;
};

}

#endif // !SURFACE_H
