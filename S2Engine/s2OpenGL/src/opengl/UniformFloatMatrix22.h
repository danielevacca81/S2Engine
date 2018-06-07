// UniformFloatMatrix22.h
// 
#ifndef UNIFORMFLOATMATRIX22_ONCE
#define UNIFORMFLOATMATRIX22_ONCE

#include "s2OpenGL_API.h"

#include "Uniform.h"

namespace s2 {

namespace OpenGL {

class S2OPENGL_API UniformFloatMatrix22: public UniformValue<Math::mat2>
{
public:
	UniformFloatMatrix22( int location, const std::string  &name )
		: UniformValue<Math::mat2>( location, name )
	{}

	virtual ~UniformFloatMatrix22()
	{}

	void set();
};

}
}
#endif