// FrameBuffer.h
//
#ifndef FRAMEBUFFER_ONCE
#define FRAMEBUFFER_ONCE

#include "s2OpenGL_API.h"

#include "DrawState.h"
#include "ClearState.h"
#include "ViewState.h"
#include "StateManager.h"
#include "Texture.h"
#include "RenderBuffer.h"
#include "VertexArray.h"
#include "Mesh.h"

#include <string>
#include <vector>
#include <memory>
#include <map>

namespace s2 {
namespace OpenGL {

class FrameBuffer;
typedef std::shared_ptr<FrameBuffer> FrameBufferPtr;

class S2OPENGL_API FrameBuffer : public std::enable_shared_from_this<FrameBuffer>
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
		DepthAttatchment,
		StencilAttatchment
	};

public:
	static FrameBufferPtr getDefault();
	static FrameBufferPtr New( bool default = true );

private:
	static FrameBufferPtr Current;
	static std::map<void*,FrameBufferPtr> Default; // map from context to framebuffer


public:
	FrameBuffer( bool default = true );
	~FrameBuffer();

	void setViewport( const Math::Rectangle &viewport );
	Math::Rectangle viewport() const;


	void clear( const ClearState &cs = ClearState() );
	void draw( const Primitive &primitive, const VertexArray &va, const DrawState &ds = DrawState() );
	void draw( const Primitive &primitive, const Mesh &mesh, const DrawState &ds = DrawState() );
	//void drawQuad( DrawState ); // @todo

    //void setReadBuffer(BufferId b);	  // @todo
    //void setDrawBuffer(BufferId b);	  // @todo
    //void setDrawBuffers(BufferId b);	  // @todo
	//void readPixels( int x, int y, int w, int h, const Texture::DataFormat &f, PixelType t, const Buffer::Parameters &s, const Buffer &dstBuf, bool clamp)


	void attachTextureTarget( const AttachmentPoint &attachPoint, const TexturePtr &texture );
	void attachRenderTarget( const AttachmentPoint &attachPoint, const RenderBufferPtr &renderBuffer /* stencil or depth*/ );


	std::string info()        const;

private:
	void set();
	void setParameters() const;
	void setBuffers()    const;
	void setAttachments() const;
	bool checkAttatchments() const;


private:
	struct Attachment
	{
		AttachmentPoint attachmentName; // GL_COLOR_ATTACHMENT0, ...
		unsigned int    attachmentType; // GL_TEXTURE_1D, GL_TEXTURE_2D, ...
		unsigned int    attachmentID;	// textureID or renderbufferID
		bool            isRenderBuffer; // if true attachmentType will be (GL_RENDERBUFFER)
	};

private:
	unsigned int                 _fboID;
	Math::Rectangle              _viewport;
	StateManager                 _stateManager;
	
	mutable bool                 _attachmentsChanged;
	mutable bool                 _parametersChanged;
	mutable bool                 _buffersChanged;

	std::vector<AttachmentPoint> _drawBuffers; //tbd
	AttachmentPoint              _readBuffer;  //tbd
	std::vector<Attachment>      _attachments;
};

}}
#endif