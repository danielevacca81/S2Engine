// SwapChain.h
// 
#ifndef RENDERCORE_SWAPCHAIN_H
#define RENDERCORE_SWAPCHAIN_H

#include "s2Engine_API.h"

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

}

#endif