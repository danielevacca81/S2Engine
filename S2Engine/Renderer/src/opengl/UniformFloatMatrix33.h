// UniformFloatMatrix33.h
// 
#ifndef UNIFORMFLOATMATRIX33_ONCE
#define UNIFORMFLOATMATRIX33_ONCE

#include "Renderer_API.h"

#include "Uniform.h"

namespace OpenGL {

class RENDERER_API UniformFloatMatrix33 : public UniformValue<Math::mat3>
{
public:
	UniformFloatMatrix33( int location, const std::string  &name )
	: UniformValue<Math::mat3>( location,name )
	{}

	virtual ~UniformFloatMatrix33()
	{}

	void set();
};

}
#endif