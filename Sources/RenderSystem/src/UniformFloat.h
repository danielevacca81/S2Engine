// UniformFloat.h
// 
#ifndef UNIFORMFLOAT_ONCE
#define UNIFORMFLOAT_ONCE

#include "RenderSystem_API.h"

#include "Uniform.h"

namespace RenderSystem {

class RENDERSYSTEM_API UniformFloat: public UniformValue<float>
{
public:
	UniformFloat( int location, const std::string  &name )
		: UniformValue<float>( location, name )
	{}

	virtual ~UniformFloat()
	{}

	void set();
};

}
#endif