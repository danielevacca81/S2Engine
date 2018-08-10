// UniformFloatVector2.h
// 
#ifndef UNIFORMFLOATVECTOR2_ONCE
#define UNIFORMFLOATVECTOR2_ONCE

#include "s2OpenGL_API.h"

#include "Uniform.h"

namespace s2 {

namespace OpenGL {

class S2OPENGL_API UniformFloatVector2: public UniformValue<Math::vec2>
{
public:
	UniformFloatVector2( int location, const std::string  &name )
		: UniformValue<Math::vec2>( location, name )
	{}

	virtual ~UniformFloatVector2()
	{}

	void set();
};

}
}
#endif