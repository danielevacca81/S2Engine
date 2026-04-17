// Picker.cpp
//
#include "Picker.h"

using namespace s2::Renderer;
using namespace s2::RenderCore;

// ------------------------------------------------------------------------------------------------
Picker::Picker( Renderer& renderer )
{
    // Connect to Renderer::onRenderCompleted.
    // The slot fires synchronously inside endFrame() while the GL context is current.
    renderer.onRenderCompleted.connect( &Picker::onRenderCompleted, this );
}

// ------------------------------------------------------------------------------------------------
void Picker::pickObjectAt( const Math::ivec2& screenPos )
{
    if( !_enabled )
        return;

    _pendingScreenPos  = screenPos;
    _hasPendingRequest = true;
}

// ------------------------------------------------------------------------------------------------
void Picker::onObjectHit( const std::function<void( const PickResult& result )>& callback )
{
	// simple store callback here.
	// it will be invoked in onRenderCompleted when a pick result is resolved.
	_hitCallback = callback;
}

// ------------------------------------------------------------------------------------------------
void Picker::onRenderCompleted( const FrameData& frameData )
{
    if( !_enabled || !_hasPendingRequest )
        return;

	// 1. Check if PickPass is in the pipeline
	auto found = frameData.renderPasses.findPass( "s2Engine.PickPass" );
	if( !found )
		return;

	// 2. Get the pick target from FrameData::passData
	auto it = frameData.passData.find( PickPass::kPickTargetKey );
    if( it == frameData.passData.end() )
		return; // PickPass is present but did not publish the pick target (e.g., disabled or failed), cannot proceed.

	auto pickTarget = std::any_cast<RenderTarget*>( it->second );

    const Math::ivec2 screenPos = _pendingScreenPos;
    _hasPendingRequest = false;

    // Bounds check
    if( screenPos.x < 0 || screenPos.x >= static_cast<int32_t>( pickTarget->width()  ) ||
        screenPos.y < 0 || screenPos.y >= static_cast<int32_t>( pickTarget->height() ) )
    {
        _hitCallback( PickResult{ 0, 0, screenPos } );
        return;
    }

    // GL context is guaranteed current here (we are inside Renderer::endFrame).
    // Read the single pixel at the requested screen position.
    Math::ivec2 pixel( 0 );
    pickTarget->framebuffer()->readPixels(
        FrameBuffer::AttachmentPoint::ColorAttachment0,
        ImageFormat::RedGreenInteger,
        ImageDataType::Int,
        Math::irect( screenPos.x, screenPos.y, 1, 1 ),
        &pixel
    );

    PickResult result;
    result.objectID    = static_cast<uint32_t>( pixel.r );
    result.primitiveID = static_cast<uint32_t>( pixel.g );
    result.screenPos   = screenPos;

    _hitCallback( result );
}
