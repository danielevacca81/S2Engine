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
		_value = Math::mat4(1.0);
	}

	virtual ~UniformFloatMatrix44()
	{}

	void set();
};

}
#endif