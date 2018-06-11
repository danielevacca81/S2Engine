// RenderBuffer.h
//
#ifndef RENDERBUFFER_ONCE
#define RENDERBUFFER_ONCE

#include "s2OpenGL_API.h"

#include <memory>

namespace s2 {
namespace OpenGL {

class RenderBuffer;
typedef std::shared_ptr<RenderBuffer> RenderBufferPtr;

class S2OPENGL_API RenderBuffer
{
public:
	enum Format
	{
		R8,
		R8_SNORM,
		R16,
		R16_SNORM,
		RG8,
		RG8_SNORM,
		RG16,
		RG16_SNORM,
		R3_G3_B2,
		RGB4,
		RGB5,
		RGB8,
		RGB8_SNORM,
		RGB10,
		RGB12,
		RGB16,
		RGB16_SNORM,
		RGBA2,
		RGBA4,
		RGB5_A1,
		RGBA8,
		RGBA8_SNORM,
		RGB10_A2,
		RGB10_A2UI,
		RGBA12,
		RGBA16,
		RGBA16_SNORM,
		SRGB8,
		SRGB8_ALPHA8,
		R16F,
		RG16F,
		RGB16F,
		RGBA16F,
		R32F,
		RG32F,
		RGB32F,
		RGBA32F,
		R11F_G11F_B10F,
		RGB9_E5,
		R8I,
		R8UI,
		R16I,
		R16UI,
		R32I,
		R32UI,
		RG8I,
		RG8UI,
		RG16I,
		RG16UI,
		RG32I,
		RG32UI,
		RGB8I,
		RGB8UI,
		RGB16I,
		RGB16UI,
		RGB32I,
		RGB32UI,
		RGBA8I,
		RGBA8UI,
		RGBA16I,
		RGBA16UI,
		RGBA32I,
		RGBA32UI,
		DepthComponent,
		DepthComponent16,
		DepthComponent24,
		DepthComponent32,
		DepthComponent32F,
		Depth24Stencil8,
		Depth32FStencil8,
		StencilIndex,
		StencilIndex1,
		StencilIndex4,
		StencilIndex8,
		StencilIndex16
	};

public:
	RenderBuffer( const Format &format, int width, int height, int samples = 0 );
	virtual ~RenderBuffer();

	unsigned int id() const;

private:
	unsigned int _bufferID;
};


}}

#endif
