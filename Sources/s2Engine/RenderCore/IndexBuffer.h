// IndexBuffer.h
// 
#ifndef S2_RENDERCORE_INDEXBUFFER_H
#define S2_RENDERCORE_INDEXBUFFER_H

#include "s2Engine_API.h"

#include "GPUBufferObject.h"

#include <memory>
#include <cstdint>

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
    IndexBuffer() = default;
    IndexBuffer( int64_t sizeInBytes, IndexDataType dataType, GPUBufferObject::UsageHint usageHint );
	IndexBuffer( const void* data, int64_t sizeInBytes, IndexDataType dataType, GPUBufferObject::UsageHint usageHint );

    // Initialize/reinitialize buffer
    void set( int64_t sizeInBytes, IndexDataType dataType, GPUBufferObject::UsageHint usageHint );

    // State queries
    bool isValid()           const { return _gpuBuffer && _gpuBuffer->isCreated(); }
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
    
    // Map buffer (DSA)
    void* map( uint32_t accessFlags );
    bool  unmap();

    // Access underlying buffer
    GPUBufferObjectPtr gpuBuffer() const { return _gpuBuffer; }
    unsigned int id() const              { return _gpuBuffer ? _gpuBuffer->id() : 0; }

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