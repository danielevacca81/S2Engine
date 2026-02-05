// Uniform.h
// 
#ifndef S2_RENDERCORE_UNIFORM_H
#define S2_RENDERCORE_UNIFORM_H

#include "s2Engine_API.h"

#include "Math/Math.h"

#include <string>

namespace s2 {
namespace RenderCore {

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
class S2ENGINE_API Uniform
{
public:
	virtual ~Uniform() = default;

	int         location()    const { return _location; }
	std::string name()        const { return _name;     }


protected:
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
	friend class Program;
};
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// maybe variant would be better here?
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
class S2ENGINE_API UniformSampler : public UniformValue<int>
{
public:
	UniformSampler( int location, const std::string& name ) : UniformValue<int>( location, name ) {}
	void set();
};

// ------------------------------------------------------------------------------------------------
class S2ENGINE_API UniformBool : public UniformValue<bool>
{
public: 
	UniformBool( int location, const std::string& name ) : UniformValue<bool>( location, name ) {}
	void set();
};

// ------------------------------------------------------------------------------------------------
class S2ENGINE_API UniformInt : public UniformValue<int>
{
public:
	UniformInt( int location, const std::string& name ) : UniformValue<int>( location, name ) {}
	void set();
};

// ------------------------------------------------------------------------------------------------
class S2ENGINE_API UniformFloat : public UniformValue<float>
{
public:
	UniformFloat( int location, const std::string& name ) : UniformValue<float>( location, name ) {}
	void set();
};

// ------------------------------------------------------------------------------------------------
class S2ENGINE_API UniformFloatVector2 : public UniformValue<Math::vec2>
{
public:
	UniformFloatVector2( int location, const std::string& name ) : UniformValue<Math::vec2>( location, name ) {}
	void set();
};

// ------------------------------------------------------------------------------------------------
class S2ENGINE_API UniformFloatVector3 : public UniformValue<Math::vec3>
{
public:
	UniformFloatVector3( int location, const std::string& name ) : UniformValue<Math::vec3>( location, name ) {}
	void set();
};

// ------------------------------------------------------------------------------------------------
class S2ENGINE_API UniformFloatVector4 : public UniformValue<Math::vec4>
{
public:
	UniformFloatVector4( int location, const std::string& name ) : UniformValue<Math::vec4>( location, name ) {}
	void set();
};


// ------------------------------------------------------------------------------------------------
class S2ENGINE_API UniformFloatMatrix22 : public UniformValue<Math::mat2>
{
public:
	UniformFloatMatrix22( int location, const std::string& name ) : UniformValue<Math::mat2>( location, name ) { _value = Math::mat2( 1.f ); }
	void set();
};

// ------------------------------------------------------------------------------------------------
class S2ENGINE_API UniformFloatMatrix33 : public UniformValue<Math::mat3>
{
public:
	UniformFloatMatrix33( int location, const std::string& name ) : UniformValue<Math::mat3>( location, name ) { _value = Math::mat3( 1.f ); }
	void set();
};

// ------------------------------------------------------------------------------------------------
class S2ENGINE_API UniformFloatMatrix44 : public UniformValue<Math::mat4>
{
public:
	UniformFloatMatrix44( int location, const std::string& name ) : UniformValue<Math::mat4>( location, name ) { _value = Math::mat4( 1.f ); }
	void set();
};


// ------------------------------------------------------------------------------------------------
class S2ENGINE_API UniformDouble : public UniformValue<double>
{
public:
	UniformDouble( int location, const std::string& name ) : UniformValue<double>( location, name ) {}
	void set();
};

// ------------------------------------------------------------------------------------------------
class S2ENGINE_API UniformDoubleVector2 : public UniformValue<Math::dvec2>
{
public:
	UniformDoubleVector2( int location, const std::string& name ) : UniformValue<Math::dvec2>( location, name ) {}
	void set();
};

// ------------------------------------------------------------------------------------------------
class S2ENGINE_API UniformDoubleVector3 : public UniformValue<Math::dvec3>
{
public:
	UniformDoubleVector3( int location, const std::string& name ) : UniformValue<Math::dvec3>( location, name ) {}
	void set();
};


// ------------------------------------------------------------------------------------------------
class S2ENGINE_API UniformDoubleVector4 : public UniformValue<Math::dvec4>
{
public:
	UniformDoubleVector4( int location, const std::string& name ) : UniformValue<Math::dvec4>( location, name ) {}
	void set();
};

// ------------------------------------------------------------------------------------------------
class S2ENGINE_API UniformDoubleMatrix22 : public UniformValue<Math::dmat2>
{
public:
	UniformDoubleMatrix22( int location, const std::string& name ) : UniformValue<Math::dmat2>( location, name ) { _value = Math::dmat2( 1.0 ); }
	void set();
};

// ------------------------------------------------------------------------------------------------
class S2ENGINE_API UniformDoubleMatrix33 : public UniformValue<Math::dmat3>
{
public:
	UniformDoubleMatrix33( int location, const std::string& name ) : UniformValue<Math::dmat3>( location, name ) { _value = Math::dmat3( 1.0 ); }
	void set();
};

// ------------------------------------------------------------------------------------------------
class S2ENGINE_API UniformDoubleMatrix44 : public UniformValue<Math::dmat4>
{
public:
	UniformDoubleMatrix44( int location, const std::string& name ) : UniformValue<Math::dmat4>( location, name ) { _value = Math::dmat4( 1.0 ); }
	void set();
};


} // namespace RenderCore
} // namespace s2
#endif // !S2_RENDERCORE_UNIFORM_H