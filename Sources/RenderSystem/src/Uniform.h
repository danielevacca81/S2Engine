// Uniform.h
// 
#ifndef UNIFORM_ONCE
#define UNIFORM_ONCE

#include "RenderSystem_API.h"


#include <string>

namespace RenderSystem {

/*
class GLUL_API Uniform
{
public:
    Uniform();
    Uniform(GLuint location);
    ~Uniform();

    GLuint getLocation() const;
            
    void setSampler(GLint value) const;

public:
    template<typename T> void operator=(const T& value) const;

    template<typename T> void set(T v0) const;
    template<typename T> void set(T v0, T v1) const;
    template<typename T> void set(T v0, T v1, T v2) const;
    template<typename T> void set(T v0, T v1, T v2, T v3) const;
            
    template<typename T> void setVec(const T& value, GLsizei count = 1) const;
    template<typename T> void setMatrix(const T& value, GLsizei count = 1, GLboolean transpose = GL_FALSE) const;

private:
    GLint _location;
};
*/



// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
class RENDERSYSTEM_API Uniform
{
public:
	virtual ~Uniform() = default;

	int         location()    const { return _location; }
	std::string name()        const { return _name;     }


protected:
	friend class Program;

	virtual void set() = 0;
	
	Uniform( int location, const std::string  &name )
	: _name(name)
	, _location(location)
	, _changed(true)
	{}

protected:
	std::string  _name;
	int          _location;
	bool         _changed;	// used for unifrom shadowing
							// uniform values remain in GLSL shader if unbound: 
							// * https://stackoverflow.com/questions/10857602/do-uniform-values-remain-in-glsl-shader-if-unbound
	                        // * https://www.khronos.org/registry/OpenGL/specs/gl/glspec33.core.pdf (section 2.11.4)
	                        // They retain their values once loaded, and their values are restored whenever a program object is used
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
	inline void set( const T &val )         { if(_value!=val) { _value=val; _changed=true; } }

protected:
	UniformValue()
	: UniformValue( -1, "" ) // If location is equal to -1, the data passed in will be silently ignored and the specified uniform variable will not be changed
	{}
	
	UniformValue( int location, const std::string  &name )
	: Uniform( location,name )
	, _value()
	{}

	virtual ~UniformValue() = default;

protected:
	T            _value;
};
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------

}
#endif