// VertexArray.cpp
// 
#include "VertexArray.h"

#include "Math/Math.h"

#include "OpenGL.h"
#include "OpenGLWrap.h"

using namespace s2::OpenGL;

static int maxVertexAttrib = 16;

//-------------------------------------------------------------------------------------------------
VertexArray::VertexArray()
: _id(0)
{
	if( maxVertexAttrib == 0) // @todo: move to a proper Device class (TBD)
		glGetIntegerv( GL_MAX_VERTEX_ATTRIBS, &maxVertexAttrib);

	_attributes.resize ( maxVertexAttrib );
	_attributes.reserve( maxVertexAttrib );

	//glGenVertexArrays( 1, &_id );
}

//-------------------------------------------------------------------------------------------------
VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &_id );
}

//-------------------------------------------------------------------------------------------------
void VertexArray::bind() const
{
	if( _id == 0 )
	{
		glGenVertexArrays( 1, &_id );
		glBindVertexArray( _id );

		if( _indexBuffer.isValid() )
			_indexBuffer.bind();

		for( size_t i=0; i < _attributes.size(); ++i )
			_attributes[i].attach( i );
	}
	else
		glBindVertexArray( _id );

	glCheck;
}

//-------------------------------------------------------------------------------------------------
AttributeBuffer *VertexArray::attribute( int i ) const
{
#ifndef _NDEBUG
	assert( i>=0 && i<maxVertexAttrib );
#endif
	return &_attributes[i];
}

//-------------------------------------------------------------------------------------------------
IndexBuffer     *VertexArray::indexBuffer() const
{
	return &_indexBuffer;
}

//-------------------------------------------------------------------------------------------------
bool VertexArray::isIndexed() const
{
	return _indexBuffer.isValid();
}

//-------------------------------------------------------------------------------------------------
int VertexArray::maxArrayIndex() const
{
	int maximumArrayIndex = 0;

	for( size_t i = 0; i < _attributes.size(); ++i )
	{
		//VertexBufferAttribute attribute = _attributes[i].VertexBufferAttribute;

		//if( _attributes[i].Dirty )
		//{
		//	if (attribute != null)
		//	{
		//		Attach(i);
		//	}
		//	else
		//	{
		//		Detach(i);
		//	}

		//	_attributes[i].Dirty = false;
		//}

		if( _attributes[i].isValid() )
			maximumArrayIndex = Math::max( _attributes[i].numberOfVertices() - 1, maximumArrayIndex);
	}

	return maximumArrayIndex;
}