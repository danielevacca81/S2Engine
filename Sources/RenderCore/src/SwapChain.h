// SwapChain.h
// 
#ifndef RENDERCORE_SWAPCHAIN_H
#define RENDERCORE_SWAPCHAIN_H

#include "RenderCore_API.h"

namespace RenderCore  {

class RenderTarget;

class RENDERCORE_API SwapChain
{
public:
	SwapChain() = delete;

	static void swapToScreen( const RenderTarget & );
	// ...
	// @todo: backbuffer, frontbuffer, third buffer, etc.
	
};

}

#endif