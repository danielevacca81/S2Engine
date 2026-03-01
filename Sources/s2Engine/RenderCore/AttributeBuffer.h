// AttributeBuffer.h
// 
#ifndef S2_RENDERCORE_ATTRIBUTEBUFFER_H
#define S2_RENDERCORE_ATTRIBUTEBUFFER_H

#include "GPUBufferObject.h"

#include "s2Engine_API.h"

#include <cstdint>
#include <string>

namespace s2 {
namespace RenderCore {

/************************************************************************************************/
/*                                     AttributeBuffer                                          */
/************************************************************************************************/
class S2ENGINE_API AttributeBuffer
{
public:
    enum class ComponentDatatype
    {
        Byte,
        UnsignedByte,
        Short,
        UnsignedShort,
        Int,
        UnsignedInt,
        Float,
        HalfFloat,
        Double,
    };

public:
    AttributeBuffer() = default;
    AttributeBuffer(
        const void *data,
        int64_t bufferSize,
        GPUBufferObject::Type bufferType,
        GPUBufferObject::UsageHint usageHint,
        ComponentDatatype componentDatatype,
        int numberOfComponents,
        bool normalize = false,
        int64_t offset = 0,
        int64_t stride = 0);

    void attach( unsigned int vaoID, int location );  // Attach to VAO using DSA (requires VAO ID)
    void detach( unsigned int vaoID );                // Detach from VAO using DSA (requires VAO ID)

    // Getters
    ComponentDatatype componentDatatype()  const { return _componentDatatype; }
    bool              isValid()            const { return _gpuBuffer && _gpuBuffer->isValid(); }
    int               numberOfComponents() const { return _numberOfComponents; }
    int64_t           offset()             const { return _offset; }
    bool              normalize()          const { return _normalize; }
    int64_t           strideInBytes()      const { return _stride; }
    int               numberOfVertices()   const;
    int               location()           const { return _location; }

    //
    void setObjectLabel( const std::string& label );

private:
    GPUBufferObjectPtr _gpuBuffer;
    ComponentDatatype  _componentDatatype { ComponentDatatype::Float };

    int     _location           { -1 };
    int     _numberOfComponents { 0 };
    int64_t _offset             { 0 };
    int64_t _stride             { 0 };
    bool    _normalize          { false };
};

} // namespace RenderCore
} // namespace s2
#endif // !S2_RENDERCORE_ATTRIBUTEBUFFER_H