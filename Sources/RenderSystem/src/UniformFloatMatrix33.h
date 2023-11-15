// UniformFloatMatrix33.h
// 
#ifndef UNIFORMFLOATMATRIX33_ONCE
#define UNIFORMFLOATMATRIX33_ONCE

#include "RenderSystem_API.h"

#include "Uniform.h"

namespace RenderSystem {

class RENDERSYSTEM_API UniformFloatMatrix33: public UniformValue<Math::mat3>
{
public:
	UniformFloatMatrix33( int location, const std::string  &name )
		: UniformValue<Math::mat3>( location, name )
	{
		_value = Math::mat3( 1.0 );
	}

	virtual ~UniformFloatMatrix33()
	{}

	void set();
};

}
#endif