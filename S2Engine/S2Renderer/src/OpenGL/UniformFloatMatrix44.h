// UniformFloatMatrix44.h
// 
#ifndef UNIFORMFLOATMATRIX44_ONCE
#define UNIFORMFLOATMATRIX44_ONCE

#include "S2RendererAPI.h"

#include "Uniform.h"

namespace OpenGL {

class S2RENDERER_API UniformFloatMatrix44 : public UniformValue<Math::mat4>
{
public:
	UniformFloatMatrix44( int location, const std::string  &name )
	: UniformValue<Math::mat4>( location,name )
	{}

	virtual ~UniformFloatMatrix44()
	{}

	void set();
};

}
#endif