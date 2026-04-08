// RenderCommands.cpp
//
#include "RenderCommands.h"

#include "Context.h"
#include "RenderTarget.h"
#include "OpenGL.h"
#include "OpenGLCheck.h"
#include "OpenGLWrap.h"

#include <cassert>

using namespace s2::RenderCore;

#pragma region HelperFunctions
static inline void executeDrawCall( const PrimitiveType& primitive, const VertexArrayPtr& va )
{
    assert( va && va->isValid() && "VertexArray must be valid" );

    // Bind VAO (required for rendering)
    va->bind();

    const GLenum primType = glWrap( primitive );

    if( va->isIndexed() )
    {
        const auto& indexBuffer = va->indexBuffer();
      
        // Indexed draw call
        glDrawRangeElements( 
            primType,
            0,
            va->maxArrayIndex(),
            indexBuffer.count(),
            glWrap( indexBuffer.dataType() ),
            nullptr  // Indices in bound element buffer
        );
        glCheck;
    }
    else
    {
        // Non-indexed draw call
        glDrawArrays( primType, 0, va->maxArrayIndex() + 1 );
        glCheck;
    }

    va->unbind();
}

// ------------------------------------------------------------------------------------------------
static inline DrawState sanitizeDrawState( const DrawState& ds, const RenderTarget& target )
{
    DrawState out( ds );

    // Set viewport to render target size if not specified
    if( out.viewport.rect.isEmpty() )
        out.viewport.rect = Math::irect( 0, 0, target.width(), target.height() );

    return out;
}
#pragma endregion



// ------------------------------------------------------------------------------------------------
RenderCommands::RenderCommands( Context& context )
    : _context( context )
{
}

// ================================================================================================
// CLEAR OPERATIONS (DSA-ready)
// ================================================================================================

void RenderCommands::clear( const RenderTarget& target, const ClearState& cs ) const
{
    clear( target.framebuffer(), cs );
}

// ------------------------------------------------------------------------------------------------
void RenderCommands::clear( const FrameBufferPtr& fbo, const ClearState& cs ) const
{
    if( !fbo )
        return;

    // Bind FBO (required for clear operations)
    fbo->bind();
    
    // Apply clear state and perform clear
    _context._stateManager.setClearState( cs );
    
    fbo->unbind();
}

// ================================================================================================
// DRAW OPERATIONS (DSA-ready)
// ================================================================================================

void RenderCommands::draw( const RenderTarget& target, const PrimitiveType& primitiveType,const VertexArrayPtr& va, const DrawState& ds ) const
{
    draw( target.framebuffer(), primitiveType, va, sanitizeDrawState( ds, target ) );
}

// ------------------------------------------------------------------------------------------------
void RenderCommands::draw( const RenderTarget& target, const PrimitiveType& primitiveType,const VertexDataPtr& primitive, const DrawState& ds ) const
{
    draw( target.framebuffer(), primitiveType, primitive, sanitizeDrawState( ds, target ) );
}

// ------------------------------------------------------------------------------------------------
void RenderCommands::draw( const RenderTarget& target, const PrimitiveBatch& batch, const DrawState& ds ) const
{
    if( !target.framebuffer() )
        return;

    const auto& b = batch.batch();

    // Create vertex data from batch (DSA)
    auto vertexData = VertexData::New();
    vertexData->setVertices( b.vertices );
    vertexData->setTextureCoords( b.textureCoords );
    vertexData->setColors( b.colors );
    vertexData->setNormals( b.normals );
    vertexData->setIndices( b.indices );

    // Setup draw state with primitive restart
    DrawState batchDrawState = sanitizeDrawState( ds, target );
    batchDrawState.renderState.primitiveRestart.enabled = true;
    batchDrawState.renderState.primitiveRestart.index = batch.primitiveRestartIndex();

    draw( target.framebuffer(), batch.primitiveType(), vertexData, batchDrawState );
}

// ------------------------------------------------------------------------------------------------
void RenderCommands::draw( const FrameBufferPtr& fbo, const PrimitiveType& primitiveType, const VertexArrayPtr& va, const DrawState& ds ) const
{
    if( !fbo || !va )
        return;

    assert( ds.shader && "DrawState must have a valid shader" );

    // Bind FBO (required for rendering)
    fbo->bind();

    // Apply draw state (DSA-aware: no binding for uniforms)
    _context._stateManager.setDrawState( ds );

    // Execute draw call
    executeDrawCall( primitiveType, va );

    fbo->unbind();
}

// ------------------------------------------------------------------------------------------------
void RenderCommands::draw( const FrameBufferPtr& fbo, const PrimitiveType& primitiveType, const VertexDataPtr& primitive, const DrawState& ds ) const
{
    if( !fbo || !primitive )
        return;

    assert( ds.shader && "DrawState must have a valid shader" );

    // Bind FBO (required for rendering)
    fbo->bind();

    // Apply draw state (DSA-aware)
    _context._stateManager.setDrawState( ds );

    // Execute draw call using VertexData's VAO
    executeDrawCall( primitiveType, primitive->vao() );

    fbo->unbind();
}

// ================================================================================================
// READ OPERATIONS (DSA where possible)
// ================================================================================================
Pixmap<uint8_t> RenderCommands::readPixels( const RenderTarget& target ) const
{
    return readPixels( target.framebuffer(), target.width(), target.height() );
}

// ------------------------------------------------------------------------------------------------
Pixmap<uint8_t> RenderCommands::readPixels( const FrameBufferPtr& fbo, uint32_t width, uint32_t height ) const
{
    if( !fbo || width == 0 || height == 0 )
        return {};

    constexpr int rowAlignment = 4;
    constexpr ImageFormat format = ImageFormat::RedGreenBlueAlpha;
    constexpr ImageDataType dataType = ImageDataType::UnsignedByte;
    
    const int64_t sizeInBytes = computeRequiredSizeInBytes( width, height, format, dataType, rowAlignment );

    // Bind FBO (required for glReadPixels)
    fbo->bind();

    // Create PBO for async readback (DSA)
    auto pixelBuffer = GPUBufferObject::New(
        sizeInBytes,
        GPUBufferObject::Type::PixelPackBuffer,
        GPUBufferObject::UsageHint::StreamRead
    );

    // Bind PBO and read pixels
    glBindBuffer( GL_PIXEL_PACK_BUFFER, pixelBuffer->id() );
    glCheck;

    glReadBuffer( GL_COLOR_ATTACHMENT0 );
    glCheck;

    glReadPixels( 
        0, 0, 
        width, height, 
        glWrap( format ), 
        glWrap( dataType ), 
        nullptr  // Read into PBO
    );
    glCheck;

    // Map PBO to read data (DSA)
    void* mappedData = pixelBuffer->mapRange( 
        0, 
        sizeInBytes, 
        static_cast<uint32_t>( GPUBufferObject::MapAccess::Read )
    );

    // Copy to pixmap
    Pixmap<uint8_t> img( width, height, 4, static_cast<uint8_t*>( mappedData ) );

    // Unmap and cleanup
    pixelBuffer->unmap();
    
    glBindBuffer( GL_PIXEL_PACK_BUFFER, 0 );
    glCheck;

    fbo->unbind();

    return img;
}

// ------------------------------------------------------------------------------------------------
void RenderCommands::readPixels( const RenderTarget& target, 
                                 const FrameBuffer::AttachmentPoint& attachPoint, 
                                 const ImageFormat& pixelFormat,
								 const ImageDataType& pixelType,
                                 const Math::irect& roi,
                                 void* pixels ) const
{
    assert( pixels && "Pixel buffer cannot be null" );

    // Bind FBO and read pixels (DSA)
    target.framebuffer()->bind();
    target.framebuffer()->readPixels( 
		attachPoint, // which attachment to read from
		pixelFormat, // format of pixel data in output buffer (e.g., RGBA, RGB, etc.)
		pixelType, // how many bytes per channel in output buffer (e.g., unsigned byte, float, etc.)
        roi, 
		pixels // output buffer to receive pixel data (cpu-side pointer)
    );
    target.framebuffer()->unbind();
}

// ------------------------------------------------------------------------------------------------
void RenderCommands::readPixelsAsync( const RenderTarget& target,
                                      const FrameBuffer::AttachmentPoint& attachPoint,
                                      const ImageFormat& imageFormat,
                                      const ImageDataType& pixelType,
                                      const GPUBufferObjectPtr& pbo ) const
{
    assert( pbo && pbo->isValid() && "readPixelsAsync: PBO must be valid" );
    assert( pbo->type() == GPUBufferObject::Type::PixelPackBuffer && "readPixelsAsync: PBO must be of type PixelPackBuffer" );

    // Invalidate PBO contents before writing to hint the driver it can discard old data,
    // avoiding a costly CPU->GPU sync on reuse.
    pbo->invalidate();

    // Bind PBO as pack target: subsequent glReadPixels will DMA into it (non-blocking).
    glBindBuffer( GL_PIXEL_PACK_BUFFER, pbo->id() );
    glCheck;

    // With GL_PIXEL_PACK_BUFFER bound, the nullptr offset routes data into the PBO.
    target.framebuffer()->readPixels(
        attachPoint,
        imageFormat,
        pixelType,
        Math::irect( 0, 0, target.width(), target.height() ),
        nullptr   // offset into PBO
    );

    // Unbind PBO: restore default state so subsequent CPU-side readPixels are unaffected.
    glBindBuffer( GL_PIXEL_PACK_BUFFER, 0 );
    glCheck;
}

// ================================================================================================
// BLIT OPERATIONS (DSA-ready)
// ================================================================================================

void RenderCommands::blit( 
    const RenderTarget& source, 
    const RenderTarget& destination,
    const Math::irect& srcRect, 
    const Math::irect& dstRect )
{
    const Math::irect src = srcRect.isEmpty() 
        ? Math::irect( 0, 0, source.width(), source.height() ) 
        : srcRect;
        
    const Math::irect dst = dstRect.isEmpty() 
        ? Math::irect( 0, 0, destination.width(), destination.height() ) 
        : dstRect;

    // Use DSA blit (FrameBuffer::blitTo)
    source.framebuffer()->blitTo(
        destination.framebuffer(),
        src,
        dst,
        static_cast<uint32_t>( FrameBuffer::BufferBit::Color ),
        FrameBuffer::BlitFilter::Nearest
    );
}

// ------------------------------------------------------------------------------------------------
void RenderCommands::blit( 
    const FrameBufferPtr& srcFBO, 
    const FrameBufferPtr& dstFBO,
    const Math::irect& srcRect, 
    const Math::irect& dstRect )
{
    if( !srcFBO )
        return;

    const Math::irect destRect = dstRect.isEmpty() ? srcRect : dstRect;

    // Use DSA blit if both FBOs are valid
    if( dstFBO )
    {
        srcFBO->blitTo(
            dstFBO,
            srcRect,
            destRect,
            static_cast<uint32_t>( FrameBuffer::BufferBit::Color ),
            FrameBuffer::BlitFilter::Nearest
        );
    }
    else
    {
        // Blit to default framebuffer (screen) - requires legacy binding
        const uint32_t srcFBOId = srcFBO->id();

        glBindFramebuffer( GL_READ_FRAMEBUFFER, srcFBOId );
        glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );  // Default FBO
        glCheck;

        glBlitFramebuffer( 
            srcRect.left(), srcRect.bottom(), srcRect.right(), srcRect.top(),
            destRect.left(), destRect.bottom(), destRect.right(), destRect.top(),
            GL_COLOR_BUFFER_BIT, 
            GL_NEAREST 
        );
        glCheck;

        glBindFramebuffer( GL_FRAMEBUFFER, 0 );
        glCheck;
    }
}

// ------------------------------------------------------------------------------------------------
void RenderCommands::blitToScreen( const RenderTarget& source, const Math::irect& srcRect )
{
    const Math::irect src = srcRect.isEmpty() 
        ? Math::irect( 0, 0, source.width(), source.height() ) 
        : srcRect;

    blit( source.framebuffer(), nullptr, src, {} );
}

// ================================================================================================
// UTILITY OPERATIONS
// ================================================================================================

void RenderCommands::drawFullscreenQuad( const Texture2DPtr& srcTexture )
{
    if( !srcTexture )
        return;

    assert( DefaultShaders.FullscreenQuad && "DefaultShaders.FullscreenQuad must be initialized" );

    // Setup draw state
    DrawState drawState;
    drawState.shader = DefaultShaders.FullscreenQuad;
    drawState.renderState.depthTest.enabled = false;
    drawState.renderState.faceCulling.enabled = false;
    drawState.viewport.rect = Math::irect( 
        0, 0, 
        srcTexture->description().width(),
        srcTexture->description().height() 
    );

    // Set texture using bindless (DSA)
    drawState.shader->setTexture( "screenTexture", srcTexture );

    // Bind default framebuffer (screen)
    glBindFramebuffer( GL_FRAMEBUFFER, 0 );
    glCheck;

    // Apply draw state and render fullscreen quad
    _context._stateManager.setDrawState( drawState );
    
    // Draw fullscreen quad (no VAO needed - vertex shader generates positions)
    glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
    glCheck;

    glBindFramebuffer( GL_FRAMEBUFFER, 0 );
    glCheck;
}