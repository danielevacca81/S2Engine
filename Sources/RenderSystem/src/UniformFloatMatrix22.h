// UniformFloatMatrix22.h
// 
#ifndef UNIFORMFLOATMATRIX22_ONCE
#define UNIFORMFLOATMATRIX22_ONCE

#include "RenderSystem_API.h"

#include "Uniform.h"

namespace RenderSystem {

class RENDERSYSTEM_API UniformFloatMatrix22: public UniformValue<Math::mat2>
{
public:
	UniformFloatMatrix22( int location, const std::string  &name )
		: UniformValue<Math::mat2>( location, name )
	{
		_value = Math::mat2( 1.0 );
	}

	virtual ~UniformFloatMatrix22()
	{}

	void set();
};

}
#endif