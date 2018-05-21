// UniformFloatMatrix22.h
// 
#ifndef UNIFORMFLOATMATRIX22_ONCE
#define UNIFORMFLOATMATRIX22_ONCE

#include "S2RendererAPI.h"

#include "Uniform.h"

namespace OpenGL {

class S2RENDERER_API UniformFloatMatrix22 : public UniformValue<Math::mat2>
{
public:
	UniformFloatMatrix22( int location, const std::string  &name )
	: UniformValue<Math::mat2>( location,name )
	{}

	virtual ~UniformFloatMatrix22()
	{}

	void set();
};

}
#endif