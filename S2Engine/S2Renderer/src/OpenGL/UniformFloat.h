// UniformFloat.h
// 
#ifndef UNIFORMFLOAT_ONCE
#define UNIFORMFLOAT_ONCE

#include "S2RendererAPI.h"

#include "Uniform.h"

namespace OpenGL {

class S2RENDERER_API UniformFloat : public UniformValue<float>
{
public:
	UniformFloat( int location, const std::string  &name )
	: UniformValue<float>( location,name )
	{}

	virtual ~UniformFloat()
	{}

	void set();
};

}
#endif