// VertexArray.h
// 
#ifndef VERTEXARRAY_ONCE
#define VERTEXARRAY_ONCE

#include "Renderer_API.h"

#include "AttributeBuffer.h"
#include "IndexBuffer.h"

#include <vector>

namespace OpenGL {

//class VertexArray;
//typedef std::shared_ptr<VertexArray>   VertexArrayPtr;

/************************************************************************************************/
/*                                       VertexArray                                            */
/************************************************************************************************/
class RENDERER_API VertexArray
{
public:
	VertexArray();
	~VertexArray();

	AttributeBuffer *attribute(int i) const;
	IndexBuffer     *indexBuffer()    const;

	bool isIndexed() const;
	int  maxArrayIndex() const;

	void bind() const;

private:
	mutable unsigned int _id; // opengl > 3.0

	mutable std::vector<AttributeBuffer> _attributes;
	mutable IndexBuffer                  _indexBuffer; //optional

};

} // namespace OpenGL

#endif