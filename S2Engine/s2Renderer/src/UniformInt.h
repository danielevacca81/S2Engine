// UniformInt.h
// 
#ifndef UNIFORMINT_ONCE
#define UNIFORMINT_ONCE

#include "s2Renderer_API.h"

#include "Uniform.h"

namespace Renderer {

class S2RENDERER_API UniformInt: public UniformValue<int>
{
public:
	UniformInt( int location, const std::string  &name )
		: UniformValue<int>( location, name )
	{}

	virtual ~UniformInt()
	{}

	void set();
};

}
#endif