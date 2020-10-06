// FrameBuffer.h
//
#ifndef FRAMEBUFFER_ONCE
#define FRAMEBUFFER_ONCE

#include "s2Renderer_API.h"

#include "OpenGLObject.h"

#include "Texture.h"

#include "Core/Rectangle.h"

#include <string>
#include <vector>
#include <memory>
#include <cstdint>

namespace Renderer {

/************************************************************************************************/
/*                                       FrameBuffer                                            */
/************************************************************************************************/
class FrameBuffer;
typedef std::shared_ptr<FrameBuffer> FrameBufferPtr;

class S2RENDERER_API FrameBuffer : public OpenGLObject
{
public:
	enum AttachmentPoint
	{
		ColorAttachment0 = 0,
		ColorAttachment1,
		ColorAttachment2,
		ColorAttachment3,
		ColorAttachment4,
		ColorAttachment5,
		ColorAttachment6,
		ColorAttachment7,
		ColorAttachment8,
		ColorAttachment9,
		DepthAttachment,
		DepthStencilAttachment
	};

public:
	// CANNOT BE SHARED BETWEEN CONTEXTS
	static FrameBufferPtr New();

public:
	//OBJECT_DISABLE_COPY( FrameBuffer )
	//OBJECT_DECLARE_MOVEABLE( FrameBuffer )

	FrameBuffer();
	~FrameBuffer();

	bool create()  override;
	void destroy() override;
	void bind()    const override;
	void unbind()  const override;

	//void bind();
	//static void unbind();
	
	void attach( const AttachmentPoint &attachPoint, const Texture2DPtr &texture );

	int  colorAttachmentCount() const;
	bool hasDepthAttachment() const;
	bool hasDepthStencilAttachment() const;

	Texture2DPtr attachment( const AttachmentPoint &a ) const;
	int			 colorAttachmentDrawBufferIndex( const AttachmentPoint a ) const;	

	void readPixels( const AttachmentPoint attachPoint, const ImageFormat pixelFormat, const Math::Rectangle &roi, float   *pixels ) const  { readPixels( attachPoint, pixelFormat, ImageDataType::Float, roi, pixels ); }        // This is a convenience function
	void readPixels( const AttachmentPoint attachPoint, const ImageFormat pixelFormat, const Math::Rectangle &roi, uint8_t *pixels ) const  { readPixels( attachPoint, pixelFormat, ImageDataType::UnsignedByte, roi, pixels ); } // This is a convenience function
	void readPixels( const AttachmentPoint attachPoint, const ImageFormat pixelFormat, 
					 const ImageDataType pixelType, const Math::Rectangle &roi, void *pixels ) const;

	std::string info()        const;
private:
	void reset() override;
	int  objectLabelIdentifier() const override;

private:
	enum Changes
	{
		None         = 0,
		Color        = 1,
		Depth        = 2,
		DepthStencil = 4
	};

	struct ColorAttachment
	{
		Texture2DPtr texture;
		bool         changed;

		ColorAttachment()
		: changed( false )
		{}
	};

private:
	static constexpr int kMaxColorAttachment = ColorAttachment9+1;

	int          _colorAttachmentCount;
	Texture2DPtr _depthAttachment;
	Texture2DPtr _depthStencilAttachment;
	
	mutable std::vector<ColorAttachment> _colorAttachments;
	mutable Changes                      _changes;
};

}
#endif