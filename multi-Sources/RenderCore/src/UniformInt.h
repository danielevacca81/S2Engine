// UniformInt.h
// 
#ifndef UNIFORMINT_H
#define UNIFORMINT_H

#include "RenderCore_API.h"

#include "Uniform.h"

namespace RenderCore {

class RENDERCORE_API UniformInt: public UniformValue<int>
{
public:
	UniformInt( int location, const std::string  &name )
		: UniformValue<int>( location, name )
	{}

	virtual ~UniformInt()
	{}

	void set();
};

}
#endif