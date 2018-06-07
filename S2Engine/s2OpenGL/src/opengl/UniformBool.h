// UniformBool.h
// 
#ifndef UNIFORMBOOL_ONCE
#define UNIFORMBOOL_ONCE

#include "s2OpenGL_API.h"

#include "Uniform.h"

namespace s2 {

namespace OpenGL {

class S2OPENGL_API UniformBool: public UniformValue<bool>
{
public:
	UniformBool( int location, const std::string  &name )
		: UniformValue<bool>( location, name )
	{}

	virtual ~UniformBool()
	{}

	void set();
};

}
}
#endif