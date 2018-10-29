// UniformFloatVector3.h
// 
#ifndef UNIFORMFLOATVECTOR3_ONCE
#define UNIFORMFLOATVECTOR3_ONCE

#include "s2Renderer_API.h"

#include "Uniform.h"

namespace s2 {

namespace Renderer {

class S2RENDERER_API UniformFloatVector3: public UniformValue<Math::vec3>
{
public:
	UniformFloatVector3( int location, const std::string  &name )
		: UniformValue<Math::vec3>( location, name )
	{}

	virtual ~UniformFloatVector3()
	{}

	void set();
};

}
}
#endif