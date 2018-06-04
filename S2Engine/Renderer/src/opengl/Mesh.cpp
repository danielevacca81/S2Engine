// Mesh.cpp
//
#include "Mesh.h"

using namespace OpenGL;


// ------------------------------------------------------------------------------------------------
Mesh::Mesh()
: primitiveType(Points)
, frontFaceWindingOrder(Counterclockwise)
, usageHint(BufferObject::StaticDraw)
{}

// ------------------------------------------------------------------------------------------------
Mesh::~Mesh()
{}

// ------------------------------------------------------------------------------------------------
void Mesh::setVertices( const std::vector<Math::vec3> &points )
{
	VertexBuffer buf( points.size() * sizeof(Math::vec3), usageHint );
	buf.sendData( (void*)(&points[0]), points.size() * sizeof(Math::vec3), 0 );

	_va.attribute(0)->set( buf, AttributeBuffer::Float,3, false,0,0 );
}

// ------------------------------------------------------------------------------------------------
void Mesh::setColors( const std::vector<Color>   &colors   )
{
	VertexBuffer buf( colors.size() * sizeof(Color), usageHint );
	buf.sendData( (void*)(&colors[0]), colors.size() * sizeof(Color), 0 );

	_va.attribute(1)->set( buf, AttributeBuffer::Float,4, false,0,0 );
}

// ------------------------------------------------------------------------------------------------
void Mesh::setNormals( const std::vector<Math::vec3>  &normals  )
{
	VertexBuffer buf( normals.size() * sizeof(Math::vec3), usageHint );
	buf.sendData( (void*)(&normals[0]), normals.size() * sizeof(Math::vec3), 0 );

	_va.attribute(2)->set( buf, AttributeBuffer::Float,3, false,0,0 );
}

// ------------------------------------------------------------------------------------------------
void Mesh::setTextureCoords( const std::vector<Math::vec2>  &texCoords)
{
	VertexBuffer buf( texCoords.size() * sizeof(Math::vec2), usageHint );
	buf.sendData( (void*)(&texCoords[0]), texCoords.size() * sizeof(Math::vec2), 0 );

	_va.attribute(3)->set( buf, AttributeBuffer::Float,2, false,0,0 );
}

// ------------------------------------------------------------------------------------------------
//void Mesh::setAttribute( int attribID, void *data, int elementsCount, AttributeBuffer::ComponentDatatype dataType, int componentsCount )
//{
//	VertexBuffer vb;
//	vb.set( elementsCount * dataType * componentsCount, BufferObject::StaticDraw );
//	vb.sendData( data, elementsCount * dataType * componentsCount, 0 );
//
//	_va.attribute(attribID)->set( vb, (void*)(&pts[0]), pts.size(),    OpenGL::AttributeBuffer::Float, 3
//}


// ------------------------------------------------------------------------------------------------
void Mesh::setIndices( const std::vector<int> &indices )
{
	// @tbd
	//_va.indexBuffer()->s
}
