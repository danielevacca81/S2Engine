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

class S2OPENGL_API FrameBuffer
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
		DepthComponent32F,
	};

public:
	static FrameBufferPtr getDefault();
	static FrameBufferPtr New();

public:
	FrameBuffer();
	~FrameBuffer();

	bool create();
	void bind( int width, int height );

	void unBind();
	void clear(float r = 0.f, float g = 0.f, float b = 0.f, float a = 1.f);
	bool attachTextureTarget( const TexturePtr &texture, const AttachmentPoint &attachPoint );
	bool detachTextureTarget( const AttachmentPoint &attachPoint );
	bool attachDepthRenderTarget( const DepthFormat &depthFormat, int width, int height );
	//bool attachStencilRenderTarget(); //@TODO

	bool   isBound()     const;
	bool   isValid()     const;

	std::string info()        const;
	bool        checkStatus() const;

private:
	void destroy();

private:
	struct Attachment
	{
		AttachmentPoint       attachmentName; // GL_COLOR_ATTACHMENT0, ...
		Texture::TextureType  attachmentType; // GL_TEXTURE_1D, GL_TEXTURE_2D, ...
		unsigned int          attachmentID;	  // textureID
	};

private:
	int          _width;
	int          _height;
	bool         _bound;
	unsigned int _fbo;

	DepthFormat  _depthFormat;
	unsigned int _depthBuffer;

	std::vector<Attachment> _attachments;
};

}}
#endif