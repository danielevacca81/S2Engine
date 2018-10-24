// VertexArray.h
// 
#ifndef VERTEXARRAY_ONCE
#define VERTEXARRAY_ONCE

#include "s2OpenGL_API.h"

#include "OpenGLObject.h"

#include "AttributeBuffer.h"
#include "IndexBuffer.h"

#include <vector>

namespace s2 {
namespace OpenGL {


/************************************************************************************************/
/*                                       VertexArray                                            */
/************************************************************************************************/
class VertexArray;
typedef std::shared_ptr<VertexArray>   VertexArrayPtr;

class S2OPENGL_API VertexArray : public OpenGLObject, public std::enable_shared_from_this<VertexArray>
{
public:
	OBJECT_DECLARE_MOVEABLE( VertexArray )
	OBJECT_DISABLE_COPY( VertexArray )

	VertexArray();
	~VertexArray();

	void bind()    const override;
	void unbind()  const override;
	bool create()  override;
	void destroy() override;

	AttributeBuffer &attribute( int i );
	IndexBuffer     &indexBuffer();

	AttributeBuffer const &attribute(int i) const;
	IndexBuffer     const &indexBuffer() const;

	bool isIndexed() const;
	int  maxArrayIndex() const;

private:
	void reset() override;

private:
	mutable std::vector<AttributeBuffer> _attributes;
	mutable IndexBuffer                  _indexBuffer; //optional

	friend class Renderer;
};

} // namespace OpenGL
}
#endif