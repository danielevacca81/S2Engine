// VertexArray.h
// 
#ifndef S2_RENDERCORE_VERTEXARRAY_H
#define S2_RENDERCORE_VERTEXARRAY_H

#include "s2Engine_API.h"

#include "OpenGLObject.h"
#include "AttributeBuffer.h"
#include "IndexBuffer.h"

#include <vector>
#include <memory>
#include <optional>

namespace s2 {
namespace RenderCore {


/************************************************************************************************/
/*                                       VertexArray                                            */
/************************************************************************************************/
class VertexArray;
typedef std::shared_ptr<VertexArray> VertexArrayPtr;

class S2ENGINE_API VertexArray : public OpenGLObject
{
public:
    // CANNOT BE SHARED BETWEEN CONTEXTS
    static VertexArrayPtr New( GPUBufferObject::UsageHint hint = GPUBufferObject::UsageHint::StaticDraw );

public:
    VertexArray( GPUBufferObject::UsageHint hint = GPUBufferObject::UsageHint::StaticDraw );
    ~VertexArray();

    void create()  override;
    void destroy() override;
    
    // Binding (required only for rendering)
    void bind()    const;
    void unbind()  const;

    // ===== DSA API (OpenGL 4.5+) - ONLY WAY TO CONFIGURE =====
    
    // Set attribute using DSA
    void setAttribute( int location, const AttributeBuffer& attribute );
    
    // Set index buffer using DSA
    void setIndexBuffer( const IndexBuffer& indexBuffer );
    
    // Get attribute (read-only access)
    const AttributeBuffer& attribute( int index ) const;
    
    // Get attribute (mutable access for data updates)
    AttributeBuffer& attribute( int index );
    
    // Get index buffer (read-only access)
    const IndexBuffer& indexBuffer() const;
    
    // Get index buffer (mutable access for data updates)
    IndexBuffer& indexBuffer();

    // ===== Queries =====
    
    GPUBufferObject::UsageHint usageHint() const { return _usageHint; }
    bool isIndexed() const;
    int  maxArrayIndex() const;
    int  attributeCount() const;

private:
    void reset() override;
    int  objectLabelIdentifier() const override;

private:
    GPUBufferObject::UsageHint      _usageHint;
    std::vector<AttributeBuffer>    _attributes;
    std::optional<IndexBuffer>      _indexBuffer;
};

} // namespace RenderCore
} // namespace s2
#endif // !S2_RENDERCORE_VERTEXARRAY_H