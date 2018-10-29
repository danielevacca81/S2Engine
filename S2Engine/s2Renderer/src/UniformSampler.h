// UniformSampler.h
// 
#ifndef UNIFORMSAMPLER_ONCE
#define UNIFORMSAMPLER_ONCE

#include "s2Renderer_API.h"

#include "Uniform.h"

namespace s2 {

namespace Renderer {

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
}
#endif