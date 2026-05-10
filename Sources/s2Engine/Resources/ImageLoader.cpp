// ImageLoader.cpp  
//  
#include "ImageLoader.h"  
  
// STB_IMAGE for common formats (PNG, JPG, etc.)  
#define STB_IMAGE_IMPLEMENTATION  
#include <stb_image.h>  
  
#if defined (USE_GLI_FOR_GPU_FORMATS)  
// GLI for GPU formats (DDS, KTX, etc.)  
#include <gli/gli.hpp>  
#endif  
  
#include <algorithm>  
#include <cctype>  
#include <fstream>  
  
using namespace s2::Resources;  
  
#pragma region Helper Functions  
// ------------------------------------------------------------------------------------------------  
static inline ImageFormat detectFormat( const std::filesystem::path& filename )  
{  
    if( filename.empty() )  
        return ImageFormat::Unknown;  
  
    if( !filename.has_extension() )  
        return ImageFormat::Unknown;  
  
    std::string ext = filename.extension().string();  
  
    // Remove leading dot if present  
    if( !ext.empty() && ext[0] == '.' )  
        ext = ext.substr( 1 );  
  
    // Convert to lowercase  
    std::transform( ext.begin(), ext.end(), ext.begin(),  
                    [] ( unsigned char c ) { return std::tolower( c ); } );  
  
    // Common formats  
    if( ext == "png" )  return ImageFormat::PNG;  
    if( ext == "jpg" )  return ImageFormat::JPG;  
    if( ext == "jpeg" ) return ImageFormat::JPEG;  
    if( ext == "bmp" )  return ImageFormat::BMP;  
    if( ext == "tga" )  return ImageFormat::TGA;  
    if( ext == "psd" )  return ImageFormat::PSD;  
    if( ext == "gif" )  return ImageFormat::GIF;  
    if( ext == "hdr" )  return ImageFormat::HDR;  
    if( ext == "pic" )  return ImageFormat::PIC;  
    if( ext == "pnm" )  return ImageFormat::PNM;  
  
    // GPU formats  
    if( ext == "dds" )  return ImageFormat::DDS;  
    if( ext == "ktx" )  return ImageFormat::KTX;  
    if( ext == "kmg" )  return ImageFormat::KMG;  
  
    return ImageFormat::Unknown;  
}  
  
// ------------------------------------------------------------------------------------------------  
static inline std::optional<ImageData> loadWithSTB( const std::filesystem::path& filename, bool flipVertically )  
{  
    // Set flip flag  
    stbi_set_flip_vertically_on_load( flipVertically );  
  
    int width, height, channels;  
    uint8_t* pixels = stbi_load( filename.string().c_str(), &width, &height, &channels, 0 );  
  
    if( !pixels )  
        return std::nullopt;  
  
    // Create Pixmap and copy data  
    s2::Pixmap<uint8_t> pixmap( width, height, channels, nullptr );  
  
    const size_t totalBytes = size_t(width * height * channels);  
    for( size_t i = 0; i < totalBytes; ++i )  
        pixmap[i] = pixels[i];  
  
    // Free STB memory  
    stbi_image_free( pixels );  
  
    ImageData result;  
    result.pixmap     = std::move( pixmap );  
    result.format     = detectFormat( filename );  
    result.mipLevels  = 1;  
    result.layers     = 1;  
    result.faces      = 1;  
    result.compressed = false;  
    result.hdr        = result.format == ImageFormat::HDR;  
  
    return result;  
}  
  
  
// ------------------------------------------------------------------------------------------------  
static inline std::optional<ImageData> loadWithSTBFromMemory( const uint8_t* data, size_t size, bool flipVertically )  
{  
    stbi_set_flip_vertically_on_load( flipVertically );  
  
    int width, height, channels;  
    uint8_t* pixels = stbi_load_from_memory(   
        data,   
        static_cast<int>( size ),   
        &width,   
        &height,   
        &channels,   
        0 );  
  
    if( !pixels )  
        return std::nullopt;  
  
    s2::Pixmap<uint8_t> pixmap( width, height, channels, nullptr );  
      
    const size_t totalBytes = size_t(width * height * channels);  
    for( size_t i = 0; i < totalBytes; ++i )  
        pixmap[i] = pixels[i];  
  
    stbi_image_free( pixels );  
  
    ImageData result;  
    result.pixmap     = std::move( pixmap );  
    result.format     = ImageFormat::Unknown;  
    result.mipLevels  = 1;  
    result.layers     = 1;  
    result.faces      = 1;  
    result.compressed = false;  
  
    return result;  
}  
  
#if defined (USE_GLI_FOR_GPU_FORMATS)  
// ------------------------------------------------------------------------------------------------  
static inline std::optional<s2::Pixmap<uint8_t>> convertGLIToPixmap( const void* gliTexture, bool flipVertically )  
{  
    if( !gliTexture )  
        return std::nullopt;  
  
    const gli::texture* texture = static_cast<const gli::texture*>( gliTexture );  
  
    if( texture->empty() )  
        return std::nullopt;  
  
    gli::texture2d tex2d( *texture );  
    if( tex2d.empty() )  
        return std::nullopt;  
  
    const auto extent = tex2d.extent();  
    const int width = extent.x;  
    const int height = extent.y;  
  
    int channels = 4;  
    gli::format format = texture->format();  
  
    switch( gli::component_count( format ) )  
    {  
    case 1:  channels = 1; break;  
    case 2:  channels = 2; break;  
    case 3:  channels = 3; break;  
    case 4:  channels = 4; break;  
    default: channels = 4; break;  
    }  
  
    bool needsConversion = gli::is_compressed( format ) ||  
        format != gli::FORMAT_RGBA8_UNORM_PACK8;  
  
    gli::texture2d convertedTex;  
    const uint8_t* sourceData = nullptr;  
  
    if( needsConversion )  
    {  
        convertedTex = gli::convert( tex2d, gli::FORMAT_RGBA8_UNORM_PACK8 );  
        if( convertedTex.empty() )  
            return std::nullopt;  
  
        sourceData = static_cast<const uint8_t*>( convertedTex.data() );  
        channels = 4;  
    }  
    else  
    {  
        sourceData = static_cast<const uint8_t*>( tex2d.data() );  
    }  
  
    s2::Pixmap<uint8_t> pixmap( width, height, channels, nullptr );  
  
    const size_t pixelCount = width * height * channels;  
  
    if( flipVertically )  
    {  
        for( int y = 0; y < height; ++y )  
        {  
            const int srcRow = height - 1 - y;  
            const size_t srcOffset = srcRow * width * channels;  
            const size_t dstOffset = y * width * channels;  
  
            for( int x = 0; x < width * channels; ++x )  
                pixmap[dstOffset + x] = sourceData[srcOffset + x];  
        }  
    }  
    else  
    {  
        for( size_t i = 0; i < pixelCount; ++i )  
            pixmap[i] = sourceData[i];  
    }  
  
    return pixmap;  
}  
  
// ------------------------------------------------------------------------------------------------  
static inline std::optional<ImageData> loadWithGLI( const std::filesystem::path& filename, bool flipVertically )  
{  
    ImageFormat format = detectFormat( filename );  
      
    gli::texture texture;  
      
    // Convert path to string for GLI (which expects std::string)  
    const std::string filenameStr = filename.string();  
      
    switch( format )  
    {  
    case ImageFormat::DDS: texture = gli::load_dds( filenameStr ); break;  
    case ImageFormat::KTX: texture = gli::load_ktx( filenameStr ); break;  
    case ImageFormat::KMG: texture = gli::load_kmg( filenameStr ); break;  
    default: return std::nullopt;  
    }  
      
    if( texture.empty() )  
        return std::nullopt;  
  
    auto pixmap = convertGLIToPixmap( &texture, flipVertically );  
    if( !pixmap )  
        return std::nullopt;  
  
    ImageData result;  
    result.pixmap     = std::move( *pixmap );  
    result.format     = format;  
    result.mipLevels  = static_cast<int>( texture.levels() );  
    result.layers     = static_cast<int>( texture.layers() );  
    result.faces      = static_cast<int>( texture.faces() );  
    result.compressed = gli::is_compressed( texture.format() );  
  
    return result;  
}  
  
  
// ------------------------------------------------------------------------------------------------  
static inline std::optional<ImageData> loadWithGLIFromMemory( const uint8_t* data, size_t size, bool flipVertically )  
{  
    gli::texture texture = gli::load( reinterpret_cast<const char*>( data ), size );  
      
    if( texture.empty() )  
        return std::nullopt;  
  
    auto pixmap = convertGLIToPixmap( &texture, flipVertically );  
    if( !pixmap )  
        return std::nullopt;  
  
    ImageData result;  
    result.pixmap     = std::move( *pixmap );  
    result.mipLevels  = static_cast<int>( texture.levels() );  
    result.layers     = static_cast<int>( texture.layers() );  
    result.faces      = static_cast<int>( texture.faces() );  
    result.compressed = gli::is_compressed( texture.format() );  
  
    return result;  
}  
#endif  
  
#pragma endregion  
  
  
// -------------------------------------------------------------------------------------------------  
std::optional<ImageData> ImageLoader::loadFromFile( const std::filesystem::path& filename, bool flipVertically )  
{  
    if( filename.empty() )  
        return std::nullopt;  
  
    ImageFormat format = detectFormat( filename );  
      
#if defined (USE_GLI_FOR_GPU_FORMATS)  
    if( isGPUFormat( format ) )  
        return loadWithGLI( filename, flipVertically );  
    else  
#else  
    if( isCommonFormat( format ) )  
        return loadWithSTB( filename, flipVertically );  
#endif      
    return std::nullopt;  
}  
  
// -------------------------------------------------------------------------------------------------  
std::optional<ImageData> ImageLoader::loadFromMemory( const uint8_t* data, size_t size, bool flipVertically )  
{  
    if( !data || size == 0 )  
        return std::nullopt;  
  
    // Try STB_IMAGE first (more common)  
    auto result = loadWithSTBFromMemory( data, size, flipVertically );  
    if( result )  
        return result;  
  
#if defined (USE_GLI_FOR_GPU_FORMATS)  
    // Fallback to GLI for GPU formats  
    return loadWithGLIFromMemory( data, size, flipVertically );  
#else  
    return std::nullopt;  
#endif  
}  
  
// -------------------------------------------------------------------------------------------------  
bool ImageLoader::isFormatSupported( const std::filesystem::path& filename )  
{  
    return getFormatFromExtension( filename ) != ImageFormat::Unknown;  
}  
  
// -------------------------------------------------------------------------------------------------  
ImageFormat ImageLoader::getFormatFromExtension( const std::filesystem::path& filename )  
{  
    return detectFormat( filename );  
}  
  
// -------------------------------------------------------------------------------------------------  
bool ImageLoader::isGPUFormat( ImageFormat format )  
{  
    return format == ImageFormat::DDS ||   
           format == ImageFormat::KTX ||   
           format == ImageFormat::KMG;  
}  
  
// -------------------------------------------------------------------------------------------------  
bool ImageLoader::isCommonFormat( ImageFormat format )  
{  
    return format == ImageFormat::PNG ||   
           format == ImageFormat::JPG ||   
           format == ImageFormat::JPEG ||  
           format == ImageFormat::BMP ||   
           format == ImageFormat::TGA ||   
           format == ImageFormat::PSD ||  
           format == ImageFormat::GIF ||   
           format == ImageFormat::HDR ||   
           format == ImageFormat::PIC ||   
           format == ImageFormat::PNM;  
}