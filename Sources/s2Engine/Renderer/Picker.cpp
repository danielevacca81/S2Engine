// Picker.cpp
//
#include "Picker.h"

using namespace s2::Renderer;
using namespace s2::RenderCore;

// ------------------------------------------------------------------------------------------------
Picker::Picker( Renderer& renderer )
{
    // Connect to Renderer::onFrameDone.
    // The slot fires synchronously inside endFrame() while the GL context is current.
    renderer.onFrameDone.connect( &Picker::onFrameDone, this );
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
	// it will be invoked in onFrameDone when a pick result is resolved.
	_hitCallback = callback;
}

// ------------------------------------------------------------------------------------------------
void Picker::onFrameDone( const FrameData& frameData )
{
    if( !_enabled || !_hasPendingRequest )
        return;

    // Retrieve the pick RenderTarget published by PickPass
    auto it = frameData.passData.find( PickPass::kPickTargetKey );
    if( it == frameData.passData.end() || !it->second )
        return; // PickPass not in the pipeline

    auto* pickTarget = static_cast<RenderTarget*>( it->second );

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
