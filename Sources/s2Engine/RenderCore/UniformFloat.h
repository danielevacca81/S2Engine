// UniformFloat.h
// 
#ifndef UNIFORMFLOAT_H
#define UNIFORMFLOAT_H

#include "s2Engine_API.h"

#include "Uniform.h"

namespace RenderCore {

class S2ENGINE_API UniformFloat: public UniformValue<float>
{
public:
	UniformFloat( int location, const std::string  &name )
		: UniformValue<float>( location, name )
	{}

	virtual ~UniformFloat()
	{}

	void set();
};


class S2ENGINE_API UniformDouble : public UniformValue<double>
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