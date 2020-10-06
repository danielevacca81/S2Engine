// BatchedPrimitives.h
//
#ifndef BATCHEDPRIMITIVES_ONCE
#define BATCHEDPRIMITIVES_ONCE

#include "s2Renderer_API.h"


#include "Surface.h"
#include "DrawingState.h"
#include "PrimitiveBuffer.h"
#include "PrimitiveType.h"

#include "Core/Color.h"
#include "Core/Math.h"

#include <vector>


namespace Renderer {


// ------------------------------------------------------------------------------------------------
class S2RENDERER_API Batch
{
public:
	Batch( const PrimitiveType &type )
	: _primitiveType(type) 
	{}

	void operator+=( const Batch &other );


public:
	std::vector<Math::dvec3> vertices;
	std::vector<Math::vec3>  normals;
	std::vector<Color>       colors;
	std::vector<Math::dvec2> textureCoords;
	std::vector<uint32_t>    indices;

private:
	PrimitiveType    _primitiveType;
	friend class BatchedPrimitives;
};


// ------------------------------------------------------------------------------------------------
class S2RENDERER_API BatchedPrimitives
{	
public:
	BatchedPrimitives( const PrimitiveType &type );
	void add( const Batch &batch );
	void clear();

	void draw( const SurfacePtr &surface, const Renderer::DrawingState &ds ) const;

public:
	WindingOrder    frontFaceWindingOrder;

private:
	PrimitiveType   _primitiveType;
	Batch           _batchedPrimitives;
	VertexArrayPtr  _vao;
};

}
#endif