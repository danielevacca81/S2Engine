// SwapChain.cpp
// 
#include "SwapChain.h"

#include "RenderTarget.h"
#include "Context.h"
#include "RendererBackend.h"

using namespace s2::RenderCore;

// ------------------------------------------------------------------------------------------------
void SwapChain::swapToScreen( const RenderTarget& renderTarget )
{
	// Get current context and use its command buffer
	Context* ctx = Context::current();
	if( !ctx )
		return; // No active context

	// Blit is slightly faster than drawFullscreenQuad
	ctx->rendererBackend().blit(renderTarget.framebuffer(),
								nullptr,													   // default FBO, which is the screen
								Math::irect(0, 0, renderTarget.width(), renderTarget.height()) // whole size
	);
}