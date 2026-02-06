// SwapChain.cpp
// 
#include "SwapChain.h"

#include "RenderTarget.h"
#include "RenderBackend.h"

using namespace s2::RenderCore;


// ------------------------------------------------------------------------------------------------
void SwapChain::swapToScreen( const RenderTarget& renderTarget )
{
	// RenderBackend::blit is slightly faster than RenderBackend::drawFullscreenQuad
	RenderBackend::blit( renderTarget.fbo(),
						 nullptr,                                                         // default FBO, which is the screen
						 Math::irect( 0, 0, renderTarget.width(), renderTarget.height() ) // whole size
	);
}