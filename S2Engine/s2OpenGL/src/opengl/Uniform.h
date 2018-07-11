// Uniform.h
// 
#ifndef UNIFORM_ONCE
#define UNIFORM_ONCE

#include "s2OpenGL_API.h"

#include "Math/Math.h"

#include <string>

namespace s2 {

namespace OpenGL {

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
class S2OPENGL_API Uniform
{
public:
	virtual ~Uniform()	{}

	int         location()    const { return _location; }
	std::string name()        const { return _name;     }


protected:
	friend class Program;

	virtual void set() = 0;
	
	Uniform( int location, const std::string  &name )
	: _name(name)
	, _location(location)
	{}

protected:
	std::string  _name;
	int          _location;
};
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
template<typename T>
class UniformValue : public Uniform
{
public:
	inline T    get()                 const { return _value; }
	inline void set( const T &val )         { _value = val;  }

protected:
	UniformValue( int location, const std::string  &name )
	: Uniform( location,name )
	, _value()
	{}

	virtual ~UniformValue()
	{}

protected:
	T            _value;
};
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------

}



}
#endif