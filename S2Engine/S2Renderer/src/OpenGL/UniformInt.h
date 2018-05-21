// UniformInt.h
// 
#ifndef UNIFORMINT_ONCE
#define UNIFORMINT_ONCE

#include "S2RendererAPI.h"

#include "Uniform.h"

namespace OpenGL {

class S2RENDERER_API UniformInt : public UniformValue<int>
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