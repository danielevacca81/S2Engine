// BufferObject.cpp
// 
#include "BufferObject.h"

#include "OpenGL.h"
#include "OpenGLWrap.h"
#include "OpenGLCheck.h"

using namespace RenderSystem;

// -------------------------------------------------------------------------------------------------
BufferObjectPtr BufferObject::New( int size, const Type &type, const UsageHint &usageHint )
{
	return std::make_shared<BufferObject>( size, type, usageHint );
}

// -------------------------------------------------------------------------------------------------
//BufferObject::BufferObject()
//: BufferObject( 0, Type::ArrayBuffer, UsageHint::StaticDraw )
//{}

// -------------------------------------------------------------------------------------------------
BufferObject::BufferObject( int size, const Type &type, const UsageHint &usageHint )
: _size( size )
, _type( type )
, _usageHint( usageHint )
{
	//if (sizeInBytes <= 0)
	//{
	//	throw new ArgumentOutOfRangeException("sizeInBytes", "sizeInBytes must be greater than zero.");
	//}
	create();
}

// -------------------------------------------------------------------------------------------------
//BufferObject::BufferObject( BufferObject &&other )
//: BufferObject()
//{
//	std::swap( _usageHint, other._usageHint );
//	std::swap( _size,      other._size );
//	std::swap( _type,      other._type );
//	
//	std::swap( _created,   other._created);
//	std::swap( _objectID,  other._objectID);
//}

// -------------------------------------------------------------------------------------------------
BufferObject::~BufferObject()
{
	destroy();
}

// -------------------------------------------------------------------------------------------------
//BufferObject& BufferObject::operator=( BufferObject &&other )
//{
//	//if( this != &other )
//	//	return; //check for self assignment
//
//	reset();
//
//	std::swap( _usageHint, other._usageHint );
//	std::swap( _size,      other._size );
//	std::swap( _type,      other._type );
//	
//	std::swap( _created,   other._created);
//	std::swap( _objectID,  other._objectID);
//
//    return *this;
//}

// -------------------------------------------------------------------------------------------------
void BufferObject::reset()
{
	OpenGLObject::reset();
	
	 _usageHint =  UsageHint::StaticDraw;
	 _size      =  0;
	 _type      =  Type::ArrayBuffer;
}

// -------------------------------------------------------------------------------------------------
void BufferObject::create()
{
	destroy();
	OpenGLObject::create();

	
	glGenBuffers( 1, &_objectID );
	glCheck;
	// Allocating here with GL.BufferData, then writing with GL.BufferSubData
	// in CopyFromSystemMemory() should not have any serious overhead:
	//
	//   http://www.opengl.org/discussion_boards/ubbthreads.php?ubb=showflat&Number=267373#Post267373
	//
	// Alternately, we can delay GL.BufferData until the first
	// CopyFromSystemMemory() call.
	//
	//glBindVertexArray( 0 );
	glBindBuffer( glWrap( _type ), _objectID );
	glBufferData( glWrap( _type ), _size, 0, glWrap( _usageHint ) );
	glCheck;
	
	_created = _objectID != 0;
}

// -------------------------------------------------------------------------------------------------
void BufferObject::destroy()
{
	if( !isCreated() )
		return;

	
	glDeleteBuffers( 1, &_objectID );
	glCheck;
	reset();
}

// -------------------------------------------------------------------------------------------------
void BufferObject::bind() const
{
	
	glBindBuffer( glWrap( _type ), _objectID );
	glCheck;
}

// -------------------------------------------------------------------------------------------------
void BufferObject::unbind() const
{
	
	glBindBuffer( glWrap( _type ), 0 );
	glCheck;
}

// -------------------------------------------------------------------------------------------------
void BufferObject::sendData( void *data, int length, int offset )
{
	// @TODO: check arguments validity

	//glBindVertexArray( 0 );
	
	glBindBuffer( glWrap( _type ), _objectID );
	glBufferSubData( glWrap( _type ), offset, length, data );
	glCheck;
}

// -------------------------------------------------------------------------------------------------
void * BufferObject::receiveData( int length, int offset )
{
	// @TODO: check arguments validity

	unsigned char *data = new unsigned char[length];

	//glBindVertexArray( 0 );
	
	glBindBuffer( glWrap( _type ), _objectID );
	glGetBufferSubData( glWrap( _type ), offset, length, data );
	glCheck;
	return data;
}

// -------------------------------------------------------------------------------------------------
void * BufferObject::mapData( const MapMode &mode )
{
	// Note that glMapBuffer() causes a synchronizing issue
	
	glBindBuffer( glWrap( _type ), _objectID );
	auto retval = glMapBuffer( glWrap( _type ), glWrap( mode ) );
	glCheck;
	return retval;
}

// -------------------------------------------------------------------------------------------------
bool BufferObject::unmapData()
{
	
	auto retval = glUnmapBuffer( glWrap( _type ) );
	glCheck;
	return (bool) retval;
}
