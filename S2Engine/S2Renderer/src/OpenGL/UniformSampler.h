// UniformSampler.h
// 
#ifndef UNIFORMSAMPLER_ONCE
#define UNIFORMSAMPLER_ONCE

#include "S2RendererAPI.h"

#include "Uniform.h"

namespace OpenGL {

class S2RENDERER_API UniformSampler : public UniformValue<int>
{
public:
	UniformSampler( int location, const std::string  &name )
	: UniformValue<int>( location,name )
	{}

	virtual ~UniformSampler()
	{}

	void set();
};

}
#endif