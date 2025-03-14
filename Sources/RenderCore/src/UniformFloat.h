// UniformFloat.h
// 
#ifndef UNIFORMFLOAT_H
#define UNIFORMFLOAT_H

#include "RenderCore_API.h"

#include "Uniform.h"

namespace RenderCore {

class RENDERCORE_API UniformFloat: public UniformValue<float>
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