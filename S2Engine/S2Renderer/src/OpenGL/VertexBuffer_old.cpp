// VertexBuffer.cpp
// 
#include "VertexBuffer.h"

#include <assert.h>

namespace OpenGL{

// -----------------------------------------------------------------------------------------------
VertexBuffer::VertexBuffer()
{}

// -----------------------------------------------------------------------------------------------
VertexBuffer::~VertexBuffer()
{}

// -----------------------------------------------------------------------------------------------
const std::vector< Vertex > &VertexBuffer::vertices() const
{
	return _vertices;
}

// -----------------------------------------------------------------------------------------------
Vertex &VertexBuffer::vertex( int i )
{
#ifndef NDEBUG
	assert( i >=0 && (size_t)i<_vertices.size() );
#endif

	return _vertices[i];
}

// -----------------------------------------------------------------------------------------------
int VertexBuffer::size() const
{
	return _vertices.size();
}

// -----------------------------------------------------------------------------------------------
bool VertexBuffer::hasIndexBuffer() const
{
	return false;
}

// -----------------------------------------------------------------------------------------------
VertexBuffer &VertexBuffer::operator << ( const Vertex &v )
{
	_vertices.push_back(v);
	return *this;
}

// -----------------------------------------------------------------------------------------------
VertexBuffer &VertexBuffer::operator << ( const VertexBuffer &vb )
{
	_vertices.insert( _vertices.end(), vb._vertices.begin(), vb._vertices.end() );
	return *this;
}


} // namespace OpenGL