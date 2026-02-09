// RenderTarget.h
//
#ifndef S2_RENDERCORE_RENDERTARGET_H
#define S2_RENDERCORE_RENDERTARGET_H

#include "s2Engine_API.h"

#include "FrameBuffer.h"
#include "Texture.h"

#include <initializer_list>
#include <string>

namespace s2 {
namespace RenderCore {

// ------------------------------------------------------------------------------------------------
// RenderTarget: Manages framebuffer attachments and dimensions
// Does NOT perform rendering operations (delegated to CommandBuffer)
// ------------------------------------------------------------------------------------------------
class S2ENGINE_API RenderTarget
{
public:
	struct AttachmentDescription
	{
		FrameBuffer::AttachmentPoint attachPoint;
		TextureFormat                textureFormat;

		AttachmentDescription( const FrameBuffer::AttachmentPoint ap, const TextureFormat tf )
			: attachPoint( ap ), textureFormat( tf ) {}
	};

public:
	RenderTarget();
	explicit RenderTarget( const std::initializer_list<AttachmentDescription>& init );

	// Label for debugging
	void setLabel( const std::string& label );

	// Dimensions
	uint32_t width()  const { return _width; }
	uint32_t height() const { return _height; }
	void resize( int32_t width, int32_t height );

	// Attachment management
	void attach( const FrameBuffer::AttachmentPoint& attachPoint, const Texture2DPtr& texture );
	void createAttachment( const FrameBuffer::AttachmentPoint& attachPoint, const TextureFormat& textureFormat );
	void removeAttachment( const FrameBuffer::AttachmentPoint& attachPoint );

	Texture2DPtr attachment( const FrameBuffer::AttachmentPoint& a ) const;
	uint32_t     colorAttachmentDrawBufferIndex( const FrameBuffer::AttachmentPoint& attachPoint ) const;

	// Direct FBO access (use with caution)
	const FrameBufferPtr& fbo() const { return _fbo; }

private:
	std::string genLabelAttachment( const FrameBuffer::AttachmentPoint& attachPoint ) const;

private:
	uint32_t       _width  { 0 };
	uint32_t       _height { 0 };
	FrameBufferPtr _fbo;
};

} // namespace RenderCore
} // namespace s2

#endif // !S2_RENDERCORE_RENDERTARGET_H
