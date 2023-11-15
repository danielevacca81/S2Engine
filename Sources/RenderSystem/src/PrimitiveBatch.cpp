// PrimitiveBatch.cpp
//
#include "PrimitiveBatch.h"


using namespace RenderSystem;

// ------------------------------------------------------------------------------------------------
PrimitiveBatch::PrimitiveBatch( const PrimitiveType &type, const uint32_t &restartIndex )
: _batch( type )
, _primitiveRestartIndex( restartIndex )
, frontFaceWindingOrder( WindingOrder::Counterclockwise )
{}

// ------------------------------------------------------------------------------------------------
void PrimitiveBatch::add( const Batch& b )
{
	// no meaning in batching primitive of different type
	if( _batch._primitiveType != b._primitiveType )
		return;

	if( b.indices.empty() )
		return;

	_batch.indices.push_back( _primitiveRestartIndex );
	for( auto& i : b.indices )
	{
		if( i == _primitiveRestartIndex )
			continue;

		_batch.indices.push_back( i + _batch.vertices.size() );
	}

	_batch.vertices.insert     ( _batch.vertices.end(),      b.vertices.begin(),      b.vertices.end() );
	_batch.colors.insert       ( _batch.colors.end(),        b.colors.begin(),        b.colors.end() );
	_batch.normals.insert      ( _batch.normals.end(),       b.normals.begin(),       b.normals.end() );
	_batch.textureCoords.insert( _batch.textureCoords.end(), b.textureCoords.begin(), b.textureCoords.end() );
}