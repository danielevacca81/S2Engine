// Surface.h
//
#ifndef SURFACE_ONCE
#define SURFACE_ONCE

#include "s2Renderer_API.h"

#include "StateManager.h"
#include "PrimitiveType.h"
#include "VertexArray.h"
#include "PrimitiveBuffer.h"
#include "Texture.h"
#include "Picker.h"

#include "Core/ImageBuffer.h"

#include <initializer_list>

namespace Renderer {


class Surface;
typedef std::shared_ptr<Surface> SurfacePtr;

class S2RENDERER_API Surface
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
	// CANNOT BE SHARED BETWEEN CONTEXTS
	static SurfacePtr New();

public:
	Surface();
	Surface( std::initializer_list<AttachmentDescription> init );
	~Surface();

	void setLabel( const std::string &label );

	void clear( const ClearState &cs );
	void draw( const PrimitiveType &primitiveType, const VertexArrayPtr &va, const DrawingState &ds );
	void draw( const PrimitiveType &primitiveType, const PrimitiveBufferPtr &p, const DrawingState &ds );
	// @todo_dv:add draw batch

	int width()  const { return _width;  }
	int height() const { return _height; }

	void resize( int width, int height );

	void              attach( const FrameBuffer::AttachmentPoint attachPoint, const Texture2DPtr &texture );
	void              createAttachment( const FrameBuffer::AttachmentPoint attachPoint, const TextureFormat textureFormat );
	void              removeAttachment( const FrameBuffer::AttachmentPoint attachPoint );
	int               colorAttachmentDrawBufferIndex( const FrameBuffer::AttachmentPoint attachPoint ) const;

	Texture2DPtr      attachment( const FrameBuffer::AttachmentPoint &a ) const;

	void              setPicker( const PickerPtr &picker, const FrameBuffer::AttachmentPoint attachPoint = FrameBuffer::AttachmentPoint::ColorAttachment1 );	      
	const PickerPtr  &picker() const { return _picker; }
	bool	          setPickingEnabled( const bool enable );
	bool              clearPickBuffer();
	Picker::Value     pickValueAt( int x, int y );


	void swap( unsigned int targetFBO, const FrameBuffer::AttachmentPoint &att = FrameBuffer::ColorAttachment0 );
	
	// todo: grab image instead of swap?
	ImageBuffer<uint8_t> grabImage() const;

	void readPixels( const FrameBuffer::AttachmentPoint attachPoint, const ImageFormat pixelFormat, 
					 const Math::Rectangle &roi, float *pixels ) const; // This is a convenience function
	void readPixels( const FrameBuffer::AttachmentPoint attachPoint, const ImageFormat pixelFormat, 
					 const Math::Rectangle &roi, uint8_t *pixels ) const; // This is a convenience function
	void readPixels( const FrameBuffer::AttachmentPoint attachPoint, const ImageFormat pixelFormat, 
					 const ImageDataType pixelType, const Math::Rectangle &roi, void *pixels );
	
	// void swap( SurfacePtr )
	//readFramebuffer( AttachmentPoint )

	// todo: enable state shadowing. consecutive draw calls can take advantage of unchanged state.
	// if qt will trash gl state. >> force state manager to set render state on clear/invalidate method

	const FrameBufferPtr &fbo() const { return _fbo; }  // WARNING: to be used in special conditions only or for debugging purpose. 
														// It is preferable to use Surface methods to manipulate the underlying fbo

private:
	std::string    genLabelAttachment( const FrameBuffer::AttachmentPoint attachPoint ) const;

private:
	int								_width;
	int								_height;

	FrameBufferPtr					_fbo;
	StateManager					_state;

	PickerPtr						_picker;
	FrameBuffer::AttachmentPoint	_pickerAttachmentPoint;
};

}

#endif // !SURFACE_ONCE
