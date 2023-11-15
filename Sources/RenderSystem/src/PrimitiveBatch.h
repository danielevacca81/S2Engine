// PrimitiveBatch.h
//
#ifndef PRIMITIVEBATCH_ONCE
#define PRIMITIVEBATCH_ONCE

#include "RenderSystem_API.h"

#include "PrimitiveBuffer.h"
#include "PrimitiveType.h"

#include "Core/Color.h"
#include "Math/Math.h"

#include <vector>


namespace RenderSystem {

class RENDERSYSTEM_API PrimitiveBatch
{
public:
	static constexpr uint32_t kDefaultPrimitiveRestartIndex = 0xffff;

	// ------------------------------------------------------------------------------------------------
	class RENDERSYSTEM_API Batch
	{
	public:
		Batch( const PrimitiveType& type )
			: _primitiveType( type )
		{}

	public:
		std::vector<Math::vec3>  vertices;
		std::vector<Math::vec3>  normals;
		std::vector<Color>       colors;
		std::vector<Math::vec2>  textureCoords;
		std::vector<uint32_t>    indices;

	private:
		PrimitiveType    _primitiveType;
		friend class PrimitiveBatch;
	};


public:
	PrimitiveBatch( const PrimitiveType& type, const uint32_t& restertIndex = kDefaultPrimitiveRestartIndex /*@todo: hint?*/ );
	void add( const Batch& batch );

	uint32_t      primitiveRestartIndex() const { return _primitiveRestartIndex; }
	PrimitiveType primitiveType() const { return _batch._primitiveType; }
	const Batch&  batch() const { return _batch; }

public:
	WindingOrder frontFaceWindingOrder; //?

private:
	uint32_t _primitiveRestartIndex { kDefaultPrimitiveRestartIndex };
	Batch    _batch;
};
}
#endif