// UniformBool.h
// 
#ifndef UNIFORMBOOL_ONCE
#define UNIFORMBOOL_ONCE

#include "Renderer_API.h"

#include "Uniform.h"

namespace OpenGL {

class RENDERER_API UniformBool : public UniformValue<bool>
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