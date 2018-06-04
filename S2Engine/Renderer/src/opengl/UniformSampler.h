// UniformSampler.h
// 
#ifndef UNIFORMSAMPLER_ONCE
#define UNIFORMSAMPLER_ONCE

#include "Renderer_API.h"

#include "Uniform.h"

namespace OpenGL {

class RENDERER_API UniformSampler : public UniformValue<int>
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