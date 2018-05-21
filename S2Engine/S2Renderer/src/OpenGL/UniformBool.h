// UniformBool.h
// 
#ifndef UNIFORMBOOL_ONCE
#define UNIFORMBOOL_ONCE

#include "S2RendererAPI.h"

#include "Uniform.h"

namespace OpenGL {

class S2RENDERER_API UniformBool : public UniformValue<bool>
{
public:
	UniformBool( int location, const std::string  &name )
	: UniformValue<bool>( location,name )
	{}

	virtual ~UniformBool()
	{}

	void set();
};

}
#endif