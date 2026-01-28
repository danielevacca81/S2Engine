// UniformFloatVector3.h
// 
#ifndef UNIFORMFLOATVECTOR3_H
#define UNIFORMFLOATVECTOR3_H

#include "s2Engine_API.h"

#include "Uniform.h"

namespace RenderCore {

class S2ENGINE_API UniformFloatVector3: public UniformValue<Math::vec3>
{
public:
	UniformFloatVector3( int location, const std::string  &name )
		: UniformValue<Math::vec3>( location, name )
	{}

	virtual ~UniformFloatVector3()
	{}

	void set();
};

class S2ENGINE_API UniformDoubleVector3 : public UniformValue<Math::dvec3>
{
public:
	UniformDoubleVector3( int location, const std::string& name )
		: UniformValue<Math::dvec3>( location, name )
	{
	}

	virtual ~UniformDoubleVector3()
	{
	}

	void set();
};


}
#endif