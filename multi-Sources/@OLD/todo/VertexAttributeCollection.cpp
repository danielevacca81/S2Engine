// VertexAttributeCollection.cpp
//
#include "VertexAttributeCollection.h"
#if 0
using namespace Renderer;

// ------------------------------------------------------------------------------------------------
void VertexAttributeCollection::add( const VertexAttribute &va )
{
	_collection.emplace(  va.name(), va );
}

// ------------------------------------------------------------------------------------------------
bool VertexAttributeCollection::remove( const VertexAttribute &item )
{
	return remove( item.name() );
}

// ------------------------------------------------------------------------------------------------
bool VertexAttributeCollection::remove( const std::string &vertexAttributeName )
{
	auto it = _collection.find( vertexAttributeName );
	
	if( it == _collection.end() )
		return false;
	
	_collection.erase( it );
	return true;
}

// ------------------------------------------------------------------------------------------------
void VertexAttributeCollection::clear()
{
	_collection.clear();
}

// ------------------------------------------------------------------------------------------------
bool VertexAttributeCollection::contains( const VertexAttribute &vertexAttribute ) const
{
	return contains( vertexAttribute.name() );
}

// ------------------------------------------------------------------------------------------------
bool VertexAttributeCollection::contains( const std::string &vertexAttributeName ) const
{
	return _collection.find( vertexAttributeName ) != _collection.end();
}

// ------------------------------------------------------------------------------------------------
int  VertexAttributeCollection::size() const
{
	return (int) _collection.size();
}
#endif