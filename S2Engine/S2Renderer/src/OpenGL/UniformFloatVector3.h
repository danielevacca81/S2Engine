// UniformFloatVector3.h
// 
#ifndef UNIFORMFLOATVECTOR3_ONCE
#define UNIFORMFLOATVECTOR3_ONCE

#include "S2RendererAPI.h"

#include "Uniform.h"

namespace OpenGL {

class S2RENDERER_API UniformFloatVector3 : public UniformValue<Math::vec3>
{
public:
	UniformFloatVector3( int location, const std::string  &name )
	: UniformValue<Math::vec3>( location,name )
	{}

	virtual ~UniformFloatVector3()
	{}

	void set();
};

}
#endif