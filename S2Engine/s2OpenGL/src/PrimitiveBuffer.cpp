// PrimitiveBuffer.cpp
//
#include "PrimitiveBuffer.h"

#include "Primitive.h"
#include "Locations.h"

using namespace s2;
using namespace s2::OpenGL;


// ------------------------------------------------------------------------------------------------
PrimitiveBufferPtr PrimitiveBuffer::makeNew()
{
	return std::make_shared<PrimitiveBuffer>();
}


// ------------------------------------------------------------------------------------------------
// PrimitiveBuffer Attribute location:
//	 (0) vertices
//   (1) colors
//   (2) normals
//   (3) texture coords
// ------------------------------------------------------------------------------------------------
PrimitiveBuffer::PrimitiveBuffer()
: primitiveType( Primitive::Points )
, frontFaceWindingOrder( WindingOrder::Counterclockwise )
, usageHint( BufferObject::UsageHint::StaticDraw )
{
	_va = VertexArray::makeNew();
}

// ------------------------------------------------------------------------------------------------
PrimitiveBuffer::~PrimitiveBuffer()
{}

// ------------------------------------------------------------------------------------------------
void PrimitiveBuffer::setVertices( const std::vector<Math::vec3> &points )
{
	VertexBuffer buf( points.size() * sizeof( Math::vec3 ),usageHint );
	buf.sendData( (void*)(&points[0]), points.size() * sizeof(Math::vec3), 0 );

	_va->attribute( 0 ).set( buf, AttributeBuffer::Float, 3, false, 0, 0 );
}

// ------------------------------------------------------------------------------------------------
void PrimitiveBuffer::setColors( const std::vector<Color>   &colors )
{
	VertexBuffer buf( colors.size() * sizeof( Color ),usageHint );
	buf.sendData( (void*)(&colors[0]), colors.size() * sizeof(Color), 0 );

	_va->attribute( 1 ).set( std::move( buf ), AttributeBuffer::Float, 4, false, 0, 0 );
}

// ------------------------------------------------------------------------------------------------
void PrimitiveBuffer::setNormals( const std::vector<Math::vec3>  &normals )
{
	VertexBuffer buf( normals.size() * sizeof( Math::vec3 ), usageHint );
	buf.sendData( (void*)(&normals[0]), normals.size() * sizeof(Math::vec3), 0 );

	_va->attribute( 2 ).set( std::move( buf ), AttributeBuffer::Float, 3, false, 0, 0 );
}

// ------------------------------------------------------------------------------------------------
void PrimitiveBuffer::setTextureCoords( const std::vector<Math::vec2>  &texCoords )
{
	VertexBuffer buf( texCoords.size() * sizeof( Math::vec2 ),usageHint );
	buf.sendData( (void*)(&texCoords[0]), texCoords.size() * sizeof(Math::vec2), 0 );

	_va->attribute( 3 ).set( std::move( buf ), AttributeBuffer::Float, 2, false, 0, 0 );
}

// ------------------------------------------------------------------------------------------------
//void PrimitiveBuffer::setAttribute( int attribID, void *data, int elementsCount, AttributeBuffer::ComponentDatatype dataType, int componentsCount )
//{
// @tbd (
//	VertexBuffer vb;
//	vb.set( elementsCount * dataType * componentsCount, BufferObject::StaticDraw );
//	vb.sendData( data, elementsCount * dataType * componentsCount, 0 );
//
//	_va.attribute(attribID)->set( vb, (void*)(&pts[0]), pts.size(),    OpenGL::AttributeBuffer::Float, 3
//}

// ------------------------------------------------------------------------------------------------
void PrimitiveBuffer::setIndices( const std::vector<unsigned int> &indices )
{
	_va->indexBuffer().set( indices.size() * sizeof( unsigned int ), IndexBuffer::UnsignedInt, usageHint );
	_va->indexBuffer().sendData( (void*)(&indices[0]), indices.size() * sizeof( unsigned int ), 0 );
}