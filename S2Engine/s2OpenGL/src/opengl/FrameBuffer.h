// FrameBuffer.h
//
#ifndef FRAMEBUFFER_ONCE
#define FRAMEBUFFER_ONCE

#include "s2OpenGL_API.h"

#include "Texture.h"

#include <string>
#include <vector>
#include <memory>

namespace s2 {
namespace OpenGL {

class FrameBuffer;
typedef std::shared_ptr<FrameBuffer> FrameBufferPtr;

class S2OPENGL_API FrameBuffer : public std::enable_shared_from_this<FrameBuffer>
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
	static FrameBufferPtr New();

public:
	FrameBuffer();
	~FrameBuffer();

	void set();

	void bind();
	static void unbind();
	
	void attach(const AttachmentPoint &attachPoint, const Texture2DPtr &texture);

	int colorAttachmentCount() const;
	bool hasDepthAttachment() const;
	bool hasDepthStencilAttachment() const;

	Texture2DPtr attachment( const AttachmentPoint &a ) const;

	std::string info()        const;

private:
	enum Changes
	{
		None   = 0,
		Color = 1,
		Depth = 2,
		DepthStencil = 4
	};

	struct ColorAttachment
	{
		Texture2DPtr texture;
		bool         changed;

		ColorAttachment()
		: changed(false)
		{}
	};

private:
	unsigned int              _fboID;
	int                       _colorAttachmentCount;
	std::vector<ColorAttachment> _colorAttachments;
	Texture2DPtr              _depthAttachment;
	Texture2DPtr              _depthStencilAttachment;
	Changes                   _changes;
};

}}
#endif