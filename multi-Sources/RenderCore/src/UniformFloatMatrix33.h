// UniformFloatMatrix33.h
// 
#ifndef UNIFORMFLOATMATRIX33_H
#define UNIFORMFLOATMATRIX33_H

#include "RenderCore_API.h"

#include "Uniform.h"

namespace RenderCore {

class RENDERCORE_API UniformFloatMatrix33: public UniformValue<Math::mat3>
{
public:
	UniformFloatMatrix33( int location, const std::string  &name )
		: UniformValue<Math::mat3>( location, name )
	{
		_value = Math::mat3( 1.f );
	}

	virtual ~UniformFloatMatrix33()
	{}

	void set();
};


class RENDERCORE_API UniformDoubleMatrix33 : public UniformValue<Math::dmat3>
{
public:
	UniformDoubleMatrix33( int location, const std::string& name )
		: UniformValue<Math::dmat3>( location, name )
	{
		_value = Math::dmat3( 1.0 );
	}

	virtual ~UniformDoubleMatrix33()
	{
	}

	void set();
};


}
#endif