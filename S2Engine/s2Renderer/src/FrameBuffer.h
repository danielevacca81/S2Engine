// FrameBuffer.h
//
#ifndef FRAMEBUFFER_ONCE
#define FRAMEBUFFER_ONCE

#include "s2Renderer_API.h"

#include "OpenGLObject.h"

#include "Texture.h"

#include <string>
#include <vector>
#include <memory>

namespace s2 {
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
	static FrameBufferPtr makeNew();

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

	int colorAttachmentCount() const;
	bool hasDepthAttachment() const;
	bool hasDepthStencilAttachment() const;

	Texture2DPtr attachment( const AttachmentPoint &a ) const;

	std::string info()        const;
private:
	void reset() override;

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
	int          _colorAttachmentCount;
	Texture2DPtr _depthAttachment;
	Texture2DPtr _depthStencilAttachment;
	
	mutable std::vector<ColorAttachment> _colorAttachments;
	mutable Changes                      _changes;
};

}}
#endif