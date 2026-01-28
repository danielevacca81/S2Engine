// UniformBool.h
// 
#ifndef UNIFORMBOOL_H
#define UNIFORMBOOL_H

#include "RenderCore_API.h"

#include "Uniform.h"

namespace RenderCore {

class RENDERCORE_API UniformBool: public UniformValue<bool>
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
#endif