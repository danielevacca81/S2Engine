// UniformFloatVector4.h
// 
#ifndef UNIFORMFLOATVECTOR4_H
#define UNIFORMFLOATVECTOR4_H

#include "RenderCore_API.h"

#include "Uniform.h"

namespace RenderCore {

class RENDERCORE_API UniformFloatVector4: public UniformValue<Math::vec4>
{
public:
	UniformFloatVector4( int location, const std::string  &name )
		: UniformValue<Math::vec4>( location, name )
	{}

	virtual ~UniformFloatVector4()
	{}

	void set();
};

class RENDERCORE_API UniformDoubleVector4 : public UniformValue<Math::dvec4>
{
public:
	UniformDoubleVector4( int location, const std::string& name )
		: UniformValue<Math::dvec4>( location, name )
	{
	}

	virtual ~UniformDoubleVector4()
	{
	}

	void set();
};


}
#endif