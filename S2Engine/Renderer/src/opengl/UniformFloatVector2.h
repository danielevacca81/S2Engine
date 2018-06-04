// UniformFloatVector2.h
// 
#ifndef UNIFORMFLOATVECTOR2_ONCE
#define UNIFORMFLOATVECTOR2_ONCE

#include "Renderer_API.h"

#include "Uniform.h"

namespace OpenGL {

class RENDERER_API UniformFloatVector2 : public UniformValue<Math::vec2>
{
public:
	UniformFloatVector2( int location, const std::string  &name )
	: UniformValue<Math::vec2>( location,name )
	{}

	virtual ~UniformFloatVector2()
	{}

	void set();
};

}
#endif