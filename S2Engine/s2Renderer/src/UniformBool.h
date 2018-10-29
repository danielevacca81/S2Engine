// UniformBool.h
// 
#ifndef UNIFORMBOOL_ONCE
#define UNIFORMBOOL_ONCE

#include "s2Renderer_API.h"

#include "Uniform.h"

namespace s2 {

namespace Renderer {

class S2RENDERER_API UniformBool: public UniformValue<bool>
{
public:
	UniformBool( int location, const std::string  &name )
		: UniformValue<bool>( location, name )
	{}

	virtual ~UniformBool()
	{}

	void set();
};

}
}
#endif