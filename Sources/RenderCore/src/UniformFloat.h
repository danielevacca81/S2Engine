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


class RENDERCORE_API UniformDouble : public UniformValue<double>
{
public:
	UniformDouble( int location, const std::string& name )
		: UniformValue<double>( location, name )
	{
	}

	virtual ~UniformDouble()
	{
	}

	void set();
};


}
#endif