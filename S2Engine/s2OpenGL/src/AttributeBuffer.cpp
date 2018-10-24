// AttributeBuffer.cpp
// 
#include "AttributeBuffer.h"

#include "OpenGL.h"
#include "OpenGLWrap.h"

using namespace s2;

using namespace s2::OpenGL;


//-------------------------------------------------------------------------------------------------
static int dataTypeSize( const AttributeBuffer::ComponentDatatype &type )
{
	switch( type )
	{
	case AttributeBuffer::Byte:
	case AttributeBuffer::UnsignedByte:
		return sizeof( int8_t );

	case AttributeBuffer::Short:         return sizeof( int16_t );
	case AttributeBuffer::UnsignedShort: return sizeof( uint16_t );
	case AttributeBuffer::Int:           return sizeof( int32_t );
	case AttributeBuffer::UnsignedInt:   return sizeof( uint32_t );
	case AttributeBuffer::Float:         return sizeof( float_t );
	case AttributeBuffer::HalfFloat:     return 0; //  TBD
	}

	return sizeof( int8_t );
}

//-------------------------------------------------------------------------------------------------
AttributeBuffer::AttributeBuffer()
: _valid( false )
, _location( -1 )
, _normalize( false )
, _numberOfComponents( 0 )
, _offset( 0 )
, _stride( 0 )
, _componentDatatype( Byte )
{}

//-------------------------------------------------------------------------------------------------
AttributeBuffer::AttributeBuffer( AttributeBuffer &&other )
{
	std::swap( _valid,             other._valid              );
	std::swap( _location,          other._location           );
	std::swap( _normalize,         other._normalize          );
	std::swap( _numberOfComponents,other._numberOfComponents );
	std::swap( _offset,            other._offset             );
	std::swap( _stride,            other._stride             );
	std::swap( _componentDatatype, other._componentDatatype  );
	std::swap( _vertexBuffer,      other._vertexBuffer       );
}

//-------------------------------------------------------------------------------------------------
AttributeBuffer::~AttributeBuffer()
{}

//-------------------------------------------------------------------------------------------------
AttributeBuffer &AttributeBuffer::operator=( AttributeBuffer &&other )
{
	std::swap( _valid,             other._valid              );
	std::swap( _location,          other._location           );
	std::swap( _normalize,         other._normalize          );
	std::swap( _numberOfComponents,other._numberOfComponents );
	std::swap( _offset,            other._offset             );
	std::swap( _stride,            other._stride             );
	std::swap( _componentDatatype, other._componentDatatype  );
	std::swap( _vertexBuffer,      other._vertexBuffer       );
	return *this;
}


//-------------------------------------------------------------------------------------------------
void AttributeBuffer::set( VertexBuffer &&buffer, const ComponentDatatype &componentDatatype, int numberOfComponents, bool normalize, int offset, int stride )
{
	_vertexBuffer       = std::move( buffer );
	_componentDatatype  = componentDatatype;
	_numberOfComponents = numberOfComponents;
	_offset             = offset;
	_normalize          = normalize;
	_valid              = true;
	_stride             =
		stride == 0 ?
		numberOfComponents * dataTypeSize( componentDatatype ) : // tightly packed
		stride;
}

//-------------------------------------------------------------------------------------------------
void AttributeBuffer::attach( int loc )
{
	if( !_valid )
		return; // assert?

	glEnableVertexAttribArray( loc );
	glCheck;

	_vertexBuffer.bind();
	glVertexAttribPointer( loc,
						   _numberOfComponents,
						   glWrap( _componentDatatype ),
						   _normalize,
						   _stride,               // stride in bytes
						   BUFFER_OFFSET( _offset ) // offset in bytes
	);
	glCheck;

	_location = loc;
}

//-------------------------------------------------------------------------------------------------
void AttributeBuffer::detach()
{
	if( _location > 0 )
		glDisableVertexAttribArray( _location );
	glCheck;
}