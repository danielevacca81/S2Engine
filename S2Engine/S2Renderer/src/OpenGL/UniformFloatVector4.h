// UniformFloatVector4.h
// 
#ifndef UNIFORMFLOATVECTOR4_ONCE
#define UNIFORMFLOATVECTOR4_ONCE

#include "S2RendererAPI.h"

#include "Uniform.h"

namespace OpenGL {

class S2RENDERER_API UniformFloatVector4 : public UniformValue<Math::vec4>
{
public:
	UniformFloatVector4( int location, const std::string  &name )
	: UniformValue<Math::vec4>( location,name )
	{}

	virtual ~UniformFloatVector4()
	{}

	void set();
};

}
#endif