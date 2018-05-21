// VertexArray.h
// 
#ifndef VERTEXARRAY_ONCE
#define VERTEXARRAY_ONCE

#include "S2RendererAPI.h"

#include "AttributeBuffer.h"
#include "IndexBuffer.h"

#include <vector>

namespace OpenGL {

//class VertexArray;
//typedef std::shared_ptr<VertexArray>   VertexArrayPtr;

/************************************************************************************************/
/*                                       VertexArray                                            */
/************************************************************************************************/
class S2RENDERER_API VertexArray// : public std::enable_shared_from_this<VertexArray>
{
private:
	unsigned int _id; // opengl > 3.0

	mutable std::vector<AttributeBuffer> _attributes;
	mutable IndexBuffer                  _indexBuffer; //optional

public:
	VertexArray();
	~VertexArray();

	AttributeBuffer *attribute(int i) const;
	IndexBuffer     *indexBuffer()    const;

	bool isIndexed() const;
	int  maxArrayIndex() const;

	void bind() const;
};

} // namespace OpenGL

#endif