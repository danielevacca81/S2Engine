// UniformFloatVector2.h
// 
#ifndef UNIFORMFLOATVECTOR2_H
#define UNIFORMFLOATVECTOR2_H

#include "s2Engine_API.h"

#include "Uniform.h"

#include "Math/Math.h"

namespace RenderCore {

class S2ENGINE_API UniformFloatVector2: public UniformValue<Math::vec2>
{
public:
	UniformFloatVector2( int location, const std::string  &name )
		: UniformValue<Math::vec2>( location, name )
	{}

	virtual ~UniformFloatVector2()
	{}

	void set();
};

class S2ENGINE_API UniformDoubleVector2 : public UniformValue<Math::dvec2>
{
public:
	UniformDoubleVector2( int location, const std::string& name )
		: UniformValue<Math::dvec2>( location, name )
	{
	}

	virtual ~UniformDoubleVector2()
	{
	}

	void set();
};

}
#endif