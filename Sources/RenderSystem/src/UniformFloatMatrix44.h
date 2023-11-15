// UniformFloatMatrix44.h
// 
#ifndef UNIFORMFLOATMATRIX44_ONCE
#define UNIFORMFLOATMATRIX44_ONCE

#include "RenderSystem_API.h"

#include "Uniform.h"

namespace RenderSystem {

class RENDERSYSTEM_API UniformFloatMatrix44: public UniformValue<Math::mat4>
{
public:
	UniformFloatMatrix44( int location, const std::string  &name )
		: UniformValue<Math::mat4>( location, name )
	{
		_value = Math::mat4(1.0);
	}

	virtual ~UniformFloatMatrix44()
	{}

	void set();
};

}
#endif