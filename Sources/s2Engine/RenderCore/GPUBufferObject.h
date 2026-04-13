// GPUBufferObject.h
// 
#ifndef S2_RENDERCORE_GPUBUFFEROBJECT_H
#define S2_RENDERCORE_GPUBUFFEROBJECT_H

#include "s2Engine_API.h"

#include "OpenGLObject.h"

#include <memory>
#include <cstdint>
#include <functional>
#include <type_traits>

namespace s2 {
namespace RenderCore {

/************************************************************************************************/
/*                                      GPUBufferObject                                         */
/************************************************************************************************/
class GPUBufferObject;
typedef std::shared_ptr<GPUBufferObject> GPUBufferObjectPtr;

class S2ENGINE_API GPUBufferObject : public OpenGLObject
{
public:
    enum class UsageHint
    {
        StreamDraw,
        StreamRead,
        StreamCopy,
        StaticDraw,
        StaticRead,
        StaticCopy,
        DynamicDraw,
        DynamicRead,
        DynamicCopy,
    };

    enum class Type
    {
        ArrayBuffer,
        ElementBuffer,
        PixelUnpackBuffer,
        PixelPackBuffer,
        UniformBuffer,        // For UBOs
        ShaderStorageBuffer,  // For SSBOs (OpenGL 4.3+)
        CopyReadBuffer,       // For buffer copies
        CopyWriteBuffer       // For buffer copies
    };

    enum class MapMode
    {
        ReadOnly,
        WriteOnly,
        ReadWrite,
    };

    // Map access flags (OpenGL 4.5+)
    enum class MapAccess : uint32_t
    {
        Read             = 0x0001,
        Write            = 0x0002,
        Persistent       = 0x0040,
        Coherent         = 0x0080,
        InvalidateBuffer = 0x0008,
        InvalidateRange  = 0x0004,
        FlushExplicit    = 0x0010,
        Unsynchronized   = 0x0020 
    };

    // Storage flags (OpenGL 4.4+)
    enum class StorageFlags : uint32_t
    {
        None           = 0,
        DynamicStorage = 0x0100,
        MapRead        = 0x0001,
        MapWrite       = 0x0002,
        MapPersistent  = 0x0040,
        MapCoherent    = 0x0080,
        ClientStorage  = 0x0200 
    };

public:
    struct Data 
    {
        Data() : data( nullptr ), size( 0 ) {}
        
        Data( void* data_, int64_t size_ )
            : data( data_ )
            , size( size_ ) 
        {}

        void*   data;
        int64_t size;
    };

public:
    static GPUBufferObjectPtr New( int64_t size, Type type, UsageHint usageHint );

    // Create with immutable storage (DSA)
    static GPUBufferObjectPtr NewImmutable( int64_t size, Type type, uint32_t storageFlags, const void* initialData = nullptr );

public:
    GPUBufferObject( int64_t size, Type type, UsageHint usageHint );
    ~GPUBufferObject();

    void create()  override;
    void destroy() override;
    
    // Getters
    int64_t   size()        const { return _size; }
    UsageHint usageHint()   const { return _usageHint; }
    Type      type()        const { return _type; }
    bool      isImmutable() const { return _immutableStorage; }
   

    void setData( const void* data, int64_t size, int64_t offset = 0 );     // Set buffer data (DSA)
    void getData( void* data, int64_t size, int64_t offset = 0 ) const;     // Get buffer data (DSA)
    
    // ===== Low-level map/unmap (use mapped() or writeRange() instead when possible) =====
    void* mapRange( int64_t offset, int64_t length, uint32_t accessFlags ); // Map buffer range (DSA)
    bool  unmap();                                                          // Unmap buffer (DSA)

    // ===== RAII Mapped Buffer Access =====
    //
    // Maps a range, invokes the callable with a typed pointer, and automatically unmaps.
    // Exception-safe: unmap is guaranteed even if the callable throws.
    //
    // Usage:
    //   buffer->mapped<MyVertex>( 0, count, flags, [&]( MyVertex* dst ) {
    //       for( int i = 0; i < count; ++i )
    //           dst[i] = ...;
    //   });
    //
    //   auto pixels = buffer->mapped<const uint8_t>( 0, size, readFlags, [&]( const uint8_t* src ) {
    //       return Pixmap( width, height, 4, src );
    //   });
    //
    template<typename T, typename Fn>
    auto mapped( int64_t offset, int64_t length, uint32_t accessFlags, Fn&& fn )
        -> std::invoke_result_t<Fn, T*>
    {
        T* ptr = static_cast<T*>( mapRange( offset, length, accessFlags ) );
        assert( ptr && "GPUBufferObject::mapped: mapRange returned null" );

        if constexpr( std::is_void_v<std::invoke_result_t<Fn, T*>> )
        {
            fn( ptr );
            unmap();
        }
        else
        {
            auto result = fn( ptr );
            unmap();
            return result;
        }
    }

    // Convenience: maps the entire buffer for writing with invalidation.
    //
    // Usage:
    //   buffer->writeAll<float>( [&]( float* dst ) {
    //       std::memcpy( dst, srcData, byteCount );
    //   });
    //
    template<typename T, typename Fn>
    void writeAll( Fn&& fn )
    {
        mapped<T>( 0,
                   _size,
                   static_cast<uint32_t>( MapAccess::Write ) | static_cast<uint32_t>( MapAccess::InvalidateBuffer ),
                   std::forward<Fn>( fn ) );
    }

    // Convenience: maps a byte range for writing with invalidation.
    //
    // Usage:
    //   buffer->writeRange<ImDrawVert>( 0, totalVtxBytes, [&]( ImDrawVert* dst ) {
    //       for( auto& list : drawLists )  { memcpy(...); dst += list.size; }
    //   });
    //
    template<typename T, typename Fn>
    void writeRange( int64_t offset, int64_t length, Fn&& fn )
    {
        mapped<T>( offset, length, static_cast<uint32_t>( MapAccess::Write ) | static_cast<uint32_t>( MapAccess::InvalidateBuffer ), std::forward<Fn>( fn ) );
    }

    // Convenience: maps a byte range for reading.
    //
    // Usage:
    //   auto img = buffer->readRange<const uint8_t>( 0, sizeInBytes, [&]( const uint8_t* src ) {
    //       return Pixmap<uint8_t>( w, h, 4, src );
    //   });
    //
    template<typename T, typename Fn>
    auto readRange( int64_t offset, int64_t length, Fn&& fn )
        -> std::invoke_result_t<Fn, T*>
    {
        return mapped<T>( offset, length, static_cast<uint32_t>( MapAccess::Read ), std::forward<Fn>( fn ) );
    }

    void flushMappedRange( int64_t offset, int64_t length );     // Flush mapped range (DSA)
    
    // Copy between buffers (DSA)
    void copyTo( const GPUBufferObjectPtr& destination, 
                 int64_t readOffset, 
                 int64_t writeOffset, 
                 int64_t size ) const;
    
    // Clear buffer (OpenGL 4.3+)
    void clear( const void* clearValue = nullptr );
    
    // Invalidate buffer (OpenGL 4.3+)
    void invalidate();
    void invalidateRange( int64_t offset, int64_t length );

    // ===== Bindless Support (Buffer Device Address - OpenGL 4.5+ / ARB_bindless) =====
    
    // Get GPU address (requires ARB_gpu_shader_int64)
    uint64_t getGPUAddress() const;
    
    // Make buffer resident (bindless)
    void makeResident( uint32_t access );
    void makeNonResident();
    bool isResident() const { return _resident; }

private:
    void reset() override;
    int  objectLabelIdentifier() const override;
    
    // DSA helpers
    void allocateStorage( const void* initialData );
    void allocateImmutableStorage( uint32_t storageFlags, const void* initialData );

private:
    int64_t      _size;              // Byte size
    UsageHint    _usageHint;
    Type         _type;
    bool         _immutableStorage;  // Immutable storage flag
    uint32_t     _storageFlags;      // Storage flags
    
    // Bindless state
    mutable uint64_t _gpuAddress;
    bool             _resident;
};

// Bitwise operators for MapAccess
inline uint32_t operator|( GPUBufferObject::MapAccess a, GPUBufferObject::MapAccess b ) { return static_cast<uint32_t>(a) | static_cast<uint32_t>(b); }
inline uint32_t operator|( uint32_t a, GPUBufferObject::MapAccess b )                   { return a | static_cast<uint32_t>(b); }

// Bitwise operators for StorageFlags
inline uint32_t operator|( GPUBufferObject::StorageFlags a, GPUBufferObject::StorageFlags b ) { return static_cast<uint32_t>(a) | static_cast<uint32_t>(b); }
inline uint32_t operator|( uint32_t a, GPUBufferObject::StorageFlags b )                      { return a | static_cast<uint32_t>(b); }

} // namespace RenderCore
} // namespace s2
#endif // !S2_RENDERCORE_GPUBUFFEROBJECT_H