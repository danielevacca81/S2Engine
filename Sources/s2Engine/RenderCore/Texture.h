// Texture.h
//
#ifndef S2_RENDERCORE_TEXTURE_H
#define S2_RENDERCORE_TEXTURE_H

#include "s2Engine_API.h"

#include "ImageFormat.h"
#include "TextureDescription.h"

#include "OpenGLObject.h"
#include "GPUBufferObject.h"

#include "Graphics/Pixmap.h"
#include "Graphics/Color.h"

#include <memory>
#include <cstdint>

namespace s2 {
namespace RenderCore {

/************************************************************************************************/
/*                                          Texture2D                                           */
/************************************************************************************************/
class Texture2D;
typedef std::shared_ptr<Texture2D> Texture2DPtr;

class S2ENGINE_API Texture2D : public OpenGLObject
{
public:
    // Texture filtering
    enum class MinFilter
    {
        Nearest,
        Linear,
        NearestMipmapNearest,
        LinearMipmapNearest,
        NearestMipmapLinear,
        LinearMipmapLinear
    };

    enum class MagFilter
    {
        Nearest,
        Linear
    };

    // Texture wrapping
    enum class WrapMode
    {
        Repeat,
        ClampToEdge,
        ClampToBorder,
        MirroredRepeat
    };

public:	
    static Texture2DPtr New( const TextureDescription& desc, void* data = nullptr );

public:
    Texture2D( const TextureDescription& description, void* data );
    ~Texture2D();

    TextureDescription description() const;

    void create()  override;
    void destroy() override;

    // Upload pixel data (DSA)
    void setData( void* pixels );
    
    // Resize texture (DSA - recreates storage)
    void resize( int width, int height );
    
    // Update partial region (DSA)
    void update( int xOffset, int yOffset, int width, int height, const ImageFormat& imgFormat,const ImageDataType& imgDataType,void* pixels );
    void update( int xOffset, int yOffset, int width, int height, const ImageFormat& imgFormat,const ImageDataType& imgDataType,const GPUBufferObjectPtr& gpuBuffer );
    
    // Read texture data (DSA)
    Pixmap<uint8_t> readData() const;
    
    // Set sampler parameters (DSA)
    void setMinFilter( MinFilter filter );
    void setMagFilter( MagFilter filter );
    void setWrapS( WrapMode wrap );
    void setWrapT( WrapMode wrap );
    void setAnisotropy( float value );
    void setBorderColor( const Color& color );
    
    // Generate mipmaps (DSA)
    void generateMipmaps();
    
    // Clear texture (OpenGL 4.4+)
    void clear( const Color &color );
    
    // ===== Bindless Support (ARB_bindless_texture) =====
    
    // Get bindless handle (cached)
    uint64_t bindlessHandle() const;
    
    // Make texture resident in GPU memory
    void makeResident();
    
    // Make texture non-resident
    void makeNonResident();
    
    // Check if texture is currently resident
    bool isResident() const { return _resident; }

private:
    void reset() override;
    void setDefaultSampler();
    int  objectLabelIdentifier() const override;
    
    // DSA helpers
    void allocateStorage();

private:
    TextureDescription _description;
    
    // Bindless state
    mutable uint64_t _bindlessHandle = 0;
    bool _resident = false;
};

} // namespace RenderCore
} // namespace s2
#endif // !S2_RENDERCORE_TEXTURE_H