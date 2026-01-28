// UniformFloatMatrix44.h
// 
#ifndef UNIFORMFLOATMATRIX44_H
#define UNIFORMFLOATMATRIX44_H

#include "RenderCore_API.h"

#include "Uniform.h"

namespace RenderCore {

class RENDERCORE_API UniformFloatMatrix44: public UniformValue<Math::mat4>
{
public:
	UniformFloatMatrix44( int location, const std::string  &name )
		: UniformValue<Math::mat4>( location, name )
	{
		_value = Math::mat4( 1.f );
	}

	virtual ~UniformFloatMatrix44()
	{}

	void set();
};

class RENDERCORE_API UniformDoubleMatrix44 : public UniformValue<Math::dmat4>
{
public:
	UniformDoubleMatrix44( int location, const std::string& name )
		: UniformValue<Math::dmat4>( location, name )
	{
		_value = Math::dmat4( 1.0 );
	}

	virtual ~UniformDoubleMatrix44()
	{
	}

	void set();
};

}
#endif