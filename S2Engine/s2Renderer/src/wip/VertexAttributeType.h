// VertexAttributeType.h
//
#ifndef VERTEXATTRIBUTETYPE_ONCE
#define VERTEXATTRIBUTETYPE_ONCE

#include "s2Renderer_API.h"

#include "Core/Math.h"
#include "Core/Color.h"

#include <variant>
#include <vector>
#include <typeinfo>

namespace Renderer {


struct VertexAttributeDataType
{
	// use integer enumerator in order to allow variant access using std:get<> by index
	enum Type : int
	{
		UnsignedByte = 0,
		//HalfFloat,
		//HalfFloatVector2,
		//HalfFloatVector3,
		//HalfFloatVector4,
		Float,
		FloatVector2,
		FloatVector3,
		FloatVector4,
		//EmulatedDoubleVector3
	};


	Type type;
	int  sizeInBytes = 0;
	int  componentCount = 0;

	template<typename T>
	static VertexAttributeDataType build()
	{
		VertexAttributeDataType data;
		data.type           = VertexAttributeDataType::UnsignedByte;
		data.sizeInBytes    = sizeof( T );
		data.componentCount = 1;
		return data;
	}
};


template<>
static inline VertexAttributeDataType VertexAttributeDataType::build<uint8_t>()
{
	VertexAttributeDataType data;
	data.type           = VertexAttributeDataType::UnsignedByte;
	data.sizeInBytes    = sizeof( uint8_t );
	data.componentCount = 1;
	return data;
}

template<>
static inline  VertexAttributeDataType VertexAttributeDataType::build<float>()
{
	VertexAttributeDataType data;
	data.type           = VertexAttributeDataType::Float;
	data.sizeInBytes    = sizeof( float );
	data.componentCount = 1;
	return data;
}

template<>
static inline  VertexAttributeDataType VertexAttributeDataType::build<Math::fvec2>()
{
	VertexAttributeDataType data;
	data.type           = VertexAttributeDataType::FloatVector2;
	data.sizeInBytes    = sizeof( Math::fvec2 );
	data.componentCount = 2;
	return data;
}

template<>
static inline  VertexAttributeDataType VertexAttributeDataType::build<Math::fvec3>()
{
	VertexAttributeDataType data;
	data.type           = VertexAttributeDataType::FloatVector3;
	data.sizeInBytes    = sizeof( Math::fvec3 );
	data.componentCount = 3;
	return data;
}

template<>
static inline  VertexAttributeDataType VertexAttributeDataType::build<Math::fvec4>()
{
	VertexAttributeDataType data;
	data.type           = VertexAttributeDataType::FloatVector4;
	data.sizeInBytes    = sizeof( Math::fvec4 );
	data.componentCount = 4;
	return data;
}

template<>
static inline  VertexAttributeDataType VertexAttributeDataType::build<::Color>()
{
	VertexAttributeDataType data;
	data.type           = VertexAttributeDataType::FloatVector4;
	data.sizeInBytes    = sizeof( ::Color );
	data.componentCount = 4;
	return data;
}

using VertexAttributeValueType = std::variant<
	std::vector<uint8_t>,      // 0
	std::vector<float>,        // 1
	std::vector<Math::fvec2>,  // 2
	std::vector<Math::fvec3>,  // 3
	std::vector<Math::fvec4>,  // 4
	std::vector<::Color>       // 5
	>;


}
#endif