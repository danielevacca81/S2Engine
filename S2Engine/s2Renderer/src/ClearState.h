// ClearState.h
// 
#ifndef CLEARSTATE_ONCE
#define CLEARSTATE_ONCE

#include "FrameBuffer.h"
#include "RenderState.h"

#include "Core/Color.h"

#include <vector>
#include <utility>
#include <any>

namespace Renderer {

struct ClearState
{
	ClearState()
	: scissorTest()
	, colorMask(true,true,true,true)
	, depthMask(true)
	, stencilMask(~0u, ~0u)
	, buffers(ClearBuffers::ColorAndDepthBuffer)
	, color( Color::black() )
	, depth( 1.f )
	, stencil( 0 )
	{}

	ScissorTest  scissorTest;
	ColorMask    colorMask;
	bool         depthMask;
	StencilMask  stencilMask;
	ClearBuffers buffers;
	Color        color;	// it will be ignored if colorSeparate.size() ! = 0
	float        depth;
	int          stencil;


	struct ClearColorSeparate
	{
		int			drawBufferIdx;
		std::any	color;  // it can be Color or Math::ivec4;
	};

	std::vector< ClearColorSeparate > colorSeparate;
};

}
#endif