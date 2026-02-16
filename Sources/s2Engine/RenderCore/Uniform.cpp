// Uniform.cpp
// 
#include "Uniform.h"

#include "OpenGL.h"
#include "OpenGLCheck.h"

#include "Math/Math.h"

using namespace s2::RenderCore;

// ------------------------------------------------------------------------------------------------
struct UniformSetter
{
	int location;

	void operator()( bool value )               const { glUniform1i( location, value ); glCheck; }
	void operator()( int value )                const { glUniform1i( location, value ); glCheck; }
	void operator()( float value )              const { glUniform1f( location, value ); glCheck; }
	void operator()( const Math::vec2& value )  const { glUniform2fv( location, 1, Math::value_ptr( value ) ); glCheck; }
	void operator()( const Math::vec3& value )  const { glUniform3fv( location, 1, Math::value_ptr( value ) ); glCheck; }
	void operator()( const Math::vec4& value )  const { glUniform4fv( location, 1, Math::value_ptr( value ) ); glCheck; }
	void operator()( const Math::mat2& value )  const { glUniformMatrix2fv( location, 1, false, Math::value_ptr( value ) ); glCheck; }
	void operator()( const Math::mat3& value )  const { glUniformMatrix3fv( location, 1, false, Math::value_ptr( value ) ); glCheck; }
	void operator()( const Math::mat4& value )  const { glUniformMatrix4fv( location, 1, false, Math::value_ptr( value ) ); glCheck; }
	void operator()( const s2::Color& value )   const { glUniform4fv( location, 1, Math::value_ptr( Math::vec4 (value.r(), value.g(), value.b(), value.a() ) ) ); glCheck; }

	// Double precision uniforms: need to check if supported by the target OpenGL version and hardware.
	// For now, we can exclude them to maintain wider compatibility.
#if 0
	void operator()( double value )             const { glUniform1d( location, value ); glCheck; }
	void operator()( const Math::dvec2& value ) const { glUniform2dv( location, 1, Math::value_ptr( value ) ); glCheck; }
	void operator()( const Math::dvec3& value ) const { glUniform3dv( location, 1, Math::value_ptr( value ) ); glCheck; }
	void operator()( const Math::dvec4& value ) const { glUniform4dv( location, 1, Math::value_ptr( value ) ); glCheck; }
	void operator()( const Math::dmat2& value ) const { glUniformMatrix2dv( location, 1, false, Math::value_ptr( value ) ); glCheck; }
	void operator()( const Math::dmat3& value ) const { glUniformMatrix3dv( location, 1, false, Math::value_ptr( value ) ); glCheck; }
	void operator()( const Math::dmat4& value ) const { glUniformMatrix4dv( location, 1, false, Math::value_ptr( value ) ); glCheck; }
#else
	void operator()( double value )             const { glUniform1f( location, float(value) ); glCheck; }
	void operator()( const Math::dvec2& value ) const { glUniform2fv( location, 1, Math::value_ptr( Math::fvec2( value ) ) ); glCheck; }
	void operator()( const Math::dvec3& value ) const { glUniform3fv( location, 1, Math::value_ptr( Math::fvec3( value ) ) ); glCheck; }
	void operator()( const Math::dvec4& value ) const { glUniform4fv( location, 1, Math::value_ptr( Math::fvec4( value ) ) ); glCheck; }
	void operator()( const Math::dmat2& value ) const { glUniformMatrix2fv( location, 1, false, Math::value_ptr( Math::fmat2( value ) ) ); glCheck; }
	void operator()( const Math::dmat3& value ) const { glUniformMatrix3fv( location, 1, false, Math::value_ptr( Math::fmat3( value ) ) ); glCheck; }
	void operator()( const Math::dmat4& value ) const { glUniformMatrix4fv( location, 1, false, Math::value_ptr( Math::fmat4( value ) ) ); glCheck; }
#endif
};

// ------------------------------------------------------------------------------------------------
void Uniform::set()
{
	if( !_changed )
		return;
		
	std::visit( UniformSetter{ _location }, _value );
	_changed = false;
}