// FrameBuffer.h
//
#ifndef FRAMEBUFFER_ONCE
#define FRAMEBUFFER_ONCE

#include "RenderSystem_API.h"

#include "OpenGLObject.h"

#include "Texture.h"

#include "Math/Rectangle.h"

#include <string>
#include <vector>
#include <memory>
#include <cstdint>

namespace RenderSystem {

class Context;

/************************************************************************************************/
/*                                       FrameBuffer                                            */
/************************************************************************************************/
class FrameBuffer;
typedef std::shared_ptr<FrameBuffer> FrameBufferPtr;

class RENDERSYSTEM_API FrameBuffer : public OpenGLObject
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

	//std::shared_ptr<Context> context() const { return _context.lock(); }

	void create()  override;
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

	void readPixels( const AttachmentPoint attachPoint, const ImageFormat pixelFormat, const Math::irect &roi, float   *pixels ) const  { readPixels( attachPoint, pixelFormat, ImageDataType::Float, roi, pixels ); }        // This is a convenience function
	void readPixels( const AttachmentPoint attachPoint, const ImageFormat pixelFormat, const Math::irect &roi, uint8_t *pixels ) const  { readPixels( attachPoint, pixelFormat, ImageDataType::UnsignedByte, roi, pixels ); } // This is a convenience function
	void readPixels( const AttachmentPoint attachPoint, const ImageFormat pixelFormat, 
					 const ImageDataType pixelType, const Math::irect &roi, void *pixels ) const;

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

	//std::weak_ptr<Context>               _context;

	int          _colorAttachmentCount;
	Texture2DPtr _depthAttachment;
	Texture2DPtr _depthStencilAttachment;
	
	mutable std::vector<ColorAttachment> _colorAttachments;
	mutable Changes                      _changes;
};

}
#endif