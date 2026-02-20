// ImageLoader.h
//
#ifndef S2_RESOURCES_IMAGELOADER_H
#define S2_RESOURCES_IMAGELOADER_H

#include "s2Engine_API.h"
#include "Graphics/Pixmap.h"

#include <string>
#include <memory>
#include <optional>
#include <filesystem>

namespace s2 {
namespace Resources {

// Supported image formats
enum class ImageFormat
{
    // Common file formats
    PNG,
    JPG,
    JPEG,
    BMP,
    TGA,
    PSD,
    GIF,
    HDR,
    PIC,
    PNM,
    
    // GPU formats
    DDS,
    KTX,
    KMG,
    
    Unknown
};

// Image loading result with metadata
struct S2ENGINE_API ImageData
{
    s2::Pixmap<uint8_t> pixmap;
    ImageFormat         format     = ImageFormat::Unknown;
    bool                compressed = false;
    int                 mipLevels  = 1;
    int                 layers     = 1;
    int                 faces      = 1;
    bool                hdr        = false;  // High Dynamic Range
};

class S2ENGINE_API ImageLoader
{
public:
    // Load image from file (auto-detects format)
    static std::optional<ImageData> loadFromFile( const std::filesystem::path& filename, bool flipVertically = true );
    
    // Load image from memory buffer
    static std::optional<ImageData> loadFromMemory( const uint8_t* data, size_t size, bool flipVertically = true );
    
    // Check if file format is supported
    static bool isFormatSupported( const std::filesystem::path& filename );
    
    // Get format from file extension
    static ImageFormat getFormatFromExtension( const std::filesystem::path& filename );
    
    // Query capabilities
    static bool isGPUFormat( ImageFormat format );
    static bool isCommonFormat( ImageFormat format );
};

} // namespace Resources
} // namespace s2

#endif // !S2_RESOURCES_IMAGELOADER_H