// FrameBuffer.h
//
#ifndef FRAMEBUFFER_ONCE
#define FRAMEBUFFER_ONCE

#include "S2RendererAPI.h"

#include "Texture.h"

#include <string>
#include <vector>

namespace OpenGL {

class S2RENDERER_API FrameBuffer
{
public:
	enum AttachmentPoint
	{
		ColorAttachment0,
		ColorAttachment1,
		ColorAttachment2,
		ColorAttachment3,
		ColorAttachment4,
		ColorAttachment5,
		ColorAttachment6,
		ColorAttachment7,
		ColorAttachment8,
		ColorAttachment9,
	};

	enum DepthFormat
	{
		DepthNone,
		DepthComponent16,
		DepthComponent24,
		DepthComponent32,
		DepthComponent32FF,
	};

private:
	struct Attachment
	{
		AttachmentPoint       attachmentName; // GL_COLOR_ATTACHMENT0, ...
		Texture::TextureType  attachmentType; // GL_TEXTURE_1D, GL_TEXTURE_2D, ...
		unsigned int          attachmentID;	  // textureID
	};

	int          _width;
	int          _height;
	bool         _bound;
	unsigned int _fbo;

	DepthFormat  _depthFormat;
	unsigned int _depthBuffer;

	std::vector<Attachment> _attachments;

	void destroy();

public:
	FrameBuffer();
	~FrameBuffer();

	bool create();
	void bind( int width, int height );

	void unBind();
	void clear(float r = 0.f, float g = 0.f, float b = 0.f, float a = 1.f);
	bool attachTextureTarget( const TexturePtr &texture, AttachmentPoint attachPoint );
	bool detachTextureTarget( AttachmentPoint attachPoint );
	bool attachDepthRenderTarget( DepthFormat depthFormat, int width, int height );
	//bool attachStencilRenderTarget(); //@TODO

	bool   isBound()     const;
	bool   isValid()     const;

	std::string info()        const;
	bool        checkStatus() const;
};

}
#endif