// Uniform.h
// 
#ifndef S2_RENDERCORE_UNIFORM_H
#define S2_RENDERCORE_UNIFORM_H

#include "s2Engine_API.h"

#include "Math/Math.h"
#include "Graphics/Color.h"

#include <string>
#include <variant>

namespace s2 {
namespace RenderCore {

using UniformValue = std::variant<
	bool,
	int,
	uint64_t,
	float,
	double,
	Color,
	Math::ivec2,
	Math::ivec3,
	Math::ivec4,
	Math::fvec2,
	Math::fvec3,
	Math::fvec4,
	Math::fmat2,
	Math::fmat3,
	Math::fmat4,
	Math::dvec2,
	Math::dvec3,
	Math::dvec4,
	Math::dmat2,
	Math::dmat3,
	Math::dmat4
>;

class S2ENGINE_API Uniform
{
public:
	Uniform( int location, const std::string& name, UniformValue value = int{0} )
		: _name( name )
		, _location( location )
		, _value( std::move( value ) )
		, _changed( true )
	{}

	int         location() const { return _location; }
	std::string name()     const { return _name; }

	void setValue( const UniformValue& val )
	{
		if( _value != val )
		{
			_value = val;
			_changed = true;
		}
	}

	const UniformValue& value() const { return _value; }
	bool isChanged() const { return _changed; }

protected:
	// Set uniform with glProgramUniform* (DSA - no binding needed)
	void setDSA( unsigned int programID );

protected:
	std::string    _name;
	int            _location;
	UniformValue   _value;
	bool           _changed;

	friend class Shader;
};

} // namespace RenderCore
} // namespace s2
#endif // !S2_RENDERCORE_UNIFORM_H