// SwapChain.h
// 
#ifndef S2_RENDERCORE_SWAPCHAIN_H
#define S2_RENDERCORE_SWAPCHAIN_H

#include "s2Engine_API.h"

namespace s2 {
namespace RenderCore  {

class RenderTarget;

class S2ENGINE_API SwapChain
{
public:
	SwapChain() = delete;

	static void swapToScreen( const RenderTarget & );
	// ...
	// @todo: backbuffer, frontbuffer, third buffer, etc.
	
};

} // namespace RenderCore
} // namespace s2
#endif // !S2_RENDERCORE_SWAPCHAIN_H