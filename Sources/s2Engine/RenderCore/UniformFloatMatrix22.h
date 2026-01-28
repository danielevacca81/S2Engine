// UniformFloatMatrix22.h
// 
#ifndef UNIFORMFLOATMATRIX22_H
#define UNIFORMFLOATMATRIX22_H

#include "s2Engine_API.h"

#include "Uniform.h"

namespace RenderCore {

class S2ENGINE_API UniformFloatMatrix22: public UniformValue<Math::mat2>
{
public:
	UniformFloatMatrix22( int location, const std::string  &name )
		: UniformValue<Math::mat2>( location, name )
	{
		_value = Math::mat2( 1.f );
	}

	virtual ~UniformFloatMatrix22()
	{}

	void set();
};

class S2ENGINE_API UniformDoubleMatrix22 : public UniformValue<Math::dmat2>
{
public:
	UniformDoubleMatrix22( int location, const std::string& name )
		: UniformValue<Math::dmat2>( location, name )
	{
		_value = Math::dmat2( 1.0 );
	}

	virtual ~UniformDoubleMatrix22()
	{
	}

	void set();
};

}
#endif