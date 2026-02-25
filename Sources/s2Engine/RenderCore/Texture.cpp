// Texture.cpp
//
#include "Texture.h"

#include "Math/Math.h"

#include "OpenGL.h"
#include "OpenGLCheck.h"
#include "OpenGLWrap.h"

#include <cassert>
#include <algorithm>

using namespace s2::RenderCore;

// ------------------------------------------------------------------------------------------------
Texture2DPtr Texture2D::New( const TextureDescription& desc, void* data )
{
    return std::make_shared<Texture2D>( desc, data );
}

// ------------------------------------------------------------------------------------------------
Texture2D::Texture2D( const TextureDescription& description, void* data )
    : _description( description )
{
    create();
    
    if( data )
        setData( data );
        
    setDefaultSampler();
}

// ------------------------------------------------------------------------------------------------
Texture2D::~Texture2D()
{
    destroy();
}

// ------------------------------------------------------------------------------------------------
void Texture2D::create()
{
    if( isCreated() )
        return;

    // DSA: glCreateTextures (OpenGL 4.5+)
    glCreateTextures( GL_TEXTURE_2D, 1, &_objectID );
    glCheck;

    if( _objectID == 0 )
    {
        assert( false && "Failed to create texture object" );
        return;
    }

    _created = true;

    // Allocate immutable storage (DSA)
    allocateStorage();

    // Set debug label (OpenGL 4.3+)
    if( !_description.name().empty() )
        setObjectLabel( _description.name() );
}

// ------------------------------------------------------------------------------------------------
void Texture2D::destroy()
{
    if( !isCreated() )
        return;

    // Make non-resident if bindless
    if( _resident )
        makeNonResident();

    glDeleteTextures( 1, &_objectID );
    glCheck;

    reset();
}

// ------------------------------------------------------------------------------------------------
void Texture2D::reset()
{
    OpenGLObject::reset();
    _bindlessHandle = 0;
    _resident = false;
}

// ------------------------------------------------------------------------------------------------
void Texture2D::allocateStorage()
{
    assert( isCreated() && "Texture must be created before allocating storage" );

    const GLenum internalFormat = glWrap( _description.textureFormat() );
    
    // Calculate mip levels
    const int maxDimension = Math::max( _description.width(), _description.height() );
    const int levels = _description.isGenerateMipmapsEnabled() 
        ? static_cast<int>( std::floor( std::log2( maxDimension ) ) ) + 1
        : 1;

    // DSA: Allocate immutable storage (OpenGL 4.5+)
    glTextureStorage2D( 
        _objectID, 
        levels,
        internalFormat,
        _description.width(),
        _description.height() 
    );
    glCheck;
}

// ------------------------------------------------------------------------------------------------
void Texture2D::setData( void* pixels )
{
    assert( isCreated() && "Texture must be created before uploading data" );

    if( !pixels )
        return;

    const GLenum format   = glWrapTextureFormatToPixelFormat( _description.textureFormat() );
    const GLenum dataType = glWrapTextureFormatToPixelType( _description.textureFormat() );

    // DSA: Upload pixel data (OpenGL 4.5+)
    glTextureSubImage2D(
        _objectID,
        0, // mip level
        0, 0, // xoffset, yoffset
        _description.width(),
        _description.height(),
        format,
        dataType,
        pixels
    );
    glCheck;

    if( _description.isGenerateMipmapsEnabled() )
        generateMipmaps();
}

// ------------------------------------------------------------------------------------------------
void Texture2D::resize( int width, int height )
{
    if( width == _description.width() && height == _description.height() )
        return;

    _description = TextureDescription( width, height, 
                                       _description.textureFormat(), 
                                       _description.isGenerateMipmapsEnabled() );

    // With immutable storage, we need to recreate the texture
    destroy();
    create();
}

// ------------------------------------------------------------------------------------------------
void Texture2D::update( 
    int xOffset, int yOffset, 
    int width, int height, 
    const ImageFormat& imgFormat,
    const ImageDataType& imgDataType,
    void* pixels )
{
    assert( isCreated() && "Texture must be created before updating" );
    assert( pixels && "Pixel data cannot be null" );

    const GLenum format   = glWrap( imgFormat );
    const GLenum dataType = glWrap( imgDataType );

    // DSA: Update partial region (OpenGL 4.5+)
    glTextureSubImage2D(
        _objectID,
        0, // mip level
        xOffset, yOffset,
        width, height,
        format,
        dataType,
        pixels
    );
    glCheck;
}

// ------------------------------------------------------------------------------------------------
void Texture2D::update( 
    int xOffset, int yOffset, 
    int width, int height, 
    const ImageFormat& imgFormat,
    const ImageDataType& imgDataType,
    const GPUBufferObjectPtr& gpuBuffer )
{
    assert( isCreated() && "Texture must be created before updating" );
    assert( gpuBuffer->id() != 0 && "GPU buffer must be valid" );

    const GLenum format   = glWrap( imgFormat );
    const GLenum dataType = glWrap( imgDataType );

    // Bind PBO for reading
    glBindBuffer( GL_PIXEL_UNPACK_BUFFER, gpuBuffer->id() );
    glCheck;

    // DSA: Update from PBO
    glTextureSubImage2D(
        _objectID,
        0, // mip level
        xOffset, yOffset,
        width, height,
        format,
        dataType,
        nullptr // offset into PBO
    );
    glCheck;

    // Unbind PBO
    glBindBuffer( GL_PIXEL_UNPACK_BUFFER, 0 );
    glCheck;
}

// ------------------------------------------------------------------------------------------------
Pixmap<uint8_t> Texture2D::readData() const
{
    assert( isCreated() && "Texture must be created before reading data" );

    const GLenum format   = glWrapTextureFormatToPixelFormat( _description.textureFormat() );
    const GLenum dataType = glWrapTextureFormatToPixelType( _description.textureFormat() );

    // Use description's channelCount instead of switch
    const int channels = _description.channelCount();
    
    Pixmap<uint8_t> pixmap( _description.width(), _description.height(), channels, nullptr );

    // DSA: Read texture data (OpenGL 4.5+)
    glGetTextureImage(
        _objectID,
        0, // mip level
        format,
        dataType,
        static_cast<GLsizei>( pixmap.width() * pixmap.height() * pixmap.numChannels() ),
        const_cast<uint8_t*>( pixmap.pixels() )
    );
    glCheck;

    return pixmap;
}

// ------------------------------------------------------------------------------------------------
void Texture2D::setMinFilter( MinFilter filter )
{
    assert( isCreated() );
    
    // DSA: Set parameter (OpenGL 4.5+)
    glTextureParameteri( _objectID, GL_TEXTURE_MIN_FILTER, glWrap( filter ) );
    glCheck;
}

// ------------------------------------------------------------------------------------------------
void Texture2D::setMagFilter( MagFilter filter )
{
    assert( isCreated() );
    
    glTextureParameteri( _objectID, GL_TEXTURE_MAG_FILTER, glWrap( filter) );
    glCheck;
}

// ------------------------------------------------------------------------------------------------
void Texture2D::setWrapS( WrapMode wrap )
{
    assert( isCreated() );
    
    glTextureParameteri( _objectID, GL_TEXTURE_WRAP_S, glWrap( wrap ) );
    glCheck;
}

// ------------------------------------------------------------------------------------------------
void Texture2D::setWrapT( WrapMode wrap )
{
    assert( isCreated() );
    
    glTextureParameteri( _objectID, GL_TEXTURE_WRAP_T, glWrap( wrap ) );
    glCheck;
}

// ------------------------------------------------------------------------------------------------
void Texture2D::setAnisotropy( float value )
{
    assert( isCreated() );
    
    // Clamp to valid range
    GLfloat maxAniso = 1.0f;
    glGetFloatv( GL_MAX_TEXTURE_MAX_ANISOTROPY, &maxAniso );
    glCheck;
    
    
    glTextureParameterf( _objectID, GL_TEXTURE_MAX_ANISOTROPY, Math::clamp( value, 1.0f, maxAniso ) );
    glCheck;
}

// ------------------------------------------------------------------------------------------------
void Texture2D::setBorderColor( const Color &color )
{
    assert( isCreated() );
    
    glTextureParameterfv( _objectID, GL_TEXTURE_BORDER_COLOR, color.rgba() );
    glCheck;
}

// ------------------------------------------------------------------------------------------------
void Texture2D::generateMipmaps()
{
    assert( isCreated() );
    
    // DSA: Generate mipmaps (OpenGL 4.5+)
    glGenerateTextureMipmap( _objectID );
    glCheck;
}

// ------------------------------------------------------------------------------------------------
void Texture2D::clear( const Color &clearColor )
{
    assert( isCreated() );

	const GLenum format   = glWrapTextureFormatToPixelFormat( _description.textureFormat() );
	const GLenum dataType = glWrapTextureFormatToPixelType( _description.textureFormat() );

    // DSA: Clear texture (OpenGL 4.4+)
    glClearTexImage(
        _objectID,
        0, // mip level
        format,
        dataType,
        clearColor.rgba()
    );
    glCheck;
}

// ------------------------------------------------------------------------------------------------
void Texture2D::setDefaultSampler()
{
    // Set default filtering
    if( _description.isGenerateMipmapsEnabled() )
        setMinFilter( MinFilter::LinearMipmapLinear );
    else
        setMinFilter( MinFilter::Linear );
    
    setMagFilter( MagFilter::Linear );
    
    // Set default wrap mode
    setWrapS( WrapMode::Repeat );
    setWrapT( WrapMode::Repeat );
    
    // Set default anisotropy
    setAnisotropy( 1.0f );
}

// ------------------------------------------------------------------------------------------------
uint64_t Texture2D::getBindlessHandle() const
{
    assert( isCreated() );
    
    if( _bindlessHandle == 0 )
    {
        // Get bindless handle (ARB_bindless_texture)
        _bindlessHandle = glGetTextureHandleARB( _objectID );
        glCheck;
    }
    
    return _bindlessHandle;
}

// ------------------------------------------------------------------------------------------------
void Texture2D::makeResident()
{
    if( _resident )
        return;
        
    const uint64_t handle = getBindlessHandle();
    
    glMakeTextureHandleResidentARB( handle );
    glCheck;
    
    _resident = true;
}

// ------------------------------------------------------------------------------------------------
void Texture2D::makeNonResident()
{
    if( !_resident )
        return;
        
    const uint64_t handle = getBindlessHandle();
    
    glMakeTextureHandleNonResidentARB( handle );
    glCheck;
    
    _resident = false;
}

//// ------------------------------------------------------------------------------------------------
//void Texture2D::bind() const
//{
//    // Legacy binding for compatibility with existing code
//    // In DSA this is not necessary, but kept for API compatibility
//    glBindTexture( GL_TEXTURE_2D, _objectID );
//    glCheck;
//}
//
//// ------------------------------------------------------------------------------------------------
//void Texture2D::unbind() const
//{
//    glBindTexture( GL_TEXTURE_2D, 0 );
//    glCheck;
//}

// ------------------------------------------------------------------------------------------------
TextureDescription Texture2D::description() const
{
    return _description;
}

// ------------------------------------------------------------------------------------------------
int Texture2D::objectLabelIdentifier() const
{
    return GL_TEXTURE;
}