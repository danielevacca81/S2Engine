// UniformSampler.h
// 
#ifndef UNIFORMSAMPLER_H
#define UNIFORMSAMPLER_H

#include "s2Engine_API.h"

#include "Uniform.h"

namespace RenderCore {

class S2ENGINE_API UniformSampler : public UniformValue<int>
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