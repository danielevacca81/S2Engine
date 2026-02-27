// Uniform.cpp
// 
#include "Uniform.h"

#include "OpenGL.h"
#include "OpenGLCheck.h"

using namespace s2::RenderCore;

// ------------------------------------------------------------------------------------------------
// Helper visitor for glProgramUniform* (DSA)
// ------------------------------------------------------------------------------------------------
struct UniformSetterDSA
{
    unsigned int programID;
    int location;

    void operator()( bool value )               const { glProgramUniform1i           ( programID, location, value ? 1 : 0 );             glCheck; }
    void operator()( int value )                const { glProgramUniform1i           ( programID, location, value );                     glCheck; }
    void operator()( uint64_t value )           const { glProgramUniformHandleui64ARB( programID, location, value );                     glCheck; }
    void operator()( float value )              const { glProgramUniform1f           ( programID, location, value );                     glCheck; }
    void operator()( double value )             const { glProgramUniform1d           ( programID, location, value );                     glCheck; }
    void operator()( const s2::Color& value )   const { glProgramUniform4fv          ( programID, location, 1, value.rgba() );           glCheck; }
    void operator()( const Math::ivec2& value ) const { glProgramUniform2iv          ( programID, location, 1, &value.x );               glCheck; }
    void operator()( const Math::ivec3& value ) const { glProgramUniform3iv          ( programID, location, 1, &value.x );               glCheck; }
    void operator()( const Math::ivec4& value ) const { glProgramUniform4iv          ( programID, location, 1, &value.x );               glCheck; }
    void operator()( const Math::fvec2& value ) const { glProgramUniform2fv          ( programID, location, 1, &value.x );               glCheck; }
    void operator()( const Math::fvec3& value ) const { glProgramUniform3fv          ( programID, location, 1, &value.x );               glCheck; }
    void operator()( const Math::fvec4& value ) const { glProgramUniform4fv          ( programID, location, 1, &value.x );               glCheck; }
    void operator()( const Math::fmat2& value ) const { glProgramUniformMatrix2fv    ( programID, location, 1, GL_FALSE, &value[0][0] ); glCheck; }
    void operator()( const Math::fmat3& value ) const { glProgramUniformMatrix3fv    ( programID, location, 1, GL_FALSE, &value[0][0] ); glCheck; }
    void operator()( const Math::fmat4& value ) const { glProgramUniformMatrix4fv    ( programID, location, 1, GL_FALSE, &value[0][0] ); glCheck; }
    
	// double-precision variants fallback to single-precision for better compatibility end performance
    void operator()( const Math::dvec2& value ) const { const Math::fvec2 fValue( value ); glProgramUniform2fv      ( programID, location, 1, &fValue.x ); glCheck; }
    void operator()( const Math::dvec3& value ) const { const Math::fvec3 fValue( value ); glProgramUniform3fv      ( programID, location, 1, &fValue.x );               glCheck; }
    void operator()( const Math::dvec4& value ) const { const Math::fvec4 fValue( value ); glProgramUniform4fv      ( programID, location, 1, &fValue.x );               glCheck; }
    void operator()( const Math::dmat2& value ) const { const Math::fmat2 fValue( value ); glProgramUniformMatrix2fv( programID, location, 1, GL_FALSE, &fValue[0][0] ); glCheck; }
    void operator()( const Math::dmat3& value ) const { const Math::fmat3 fValue( value ); glProgramUniformMatrix3fv( programID, location, 1, GL_FALSE, &fValue[0][0] ); glCheck; }
    void operator()( const Math::dmat4& value ) const { const Math::fmat4 fValue( value ); glProgramUniformMatrix4fv( programID, location, 1, GL_FALSE, &fValue[0][0] ); glCheck; }
};

// ------------------------------------------------------------------------------------------------
// DSA: Set uniform without binding program (OpenGL 4.1+)
// ------------------------------------------------------------------------------------------------
void Uniform::setDSA( unsigned int programID )
{
    if( !_changed )
        return;

    std::visit( UniformSetterDSA{ programID, _location }, _value );
    _changed = false;
}