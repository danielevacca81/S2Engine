// UniformInt.h
// 
#ifndef UNIFORMINT_ONCE
#define UNIFORMINT_ONCE

#include "s2OpenGL_API.h"

#include "Uniform.h"

namespace OpenGL {

class S2OPENGL_API UniformInt : public UniformValue<int>
{
public:
	UniformInt( int location, const std::string  &name )
	: UniformValue<int>( location,name )
	{}

	virtual ~UniformInt()
	{}

	void set();
};

}
#endif