// VertexArray.h
// 
#ifndef VERTEXARRAY_ONCE
#define VERTEXARRAY_ONCE

#include "s2OpenGL_API.h"

#include "AttributeBuffer.h"
#include "IndexBuffer.h"
#include "Primitive.h"

#include <vector>

namespace s2 {
namespace OpenGL {

class VertexArray;
typedef std::shared_ptr<VertexArray>   VertexArrayPtr;

/************************************************************************************************/
/*                                       VertexArray                                            */
/************************************************************************************************/
class S2OPENGL_API VertexArray
{
public:
	VertexArray();
	~VertexArray();

	AttributeBuffer &attribute( int i );
	IndexBuffer     &indexBuffer()    ;

	AttributeBuffer const &attribute(int i) const;
	IndexBuffer     const &indexBuffer() const;


	bool isIndexed() const;
	int  maxArrayIndex() const;

private:
	void bind() const;
	mutable unsigned int _id;

	mutable std::vector<AttributeBuffer> _attributes;
	mutable IndexBuffer                  _indexBuffer; //optional

	friend class Renderer;
};

} // namespace OpenGL
}
#endif