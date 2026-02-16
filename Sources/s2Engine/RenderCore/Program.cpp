// Program.cpp
//
#include "Program.h"

#include "OpenGL.h"
#include "OpenGLCheck.h"
#include "OpenGLWrap.h"

#include <vector>
#include <sstream>
#include <iostream>

using namespace s2::RenderCore;

#pragma region Helpers
// ------------------------------------------------------------------------------------------------
static inline bool attach( GLenum shaderAttachType, unsigned int objID, unsigned int shaderHandle, ShaderType shaderType )
{
	if( objID == 0 || shaderHandle == 0 )
		return false;

	if( glWrap( shaderType ) != shaderAttachType )
		return false;

	glAttachShader( objID, shaderHandle );
	glCheck;
	return true;
}

// ------------------------------------------------------------------------------------------------
static inline Uniform* createUniform( const std::string& name, unsigned int loc, unsigned int type )
{
	switch( type )
	{
	case GL_FLOAT:           return new Uniform( loc, name, float{0.f} );
	case GL_FLOAT_VEC2:	     return new Uniform( loc, name, Math::fvec2{0.f, 0.f} );
	case GL_FLOAT_VEC3:      return new Uniform( loc, name, Math::fvec3{0.f, 0.f, 0.f} );
	case GL_FLOAT_VEC4:      return new Uniform( loc, name, Math::fvec4{0.f, 0.f, 0.f, 0.f} );

	case GL_INT:             return new Uniform( loc, name, int { 0 } );
	case GL_INT_VEC2:        assert( false ); break;
	case GL_INT_VEC3:        assert( false ); break;
	case GL_INT_VEC4:        assert( false ); break;

	case GL_BOOL:            return new Uniform( loc, name, bool { false } );
	case GL_BOOL_VEC2:       assert( false ); break;
	case GL_BOOL_VEC3:       assert( false ); break;
	case GL_BOOL_VEC4:       assert( false ); break;

	case GL_FLOAT_MAT2:      return new Uniform( loc, name, Math::fmat2( 1.f ) );
	case GL_FLOAT_MAT3:      return new Uniform( loc, name, Math::fmat3( 1.f ) );
	case GL_FLOAT_MAT4:      return new Uniform( loc, name, Math::fmat4( 1.f ) );

	case GL_SAMPLER_2D:
	case GL_INT_SAMPLER_2D:
	case GL_SAMPLER_CUBE:    return new Uniform( loc, name, int{0} );

	
	// OpenGL 4.0 or above
	case GL_DOUBLE:           return new Uniform( loc, name, double{0.0} );
	case GL_DOUBLE_VEC2:	  return new Uniform( loc, name, Math::dvec2{0.0, 0.0} );
	case GL_DOUBLE_VEC3:      return new Uniform( loc, name, Math::dvec3{0.0, 0.0, 0.0} );
	case GL_DOUBLE_VEC4:      return new Uniform( loc, name, Math::dvec4{0.0, 0.0, 0.0, 0.0} );

	case GL_DOUBLE_MAT2:      return new Uniform( loc, name, Math::dmat2(1.0) );
	case GL_DOUBLE_MAT3:      return new Uniform( loc, name, Math::dmat3(1.0) );
	case GL_DOUBLE_MAT4:      return new Uniform( loc, name, Math::dmat4(1.0) );

	default:
		assert( false && "Uniform type not supported" );
		return nullptr;
	}

	return nullptr;
}
#pragma endregion

// ------------------------------------------------------------------------------------------------
ProgramPtr Program::New()
{
	return std::make_shared<Program>();
}

// ------------------------------------------------------------------------------------------------
Program::Program()
{
	create();
}

// ------------------------------------------------------------------------------------------------
Program::~Program()
{
	destroy();
}

// -------------------------------------------------------------------------------------------------
void Program::reset()
{
	OpenGLObject::reset();

	_linked = false;
	_name   = "";

	for( auto &it : _uniforms )
		delete it.second;

	_uniforms.clear();
	_attributes.clear();
}

// ------------------------------------------------------------------------------------------------
void Program::create()
{
	destroy();
	OpenGLObject::create();

	//if( isSupported() )
	
	_objectID = glCreateProgram();
	glCheck;

	if( _name.empty() )
		_name = "Shader";

	_created = _objectID != 0;
}

// ------------------------------------------------------------------------------------------------
void Program::destroy()
{
	if( !isCreated() )
		return;

	//glCheck;
	
	_vshd = nullptr;
	_fshd = nullptr;
	_gshd = nullptr;
	_cshd = nullptr;
	_tshd = nullptr;
	_teshd = nullptr;

	glDeleteProgram( _objectID );
	glCheck;

	reset();
}

// ------------------------------------------------------------------------------------------------
std::string Program::name() const
{
	return _name;
}

// ------------------------------------------------------------------------------------------------
bool Program::attachVertexShader( const ShaderPtr &shader )
{
	if( !isCreated() )
		return false;
	
	if( !shader )
		return false;

	if( !attach( GL_VERTEX_SHADER, _objectID, shader->id(), shader->type() ) )
		return false;

	_vshd = shader;
	return true;
}

// ------------------------------------------------------------------------------------------------
bool Program::attachFragmentShader( const ShaderPtr &shader )
{
	if( !isCreated() )
		return false;

	if( !shader )
		return false;

	if( !attach( GL_FRAGMENT_SHADER, _objectID, shader->id(), shader->type() ) )
		return false;

	_fshd = shader;
	return true;
}

// ------------------------------------------------------------------------------------------------
bool Program::attachGeometryShader( const ShaderPtr &shader )
{
	if( !isCreated() )
		return false;

	if( !shader )
		return false;

	if( !attach( GL_GEOMETRY_SHADER, _objectID, shader->id(), shader->type() ) )
		return false;

	_gshd = shader;
	return true;
}

// ------------------------------------------------------------------------------------------------
bool Program::attachComputeShader( const ShaderPtr &shader )
{
	if( !isCreated() )
		return false;

	if( !shader )
		return false;

	if( shader->type() != ShaderType::Compute )
		return false; // assert ?

	if( !attach( GL_COMPUTE_SHADER, _objectID, shader->id(), shader->type() ) )
		return false;

	_cshd = shader;
	return true;
}

// ------------------------------------------------------------------------------------------------
bool Program::attachTessellationControlShader( const ShaderPtr &shader )
{
	if( !isCreated() )
		return false;

	if( !shader )
		return false;

	if( shader->type() != ShaderType::TessellationControl )
		return false; // assert ?

	if( !attach( GL_TESS_CONTROL_SHADER, _objectID, shader->id(), shader->type() ) )
		return false;

	_tshd = shader;
	return true;
}

// ------------------------------------------------------------------------------------------------
bool Program::attachTessellationEvaluationShader( const ShaderPtr &shader )
{
	if( !isCreated() )
		return false;

	if( !shader )
		return false;

	if( shader->type() != ShaderType::TessellationEvaluation )
		return false; // assert ?

	if( !attach( GL_TESS_EVALUATION_SHADER, _objectID, shader->id(), shader->type() ) )
		return false;

	_teshd = shader;
	return true;
}

// ------------------------------------------------------------------------------------------------
bool Program::isLinked() const { return _linked; }

// ------------------------------------------------------------------------------------------------
void Program::bind() const
{
	glUseProgram( _objectID );
	glCheck;
}

// ------------------------------------------------------------------------------------------------
void Program::unbind() const
{	
	glUseProgram( 0 );
	glCheck;

	for( auto &it : _attributes )
		glDisableVertexAttribArray( it.second );
	//glVertexAttribPointer(location,size,type,normalized,stride,ptr);
	glCheck;

}

// ------------------------------------------------------------------------------------------------
void Program::applyUniforms()
{
	// send uniform values to GPU if they have been changed
	for( auto &[name, uniform] : _uniforms )
		uniform->set();
}

// ------------------------------------------------------------------------------------------------
void Program::setUniformValue( const std::string& uniformName, const UniformValue& value )
{
	// store the value in the uniform object, 
	// it will be sent to GPU when applyUniforms() is called
	auto it = _uniforms.find( uniformName );
	if( it == _uniforms.end() )
		return;

	it->second->setValue( value );
}

// ------------------------------------------------------------------------------------------------
Uniform* Program::uniform( const std::string& name )
{
	auto it = _uniforms.find( name );
	if( it == _uniforms.end() )
		return nullptr;

	return it->second;
}

// ------------------------------------------------------------------------------------------------
void Program::findUniforms()
{
	// clear previous found uniforms
	for( auto &it : _uniforms )
		delete it.second;

	int numberOfUniforms;
	glGetProgramiv( _objectID, GL_ACTIVE_UNIFORMS, &numberOfUniforms );

	int uniformNameMaxLength;
	glGetProgramiv( _objectID, GL_ACTIVE_UNIFORM_MAX_LENGTH, &uniformNameMaxLength );
	glCheck;

	for( int i = 0; i < numberOfUniforms; ++i )
	{
		int uniformNameLength;
		int uniformSize;
		GLenum uniformType;
		std::vector<GLchar> uniformInternalName( uniformNameMaxLength );
		glGetActiveUniform( _objectID, i, uniformNameMaxLength, &uniformNameLength, &uniformSize, &uniformType, &uniformInternalName[0] );
		glCheck;

		// TODO: need to correct ATI names
		const std::string uniformName( uniformInternalName.begin(), uniformInternalName.begin() + uniformNameLength );

		if( uniformName.find( "gl_" ) == 0 )
		{
			// Names starting with the reserved prefix of "gl_" have a location of -1.
			continue;
		}

		// OpenGL 3.1 or above
		// -------------------
		// Skip uniforms in a named block
		//int uniformBlockIndex;
		//glGetActiveUniformsiv(_objectID, 1, &i, ActiveUniformParameter.UniformBlockIndex, &uniformBlockIndex);
		//if (uniformBlockIndex != -1)
		//{
		//	continue;
		//}

		if( uniformSize != 1 )
		{
			// TODO:  Support arrays
			//throw new NotSupportedException("Uniform arrays are not supported.");
		}

		int uniformLocation = glGetUniformLocation( _objectID, uniformName.c_str() );
		glCheck;

		_uniforms[uniformName] = createUniform( uniformName, uniformLocation, uniformType );
	}
}

// -------------------------------------------------------------------------------------------------
int Program::objectLabelIdentifier() const 
{
	return GL_PROGRAM;
}
