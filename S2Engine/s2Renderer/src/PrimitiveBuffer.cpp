// PrimitiveBuffer.cpp
//
#include "PrimitiveBuffer.h"

#include "PrimitiveType.h"
#include "VertexAttributeLocation.h"


using namespace Renderer;


// ------------------------------------------------------------------------------------------------
PrimitiveBufferPtr PrimitiveBuffer::New( const BufferObject::UsageHint &hint )
{
	return std::make_shared<PrimitiveBuffer>( hint );
}


// ------------------------------------------------------------------------------------------------
// PrimitiveBuffer Attribute location:
//	 (0) vertices
//   (1) colors
//   (2) normals
//   (3) texture coords
// ------------------------------------------------------------------------------------------------
PrimitiveBuffer::PrimitiveBuffer( const BufferObject::UsageHint &hint )
//: primitiveType( PrimitiveType::Points )
//, frontFaceWindingOrder( WindingOrder::Counterclockwise )
{
	_vao = VertexArray::New( hint );
}

// ------------------------------------------------------------------------------------------------
void PrimitiveBuffer::setVertices( const std::vector<Math::vec3> &points )
{
	if( points.empty() )
		return;

	VertexBuffer buf( points.size() * sizeof( Math::vec3 ), _vao->usageHint() );
	buf.sendData( (void*)(&points[0]), points.size() * sizeof(Math::vec3), 0 );

	_vao->attribute( VertexAttributeLocation::VA_Position ).set( buf, AttributeBuffer::Float, 3, false, 0, 0 );
}

// ------------------------------------------------------------------------------------------------
void PrimitiveBuffer::setColors( const std::vector<Color>   &colors )
{
	if( colors.empty() )
		return;

	VertexBuffer buf( colors.size() * sizeof( Color ), _vao->usageHint() );
	buf.sendData( (void*)(&colors[0]), colors.size() * sizeof(Color), 0 );

	_vao->attribute( VertexAttributeLocation::VA_Color ).set( std::move( buf ), AttributeBuffer::Float, 4, false, 0, 0 );
}

// ------------------------------------------------------------------------------------------------
void PrimitiveBuffer::setNormals( const std::vector<Math::vec3>  &normals )
{
	if( normals.empty() )
		return;

	VertexBuffer buf( normals.size() * sizeof( Math::vec3 ), _vao->usageHint() );
	buf.sendData( (void*)(&normals[0]), normals.size() * sizeof(Math::vec3), 0 );

	_vao->attribute( VertexAttributeLocation::VA_Normal ).set( std::move( buf ), AttributeBuffer::Float, 3, false, 0, 0 );
}

// ------------------------------------------------------------------------------------------------
void PrimitiveBuffer::setTextureCoords( const std::vector<Math::vec2>  &texCoords )
{
	if( texCoords.empty() )
		return;

	VertexBuffer buf( texCoords.size() * sizeof( Math::vec2 ), _vao->usageHint() );
	buf.sendData( (void*)(&texCoords[0]), texCoords.size() * sizeof(Math::vec2), 0 );

	_vao->attribute( VertexAttributeLocation::VA_UVCoords ).set( std::move( buf ), AttributeBuffer::Float, 2, false, 0, 0 );
}

// ------------------------------------------------------------------------------------------------
void PrimitiveBuffer::setIntAttribute( const std::vector<uint32_t> &attrib )
{
	if( attrib.empty() )
		return;

	VertexBuffer buf( attrib.size() * sizeof( uint32_t ), _vao->usageHint() );
	buf.sendData( (void*)(&attrib[0]), attrib.size() * sizeof(uint32_t), 0 );

	_vao->attribute( VertexAttributeLocation::VA_CustomInt ).set( std::move( buf ), AttributeBuffer::UnsignedInt, 1, false, 0, 0 );
}

// ------------------------------------------------------------------------------------------------
void PrimitiveBuffer::setIntVector2Attribute( const std::vector<Math::ivec2>  &attrib )
{
	if( attrib.empty() )
		return;

	VertexBuffer buf( attrib.size() * sizeof( Math::ivec2 ), _vao->usageHint() );
	buf.sendData( (void*)(&attrib[0]), attrib.size() * sizeof( Math::ivec2 ), 0 );

	_vao->attribute( VertexAttributeLocation::VA_CustomIntVector2 ).set( std::move( buf ), AttributeBuffer::UnsignedInt, 2, false, 0, 0 );
}

// ------------------------------------------------------------------------------------------------
void PrimitiveBuffer::setIntVector3Attribute( const std::vector<Math::ivec3>  &attrib )
{
	if( attrib.empty() )
		return;

	VertexBuffer buf( attrib.size() * sizeof( Math::ivec3 ), _vao->usageHint() );
	buf.sendData( (void*)(&attrib[0]), attrib.size() * sizeof( Math::ivec3 ), 0 );

	_vao->attribute( VertexAttributeLocation::VA_CustomIntVector3 ).set( std::move( buf ), AttributeBuffer::UnsignedInt, 3, false, 0, 0 );
}

// ------------------------------------------------------------------------------------------------
void PrimitiveBuffer::setIntVector4Attribute( const std::vector<Math::ivec4>  &attrib )
{
	if( attrib.empty() )
	return;

	VertexBuffer buf( attrib.size() * sizeof( Math::ivec4 ), _vao->usageHint() );
	buf.sendData( (void*)(&attrib[0]), attrib.size() * sizeof( Math::ivec4 ), 0 );

	_vao->attribute( VertexAttributeLocation::VA_CustomIntVector4 ).set( std::move( buf ), AttributeBuffer::UnsignedInt, 4, false, 0, 0 );
}

// ------------------------------------------------------------------------------------------------
void PrimitiveBuffer::setFloatAttribute( const std::vector<float> &attrib )
{
	if( attrib.empty() )
		return;

	VertexBuffer buf( attrib.size() * sizeof( float ), _vao->usageHint() );
	buf.sendData( (void*)(&attrib[0]), attrib.size() * sizeof(float), 0 );

	_vao->attribute( VertexAttributeLocation::VA_CustomFloat ).set( std::move( buf ), AttributeBuffer::Float, 1, false, 0, 0 );
}

// ------------------------------------------------------------------------------------------------
void PrimitiveBuffer::setFloatVector2Attribute( const std::vector<Math::fvec2>  &attrib )
{
	if( attrib.empty() )
		return;

	VertexBuffer buf( attrib.size() * sizeof( Math::fvec2 ), _vao->usageHint() );
	buf.sendData( (void*)(&attrib[0]), attrib.size() * sizeof( Math::fvec2 ), 0 );

	_vao->attribute( VertexAttributeLocation::VA_CustomFloatVector2 ).set( std::move( buf ), AttributeBuffer::Float, 2, false, 0, 0 );
}

// ------------------------------------------------------------------------------------------------
void PrimitiveBuffer::setFloatVector3Attribute( const std::vector<Math::fvec3>  &attrib )
{
	if( attrib.empty() )
		return;

	VertexBuffer buf( attrib.size() * sizeof( Math::fvec3 ), _vao->usageHint() );
	buf.sendData( (void*)(&attrib[0]), attrib.size() * sizeof( Math::fvec3 ), 0 );

	_vao->attribute( VertexAttributeLocation::VA_CustomFloatVector3 ).set( std::move( buf ), AttributeBuffer::Float, 3, false, 0, 0 );
}

// ------------------------------------------------------------------------------------------------
void PrimitiveBuffer::setFloatVector4Attribute( const std::vector<Math::fvec4>  &attrib )
{
	if( attrib.empty() )
		return;

	VertexBuffer buf( attrib.size() * sizeof( Math::fvec4 ), _vao->usageHint() );
	buf.sendData( (void*)(&attrib[0]), attrib.size() * sizeof( Math::fvec4 ), 0 );

	_vao->attribute( VertexAttributeLocation::VA_CustomFloatVector4 ).set( std::move( buf ), AttributeBuffer::Float, 4, false, 0, 0 );
}


// ------------------------------------------------------------------------------------------------
//void PrimitiveBuffer::setAttribute( int attribID, void *data, int elementsCount, AttributeBuffer::ComponentDatatype dataType, int componentsCount )
//{
// @tbd (
//	VertexBuffer vb;
//	vb.set( elementsCount * dataType * componentsCount, BufferObject::StaticDraw );
//	vb.sendData( data, elementsCount * dataType * componentsCount, 0 );
//
//	_vao.attribute(attribID)->set( vb, (void*)(&pts[0]), pts.size(),    OpenGL::AttributeBuffer::Float, 3
//}

// ------------------------------------------------------------------------------------------------
void PrimitiveBuffer::setIndices( const std::vector<unsigned int> &indices )
{
	if( indices.empty() )
		return;

	_vao->indexBuffer().set( indices.size() * sizeof( unsigned int ), IndexBuffer::UnsignedInt, _vao->usageHint() );
	_vao->indexBuffer().sendData( (void*)(&indices[0]), indices.size() * sizeof( unsigned int ), 0 );
}