// ClearState.h
// 
#ifndef CLEARSTATE_ONCE
#define CLEARSTATE_ONCE

//#include "FrameBuffer.h"
#include "RenderState.h"

#include "Core/Color.h"

#include <array>
#include <utility>
#include <variant>

namespace RenderSystem {

/************************************************************************/
/*                             ClearColorSeparate                       */
/************************************************************************/
struct ClearColorSeparate
{
	using NoColor      = std::monostate;
	using VariantColor = std::variant< NoColor,			// use this type to not clean up the color attachment. This is the default value.
		                               Color,			// use this type if the color attachment to be cleaned is a standard color buffer: floating-point or fixed-point (i.e. normalized integer) buffer.
		                               Math::ivec4 >;   // use this type if the color attachment to be cleaned is signed or unsigned integer buffer (not normalized)

	bool	enabled { false };
	std::array< VariantColor, kMaxRenderTargets> color; // clear each render target with its own clear color
};

/************************************************************************/
/*                             ClearState                              */
/************************************************************************/
struct ClearState
{
	bool                shadowingEnabled {true};
				        
	ScissorTest         scissorTest;
	ColorMask           colorMask {true, true,true,true};
	DepthMask           depthMask;
	StencilMask         stencilMask {~0u,~0u};
	ClearBuffers        buffers { ClearBuffers::ColorAndDepthBuffer };
	Color               color = Color::gray();	// it will be ignored if colorSeparate.enabled == true
	float               depth {1.f};
	int                 stencil{0};
	ClearColorSeparate  colorSeparate;
};

}
#endif