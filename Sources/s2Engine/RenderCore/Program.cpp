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
	case GL_FLOAT:           return new UniformFloat( loc, name );
	case GL_FLOAT_VEC2:	     return new UniformFloatVector2( loc, name );
	case GL_FLOAT_VEC3:      return new UniformFloatVector3( loc, name );
	case GL_FLOAT_VEC4:      return new UniformFloatVector4( loc, name );

		// OpenGL 4.0 or above
	case GL_DOUBLE:           return new UniformDouble( loc, name );
	case GL_DOUBLE_VEC2:	  return new UniformDoubleVector2( loc, name );
	case GL_DOUBLE_VEC3:      return new UniformDoubleVector3( loc, name );
	case GL_DOUBLE_VEC4:      return new UniformDoubleVector4( loc, name );

	case GL_INT:             return new UniformInt( loc, name );
	case GL_INT_VEC2:        assert( false ); break; //return new UniformIntVector2GL3x(name, location, this);
	case GL_INT_VEC3:        assert( false ); break; //return new UniformIntVector3GL3x(name, location, this);
	case GL_INT_VEC4:        assert( false ); break; //return new UniformIntVector4GL3x(name, location, this);

		//case GL_UNSIGNED_INT:             return new UniformUInt( loc, name );
		//case GL_UNSIGNED_INT_VEC2:        assert( false ); break; //return new UniformIntVector2GL3x(name, location, this);
		//case GL_UNSIGNED_INT_VEC3:        assert( false ); break; //return new UniformIntVector3GL3x(name, location, this);
		//case GL_UNSIGNED_INT_VEC4:        assert( false ); break; //return new UniformIntVector4GL3x(name, location, this);

	case GL_BOOL:            return new UniformBool( loc, name );
	case GL_BOOL_VEC2:       assert( false ); break; //return new UniformBoolGL3x(name, location, this);
	case GL_BOOL_VEC3:       assert( false ); break; //return new UniformBoolGL3x(name, location, this);
	case GL_BOOL_VEC4:       assert( false ); break; //return new UniformBoolGL3x(name, location, this);


	case GL_FLOAT_MAT2:      return new UniformFloatMatrix22( loc, name );
	case GL_FLOAT_MAT3:      return new UniformFloatMatrix33( loc, name );
	case GL_FLOAT_MAT4:      return new UniformFloatMatrix44( loc, name );

		// OpenGL 4.0 or above
	case GL_DOUBLE_MAT2:      return new UniformDoubleMatrix22( loc, name );
	case GL_DOUBLE_MAT3:      return new UniformDoubleMatrix33( loc, name );
	case GL_DOUBLE_MAT4:      return new UniformDoubleMatrix44( loc, name );

	case GL_SAMPLER_2D:
	case GL_INT_SAMPLER_2D:
	case GL_SAMPLER_CUBE:    return new UniformSampler( loc, name );

	default:
		assert( false && "Uniform type not supported" );
		return nullptr;
	}

	return nullptr;
	// A new Uniform derived class needs to be added to support this uniform type.
	//throw new NotSupportedException("An implementation for uniform type " + type.ToString() + " does not exist.");
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
void Program::applyUniforms() const
{
	for( auto &it : _uniforms )
		it.second->set();
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
