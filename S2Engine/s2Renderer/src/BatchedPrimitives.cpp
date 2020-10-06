// BatchedPrimitives.cpp
//
#include "BatchedPrimitives.h"

using namespace Renderer;

static const uint32_t kPrimitiveRestartIndex = 0xffff;


// ------------------------------------------------------------------------------------------------
void Batch::operator+=( const Batch &other )
{
	// no meaning in batching primitive of different type
	if( _primitiveType != other._primitiveType )
		return;


	if( !other.indices.empty() )
	{
		indices.push_back( kPrimitiveRestartIndex );
		for( auto &i : other.indices )
		{
			if( i == kPrimitiveRestartIndex )
				continue;
			
			indices.push_back( i + vertices.size() );
		}
	}
		
	vertices.insert     ( vertices.end(),      other.vertices.begin(),      other.vertices.end() );
	colors.insert       ( colors.end(),        other.colors.begin(),        other.colors.end() );
	normals.insert      ( normals.end(),       other.normals.begin(),       other.normals.end() );
	textureCoords.insert( textureCoords.end(), other.textureCoords.begin(), other.textureCoords.end() );
}



// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
BatchedPrimitives::BatchedPrimitives( const PrimitiveType &type )
: _primitiveType( type )
, _batchedPrimitives( type )
, frontFaceWindingOrder( WindingOrder::Counterclockwise )
{}

// ------------------------------------------------------------------------------------------------
void BatchedPrimitives::clear()
{
	_batchedPrimitives = Batch( _primitiveType );
	_vao               = nullptr;
}

// ------------------------------------------------------------------------------------------------
void BatchedPrimitives::add( const Batch &batch )
{
	// no meaning in batching primitive of different type
	if( _primitiveType != batch._primitiveType )
		return;

	_batchedPrimitives += batch;

	// SLOW == COPY FROM BEGINNING TO END == SLOW // (in order to convert from dvec3 to vec3 )

	// send values to VAO	
	if( !_vao )
		_vao = VertexArray::New( BufferObject::UsageHint::StaticDraw );

	//_vao->setVertices      ( { _batchedPrimitives.vertices.begin(),      _batchedPrimitives.vertices.end() } );
	//_vao->setColors        ( { _batchedPrimitives.colors.begin(),        _batchedPrimitives.colors.end() } );
	//_vao->setNormals       ( { _batchedPrimitives.normals.begin(),       _batchedPrimitives.normals.end() } );
	//_vao->setTextureCoords ( { _batchedPrimitives.textureCoords.begin(), _batchedPrimitives.textureCoords.end() } );
	//_vao->setIndices       ( { _batchedPrimitives.indices.begin(),       _batchedPrimitives.indices.end() } );
}

// ------------------------------------------------------------------------------------------------
void BatchedPrimitives::draw( const SurfacePtr &surface, const Renderer::DrawingState &ds ) const
{
	assert( 0 ); // @todo: move to surface API 
	

	//if( !_vao )
	//	return;
	//
	//Renderer::DrawingState drawState( ds );
	//drawState.renderState.primitiveRestart.enabled = true;
	//drawState.renderState.primitiveRestart.index   = kPrimitiveRestartIndex;
	//   	 	
	//_vao->frontFaceWindingOrder = frontFaceWindingOrder;
	//
	//surface->draw( _primitiveType, _primitiveBuffer,  drawState  );
}
