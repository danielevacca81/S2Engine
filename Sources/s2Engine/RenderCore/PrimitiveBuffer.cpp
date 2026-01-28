// PrimitiveBuffer.cpp
//
#include "PrimitiveBuffer.h"

#include "VertexAttributeLocation.h"


using namespace RenderCore;


// ------------------------------------------------------------------------------------------------
PrimitiveBufferPtr PrimitiveBuffer::New( const BufferObject::UsageHint &hint )                                        { return std::make_shared<PrimitiveBuffer>( hint ); }
PrimitiveBufferPtr PrimitiveBuffer::New( const std::vector<Math::vec3>& points, const BufferObject::UsageHint &hint ) { return std::make_shared<PrimitiveBuffer>( points,hint ); }


// ------------------------------------------------------------------------------------------------
// PrimitiveBuffer Attribute location:
//	 (0) vertices
//   (1) colors
//   (2) normals
//   (3) texture coords
// ------------------------------------------------------------------------------------------------
PrimitiveBuffer::PrimitiveBuffer( const BufferObject::UsageHint &hint )
{
	_vao = VertexArray::New( hint );
}

// ------------------------------------------------------------------------------------------------
PrimitiveBuffer::PrimitiveBuffer( const std::vector<Math::vec3>& points, const BufferObject::UsageHint& hint )
{
	_vao = VertexArray::New( hint );
	setVertices( points );
}

// ------------------------------------------------------------------------------------------------
void PrimitiveBuffer::setVertices( const std::vector<Math::vec3> &points )
{
	const uint32_t numPoints = static_cast<uint32_t>( points.size() );

	if( numPoints == 0 )
		return;

	VertexBuffer buf( numPoints * sizeof( Math::vec3 ), _vao->usageHint() );
	buf.sendData( (void*)(&points[0]), numPoints * sizeof(Math::vec3), 0 );

	_vao->attribute( VertexAttributeLocation::VA_Position ).set( buf, AttributeBuffer::Float, 3, false, 0, 0 );
}

// ------------------------------------------------------------------------------------------------
void PrimitiveBuffer::setVertices( const std::vector<Math::vec2>& points2D )
{
	const uint32_t numPoints = static_cast<uint32_t>( points2D.size() );

	if( numPoints == 0 )
		return;

	VertexBuffer buf( numPoints * sizeof( Math::vec2 ), _vao->usageHint() );
	buf.sendData( (void*) ( &points2D[0] ), numPoints * sizeof( Math::vec2 ), 0 );

	_vao->attribute( VertexAttributeLocation::VA_Position ).set( buf, AttributeBuffer::Float, 2, false, 0, 0 );
}

// ------------------------------------------------------------------------------------------------
void PrimitiveBuffer::setColors( const std::vector<Color>   &colors )
{
	const uint32_t numColors = static_cast<uint32_t>( colors.size() );

	if( numColors == 0 )
		return;

	VertexBuffer buf( numColors * sizeof( Color ), _vao->usageHint() );
	buf.sendData( (void*)(&colors[0]), numColors * sizeof(Color), 0 );

	_vao->attribute( VertexAttributeLocation::VA_Color ).set( std::move( buf ), AttributeBuffer::Float, 4, false, 0, 0 );
}

// ------------------------------------------------------------------------------------------------
void PrimitiveBuffer::setNormals( const std::vector<Math::vec3>  &normals )
{
	const uint32_t numNormals = static_cast<uint32_t>( normals.size() );

	if( numNormals == 0 )
		return;

	VertexBuffer buf( numNormals * sizeof( Math::vec3 ), _vao->usageHint() );
	buf.sendData( (void*)(&normals[0]), numNormals * sizeof(Math::vec3), 0 );

	_vao->attribute( VertexAttributeLocation::VA_Normal ).set( std::move( buf ), AttributeBuffer::Float, 3, false, 0, 0 );
}

// ------------------------------------------------------------------------------------------------
void PrimitiveBuffer::setTextureCoords( const std::vector<Math::vec2>  &texCoords )
{
	const uint32_t numTexCoords = static_cast<uint32_t>( texCoords.size() );

	if( numTexCoords == 0 )
		return;

	VertexBuffer buf( numTexCoords * sizeof( Math::vec2 ), _vao->usageHint() );
	buf.sendData( (void*)(&texCoords[0]), numTexCoords * sizeof(Math::vec2), 0 );

	_vao->attribute( VertexAttributeLocation::VA_UVCoords ).set( std::move( buf ), AttributeBuffer::Float, 2, false, 0, 0 );
}

// ------------------------------------------------------------------------------------------------
void PrimitiveBuffer::setIntAttribute( const std::vector<uint32_t> &attrib )
{
	const uint32_t numAttrib = static_cast<uint32_t>( attrib.size() );

	if( numAttrib == 0 )
		return;

	VertexBuffer buf( numAttrib * sizeof( uint32_t ), _vao->usageHint() );
	buf.sendData( (void*)(&attrib[0]), numAttrib * sizeof(uint32_t), 0 );

	_vao->attribute( VertexAttributeLocation::VA_CustomInt ).set( std::move( buf ), AttributeBuffer::UnsignedInt, 1, false, 0, 0 );
}

// ------------------------------------------------------------------------------------------------
void PrimitiveBuffer::setIntVector2Attribute( const std::vector<Math::ivec2>  &attrib )
{
	const uint32_t numAttrib = static_cast<uint32_t>( attrib.size() );

	if( numAttrib == 0 )
		return;

	VertexBuffer buf( numAttrib * sizeof( Math::ivec2 ), _vao->usageHint() );
	buf.sendData( (void*)(&attrib[0]), numAttrib * sizeof( Math::ivec2 ), 0 );

	_vao->attribute( VertexAttributeLocation::VA_CustomIntVector2 ).set( std::move( buf ), AttributeBuffer::UnsignedInt, 2, false, 0, 0 );
}

// ------------------------------------------------------------------------------------------------
void PrimitiveBuffer::setIntVector3Attribute( const std::vector<Math::ivec3>  &attrib )
{
	const uint32_t numAttrib = static_cast<uint32_t>( attrib.size() );

	if( numAttrib == 0 )
		return;

	VertexBuffer buf( numAttrib * sizeof( Math::ivec3 ), _vao->usageHint() );
	buf.sendData( (void*)(&attrib[0]), numAttrib * sizeof( Math::ivec3 ), 0 );

	_vao->attribute( VertexAttributeLocation::VA_CustomIntVector3 ).set( std::move( buf ), AttributeBuffer::UnsignedInt, 3, false, 0, 0 );
}

// ------------------------------------------------------------------------------------------------
void PrimitiveBuffer::setIntVector4Attribute( const std::vector<Math::ivec4>  &attrib )
{
	const uint32_t numAttrib = static_cast<uint32_t>( attrib.size() );

	if( numAttrib == 0 )
		return;

	VertexBuffer buf( numAttrib * sizeof( Math::ivec4 ), _vao->usageHint() );
	buf.sendData( (void*)(&attrib[0]), numAttrib * sizeof( Math::ivec4 ), 0 );

	_vao->attribute( VertexAttributeLocation::VA_CustomIntVector4 ).set( std::move( buf ), AttributeBuffer::UnsignedInt, 4, false, 0, 0 );
}

// ------------------------------------------------------------------------------------------------
void PrimitiveBuffer::setFloatAttribute( const std::vector<float> &attrib )
{
	const uint32_t numAttrib = static_cast<uint32_t>( attrib.size() );

	if( numAttrib == 0 )
		return;

	VertexBuffer buf( numAttrib * sizeof( float ), _vao->usageHint() );
	buf.sendData( (void*)(&attrib[0]), numAttrib * sizeof(float), 0 );

	_vao->attribute( VertexAttributeLocation::VA_CustomFloat ).set( std::move( buf ), AttributeBuffer::Float, 1, false, 0, 0 );
}

// ------------------------------------------------------------------------------------------------
void PrimitiveBuffer::setFloatVector2Attribute( const std::vector<Math::fvec2>  &attrib )
{
	const uint32_t numAttrib = static_cast<uint32_t>( attrib.size() );

	if( numAttrib == 0 )
		return;

	VertexBuffer buf( numAttrib * sizeof( Math::fvec2 ), _vao->usageHint() );
	buf.sendData( (void*)(&attrib[0]), numAttrib * sizeof( Math::fvec2 ), 0 );

	_vao->attribute( VertexAttributeLocation::VA_CustomFloatVector2 ).set( std::move( buf ), AttributeBuffer::Float, 2, false, 0, 0 );
}

// ------------------------------------------------------------------------------------------------
void PrimitiveBuffer::setFloatVector3Attribute( const std::vector<Math::fvec3>  &attrib )
{
	const uint32_t numAttrib = static_cast<uint32_t>( attrib.size() );

	if( numAttrib == 0 )
		return;

	VertexBuffer buf( numAttrib * sizeof( Math::fvec3 ), _vao->usageHint() );
	buf.sendData( (void*)(&attrib[0]), numAttrib * sizeof( Math::fvec3 ), 0 );

	_vao->attribute( VertexAttributeLocation::VA_CustomFloatVector3 ).set( std::move( buf ), AttributeBuffer::Float, 3, false, 0, 0 );
}

// ------------------------------------------------------------------------------------------------
void PrimitiveBuffer::setFloatVector4Attribute( const std::vector<Math::fvec4>  &attrib )
{
	const uint32_t numAttrib = static_cast<uint32_t>( attrib.size() );

	if( numAttrib == 0 )
		return;

	VertexBuffer buf( numAttrib * sizeof( Math::fvec4 ), _vao->usageHint() );
	buf.sendData( (void*)(&attrib[0]), numAttrib * sizeof( Math::fvec4 ), 0 );

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
	const uint32_t numIndices = static_cast<uint32_t>( indices.size() );

	if( numIndices == 0 )
		return;

	_vao->indexBuffer().set( numIndices * sizeof( unsigned int ), IndexBuffer::UnsignedInt, _vao->usageHint() );
	_vao->indexBuffer().sendData( (void*)(&indices[0]), numIndices * sizeof( unsigned int ), 0 );
}