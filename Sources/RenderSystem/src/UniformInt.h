// UniformInt.h
// 
#ifndef UNIFORMINT_ONCE
#define UNIFORMINT_ONCE

#include "RenderSystem_API.h"

#include "Uniform.h"

namespace RenderSystem {

class RENDERSYSTEM_API UniformInt: public UniformValue<int>
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