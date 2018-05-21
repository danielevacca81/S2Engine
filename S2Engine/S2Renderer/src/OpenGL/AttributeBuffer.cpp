// AttributeBuffer.cpp
// 
#include "AttributeBuffer.h"

#include "OpenGL/OpenGLWrap.h"

using namespace OpenGL;


static int dataTypeSize( AttributeBuffer::ComponentDatatype type )
{
	switch( type )
	{
	case AttributeBuffer::Byte:
	case AttributeBuffer::UnsignedByte:
		return sizeof(char);
	
	case AttributeBuffer::Short:         return sizeof(short);
	case AttributeBuffer::UnsignedShort: return sizeof(unsigned short);
	case AttributeBuffer::Int:           return sizeof(int);
	case AttributeBuffer::UnsignedInt:   return sizeof(unsigned int);
	case AttributeBuffer::Float:         return sizeof(float);
	case AttributeBuffer::HalfFloat:     return 0; //  TBD
	}

	return sizeof(char);
}

//-------------------------------------------------------------------------------------------------
AttributeBuffer::AttributeBuffer()
: _valid(false)
, _location(-1)
{}

//-------------------------------------------------------------------------------------------------
AttributeBuffer::~AttributeBuffer()
{}

//-------------------------------------------------------------------------------------------------
void AttributeBuffer::set( const VertexBuffer &buffer, ComponentDatatype componentDatatype, int numberOfComponents,  bool normalize, int offset, int stride )
{
	_vertexBuffer       = buffer;
	_componentDatatype  = componentDatatype;
	_numberOfComponents = numberOfComponents;
	_offset             = offset;
	_normalize          = normalize;
	_valid              = true;
	_stride             = 
		stride == 0 ? 
		numberOfComponents * dataTypeSize(componentDatatype) : // tightly packed
		stride;
}

//-------------------------------------------------------------------------------------------------
void AttributeBuffer::attach( int loc )
{
	if( !_valid )
		return; // assert?

	glEnableVertexAttribArray( loc );

	_vertexBuffer.bind();
	glVertexAttribPointer( loc, 
		                   _numberOfComponents, 
						   glWrap(_componentDatatype),
						   _normalize,
						   _stride,               // stride in bytes
						   BUFFER_OFFSET(_offset) // offset in bytes
						   );
						   
	_location = loc;

	glCheck;
}

//-------------------------------------------------------------------------------------------------
void AttributeBuffer::detach()
{
	if( _location >0 )
		glDisableVertexAttribArray(_location);
}