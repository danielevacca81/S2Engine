// VertexArray.cpp
// 
#include "VertexArray.h"

#include "Math/Math.h"

#include "Device.h"
#include "OpenGL.h"
#include "OpenGLWrap.h"
#include "OpenGLCheck.h"

#include <iostream>

using namespace RenderSystem;


// -------------------------------------------------------------------------------------------------
VertexArrayPtr VertexArray::New( const BufferObject::UsageHint &hint )
{
	return std::make_shared<VertexArray>(hint);
}


// -------------------------------------------------------------------------------------------------
VertexArray::VertexArray( const BufferObject::UsageHint &hint )
: _usageHint( hint )
{
	create();
}

// -------------------------------------------------------------------------------------------------
//VertexArray::VertexArray( VertexArray &&other )
//: VertexArray()
//{
//	std::swap( _attributes , other._attributes  );
//	std::swap( _indexBuffer, other._indexBuffer );
//
//	std::swap( _created,   other._created );
//	std::swap( _objectID,  other._objectID);
//}

// -------------------------------------------------------------------------------------------------
VertexArray::~VertexArray()
{
	destroy();
}

// -------------------------------------------------------------------------------------------------
//VertexArray &VertexArray::operator=( VertexArray &&other )
//{
//	reset();
//
//	std::swap( _attributes , other._attributes  );
//	std::swap( _indexBuffer, other._indexBuffer );
//
//	std::swap( _created,   other._created );
//	std::swap( _objectID,  other._objectID);
//	return *this;
//}

// -------------------------------------------------------------------------------------------------
void VertexArray::reset()
{
	OpenGLObject::reset();
	_attributes.clear();
	_indexBuffer = IndexBuffer( 0, IndexBuffer::IndexDataType::UnsignedInt, _usageHint );
}

// -------------------------------------------------------------------------------------------------
void VertexArray::create()
{
	destroy();
	OpenGLObject::create();

	const auto maxVertexAttrib = Device::maxAttribPerVertex();

	_attributes.resize ( maxVertexAttrib );
	_attributes.reserve( maxVertexAttrib );

	
	glGenVertexArrays( 1, &_objectID );
	glCheck;
	
	_created = _objectID != 0;
}

// -------------------------------------------------------------------------------------------------
void VertexArray::destroy()
{
	if( !isCreated() )
		return;
	
	glDeleteVertexArrays( 1, &_objectID );
	glCheck;
	reset();
}

// -------------------------------------------------------------------------------------------------
void VertexArray::bind() const
{
	if( !isCreated() )
		return;
	
	assert( ( "VAO name not valid", _objectID != 0) );

	
	glBindVertexArray( _objectID );
	while( glGetError() != GL_NO_ERROR )
		std::cout << "Invalid VAO name" << _objectID << '\n';
	glCheck;

	if( _indexBuffer.isValid() )
		_indexBuffer.bind();

	for( size_t i = 0; i< _attributes.size(); ++i )
		_attributes[i].attach( i );

	glCheck;
}

// -------------------------------------------------------------------------------------------------
void VertexArray::unbind() const
{
	
	glBindVertexArray( 0 );
	glCheck;
}

// -------------------------------------------------------------------------------------------------
AttributeBuffer &VertexArray::attribute( int i )
{
	assert( i>=0 && i<Device::maxAttribPerVertex() );
	return _attributes[i];
}

// -------------------------------------------------------------------------------------------------
AttributeBuffer const &VertexArray::attribute(int i) const
{
	assert( i >= 0 && i < Device::maxAttribPerVertex() );
	return _attributes[i];
}

// -------------------------------------------------------------------------------------------------
IndexBuffer     &VertexArray::indexBuffer() 
{
	return _indexBuffer;
}

// -------------------------------------------------------------------------------------------------
IndexBuffer     const &VertexArray::indexBuffer() const
{
	return _indexBuffer;
}

// -------------------------------------------------------------------------------------------------
bool VertexArray::isIndexed() const
{
	return _indexBuffer.isValid();
}

// -------------------------------------------------------------------------------------------------
int VertexArray::maxArrayIndex() const
{
	int maximumArrayIndex = 0;

	for( size_t i = 0; i < _attributes.size(); ++i )
		if( _attributes[i].isValid() )
			maximumArrayIndex = Math::max( _attributes[i].numberOfVertices() - 1, maximumArrayIndex);

	return maximumArrayIndex;
}