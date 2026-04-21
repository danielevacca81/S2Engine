// IndexBuffer.h
// 
#ifndef S2_RENDERCORE_INDEXBUFFER_H
#define S2_RENDERCORE_INDEXBUFFER_H

#include "s2Engine_API.h"

#include "GPUBufferObject.h"

#include <memory>
#include <cstdint>
#include <type_traits>

namespace s2 {
namespace RenderCore {

/************************************************************************************************/
/*                                          IndexBuffer                                         */
/************************************************************************************************/
class S2ENGINE_API IndexBuffer
{
public:
    enum class IndexDataType
    {
        UnsignedShort,  // 16-bit indices
        UnsignedInt     // 32-bit indices
    };

public:
    IndexBuffer( int64_t sizeInBytes, IndexDataType dataType, GPUBufferObject::UsageHint usageHint );
	IndexBuffer( const void* data, int64_t sizeInBytes, IndexDataType dataType, GPUBufferObject::UsageHint usageHint );
    IndexBuffer( const GPUBufferObjectPtr& gpuBuffer, IndexDataType dataType );

    // Initialize/reinitialize buffer
    void set( int64_t sizeInBytes, IndexDataType dataType, GPUBufferObject::UsageHint usageHint );

    // State queries
    int  count()             const { return _count; }
    IndexDataType dataType() const { return _dataType; }

    // ===== DSA API (via GPUBufferObject) =====
    
    // Set index data (DSA)
    void setData( const void* data, int64_t size, int64_t offset = 0 );
    
    // Set index data from typed arrays (convenience)
    void setIndices( const uint16_t* indices, int count );
    void setIndices( const uint32_t* indices, int count );
    
    // Get index data (DSA)
    void getData( void* data, int64_t size, int64_t offset = 0 ) const;
    
    // Low-level map/unmap
    void* map( uint32_t accessFlags );
    bool  unmap();

    // ===== RAII Mapped Buffer Access =====

    // Maps the entire buffer, invokes the callable, and unmaps automatically.
    template<typename T, typename Fn>
    auto mapped( uint32_t accessFlags, Fn&& fn ) -> std::invoke_result_t<Fn, T*>
    {
        return _gpuBuffer->mapped<T>( 0, _gpuBuffer->size(), accessFlags, std::forward<Fn>( fn ) );
    }

    // Convenience: maps entire buffer for writing with invalidation.
    template<typename T, typename Fn>
    void writeAll( Fn&& fn )
    {
        _gpuBuffer->writeAll<T>( std::forward<Fn>( fn ) );
    }

    // Access underlying buffer
    unsigned int id() const { return _gpuBuffer->id(); }

private:
    void updateCount();

private:
    GPUBufferObjectPtr _gpuBuffer;
    IndexDataType      _dataType  { IndexDataType::UnsignedInt };
    int                _count     { 0 };
};

} // namespace RenderCore
} // namespace s2
#endif // !S2_RENDERCORE_INDEXBUFFER_H