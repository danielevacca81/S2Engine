// VertexAttribute.h
//
#ifndef VERTEXATTRIBUTE_ONCE
#define VERTEXATTRIBUTE_ONCE

#include "s2Renderer_API.h"

#include "VertexAttributeType.h"

#include <vector>
#include <map>
#include <memory>
#include <variant>

namespace Renderer {

class S2RENDERER_API VertexAttribute
{
public:
	template<typename T>
	static VertexAttribute fromValues( const std::string &name, const std::vector<T> &v )
	{
		VertexAttributeDataType data = VertexAttributeDataType::build<T>();

		VertexAttribute va( name );
		va._datatype = data;
		va._values   = v;
		return va;
	}

public:
	VertexAttribute()                         = default;
	VertexAttribute( const VertexAttribute &) = default;
	VertexAttribute( VertexAttribute &&)      = default;
	VertexAttribute( const std::string &name )
	: _name( name )
	{}
	
	~VertexAttribute()                        = default;
	VertexAttribute &operator=( const VertexAttribute &other )
	{
		_name     = other._name;
		_datatype = other._datatype;
		_values   = other._values;
		return *this;
	}

	VertexAttribute &operator=( VertexAttribute &&other )
	{
		_name     = std::move( other._name );
		_datatype = std::move( other._datatype );
		_values   = std::move( other._values );
		return *this;
	}

	inline std::string                    name()     const { return _name; }
	inline const VertexAttributeDataType &datatype() const { return _datatype; }	
	
	template<typename T>
	inline std::vector<T> values() const// copy
	{ 
		return std::get<std::vector<T>>( _values );
	}

	VertexAttributeValueType &values() { return _values; }


private:
	std::string              _name;
	VertexAttributeDataType  _datatype;
	VertexAttributeValueType _values;
};

//template<typename T>
//class VertexAttributeBase : public VertexAttribute
//{
//public:
//	std::vector<T> values;
//
//
//protected:
//	VertexAttributeBase( const std::string &name, const VertexAttributeType &type, const std::vector<T> &v )
//    : VertexAttibute(name, type)
//	, values( v )
//    {}
//
//	VertexAttributeBase( const std::string &name, const VertexAttributeType &type, int capacity )
//	: VertexAttibute(name, type)
//	, values( capacity )
//    {}
//};

using VertexAttributeCollection = std::map< int, VertexAttribute >;

}
#endif