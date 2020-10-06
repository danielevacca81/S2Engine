// VertexArray.cpp
// 
#include "VertexArray.h"

#include "Core/Math.h"

#include "Device.h"
#include "OpenGL.h"
#include "OpenGLWrap.h"

#include <iostream>

using namespace Renderer;

static int maxVertexAttrib = 16;

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
bool VertexArray::create()
{
	//maxVertexAttrib = Device::get().maxAttribPerVertex();

	_attributes.resize ( maxVertexAttrib );
	_attributes.reserve( maxVertexAttrib );

	storeGlContext();
	glGenVertexArrays( 1, &_objectID );
	glCheck;
	
	//bind();

	//if( _indexBuffer.isValid() )
	//	_indexBuffer.bind();

	//for( size_t i=0; i < _attributes.size(); ++i )
	//	_attributes[i].attach( i );
	//
	//unbind();

	_created = true;
	return _created;
}

// -------------------------------------------------------------------------------------------------
void VertexArray::destroy()
{
	checkGlContext();
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

	checkGlContext();
	glBindVertexArray( _objectID );
	if( glGetError() != 0 )
		std::cout << "Invalid VAO name" << '\n';
	glCheck;

	if( _indexBuffer.isValid() )
		_indexBuffer.bind();

	for( size_t i=0; i < _attributes.size(); ++i )
		_attributes[i].attach( i );

	glCheck;
}

// -------------------------------------------------------------------------------------------------
void VertexArray::unbind() const
{
	checkGlContext();
	glBindVertexArray( 0 );
	glCheck;
}

// -------------------------------------------------------------------------------------------------
AttributeBuffer &VertexArray::attribute( int i )
{
#ifndef _NDEBUG
	assert( i>=0 && i<maxVertexAttrib );
#endif
	return _attributes[i];
}

// -------------------------------------------------------------------------------------------------
AttributeBuffer const &VertexArray::attribute(int i) const
{
#ifndef _NDEBUG
	assert( i >= 0 && i < maxVertexAttrib );
#endif
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