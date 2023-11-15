// VertexArray.h
// 
#ifndef VERTEXARRAY_ONCE
#define VERTEXARRAY_ONCE

#include "RenderSystem_API.h"

#include "OpenGLObject.h"

#include "AttributeBuffer.h"
#include "IndexBuffer.h"

#include <vector>

namespace RenderSystem {


/************************************************************************************************/
/*                                       VertexArray                                            */
/************************************************************************************************/
class VertexArray;
typedef std::shared_ptr<VertexArray>   VertexArrayPtr;

class RENDERSYSTEM_API VertexArray : public OpenGLObject
{
public:
	// CANNOT BE SHARED BETWEEN CONTEXTS
	static VertexArrayPtr New( const BufferObject::UsageHint &hint );
public:
	VertexArray( const BufferObject::UsageHint &hint );
	~VertexArray();

	void bind()    const override;
	void unbind()  const override;
	void create()  override;
	void destroy() override;

	AttributeBuffer &attribute( int i );
	IndexBuffer     &indexBuffer();

	AttributeBuffer const &attribute(int i) const;
	IndexBuffer     const &indexBuffer() const;

	BufferObject::UsageHint usageHint() const { return _usageHint; }

	bool isIndexed() const;
	int  maxArrayIndex() const;

private:
	void reset() override;

private:
	BufferObject::UsageHint              _usageHint;
	mutable std::vector<AttributeBuffer> _attributes;
	mutable IndexBuffer                  _indexBuffer; //optional
};

} // namespace RenderSystem
#endif