// SwapChain.cpp
// 
#include "SwapChain.h"

#include "RenderTarget.h"
#include "Renderer.h"

#include <chrono>
#include <iostream>

using namespace RenderCore;


// ------------------------------------------------------------------------------------------------
void SwapChain::swapToScreen( const RenderTarget& renderTarget )
{
	// Renderer::blit is slightly faster than Renderer::drawFullscreenQuad
	Renderer::blit( renderTarget.fbo(),
					nullptr,                                                         // default FBO, which is the screen
					Math::irect( 0, 0, renderTarget.width(), renderTarget.height() ) // whole size
	);
}