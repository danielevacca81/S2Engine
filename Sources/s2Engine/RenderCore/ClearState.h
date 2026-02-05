// ClearState.h
// 
#ifndef S2_RENDERCORE_CLEARSTATE_H
#define S2_RENDERCORE_CLEARSTATE_H

#include "RenderState.h"

#include "Graphics/Color.h"

#include <array>
#include <utility>
#include <variant>

namespace s2 {
namespace RenderCore {

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
	int32_t             stencil{0};
	ClearColorSeparate  colorSeparate;
};

} // namespace RenderCore
} // namespace s2
#endif // !S2_RENDERCORE_CLEARSTATE_H