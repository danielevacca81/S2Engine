// UniformFloatMatrix44.h
// 
#ifndef UNIFORMFLOATMATRIX44_ONCE
#define UNIFORMFLOATMATRIX44_ONCE

#include "s2Renderer_API.h"

#include "Uniform.h"

namespace s2 {

namespace Renderer {

class S2RENDERER_API UniformFloatMatrix44: public UniformValue<Math::mat4>
{
public:
	UniformFloatMatrix44( int location, const std::string  &name )
		: UniformValue<Math::mat4>( location, name )
	{}

	virtual ~UniformFloatMatrix44()
	{}

	void set();
};

}
}
#endif