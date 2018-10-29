// UniformFloatMatrix22.h
// 
#ifndef UNIFORMFLOATMATRIX22_ONCE
#define UNIFORMFLOATMATRIX22_ONCE

#include "s2Renderer_API.h"

#include "Uniform.h"

namespace s2 {

namespace Renderer {

class S2RENDERER_API UniformFloatMatrix22: public UniformValue<Math::mat2>
{
public:
	UniformFloatMatrix22( int location, const std::string  &name )
		: UniformValue<Math::mat2>( location, name )
	{}

	virtual ~UniformFloatMatrix22()
	{}

	void set();
};

}
}
#endif