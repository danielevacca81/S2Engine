// UniformInt.h
// 
#ifndef UNIFORMINT_H
#define UNIFORMINT_H

#include "s2Engine_API.h"

#include "Uniform.h"

namespace RenderCore {

class S2ENGINE_API UniformInt: public UniformValue<int>
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