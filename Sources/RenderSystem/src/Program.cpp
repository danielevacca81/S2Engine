// Program.cpp
//
#include "Program.h"

#include "OpenGL.h"
#include "OpenGLCheck.h"

#include "UniformInt.h"
#include "UniformBool.h"
#include "UniformFloat.h"
#include "UniformSampler.h"

#include "UniformFloatVector2.h"
#include "UniformFloatVector3.h"
#include "UniformFloatVector4.h"

#include "UniformFloatMatrix22.h"
#include "UniformFloatMatrix33.h"
#include "UniformFloatMatrix44.h"

#include <vector>
#include <sstream>
#include <iostream>

using namespace RenderSystem;

std::vector<Uniform*> Program::_unusedUniforms;

// ------------------------------------------------------------------------------------------------
ProgramPtr Program::New()
{
	return std::make_shared<Program>();
}

// ------------------------------------------------------------------------------------------------
Program::Program()
: _linked( false )
, _vshd( 0 )
, _gshd( 0 )
, _fshd( 0 )
{
	prepareUnusedUniforms();
	create();
}

// ------------------------------------------------------------------------------------------------
//Program::Program( Program &&other )
//: Program()
//{
//	std::swap( _vshd,       other._vshd );
//	std::swap( _gshd,       other._gshd );
//	std::swap( _fshd,       other._fshd );
//	std::swap( _linked,     other._linked );
//	std::swap( _name,       other._name );
//	std::swap( _attributes, other._attributes );
//	std::swap( _uniforms,   other._uniforms );
//
//	std::swap( _created,   other._created);
//	std::swap( _objectID,  other._objectID);
//}

// ------------------------------------------------------------------------------------------------
Program::~Program()
{
	destroy();
}

// ------------------------------------------------------------------------------------------------
//Program &Program::operator=( Program &&other )
//{
//	reset();
//
//	std::swap( _vshd,       other._vshd );
//	std::swap( _gshd,       other._gshd );
//	std::swap( _fshd,       other._fshd );
//	std::swap( _linked,     other._linked );
//	std::swap( _name,       other._name );
//	std::swap( _attributes, other._attributes );
//	std::swap( _uniforms,   other._uniforms );
//
//	std::swap( _created,   other._created);
//	std::swap( _objectID,  other._objectID);
//	return *this;
//}

// -------------------------------------------------------------------------------------------------
void Program::reset()
{
	OpenGLObject::reset();

	_vshd   = 0;
	_gshd   = 0;
	_fshd   = 0;
	_linked = false;
	_name   = "";

	for( auto &it : _uniforms )
		delete it.second;

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

	
	glDeleteShader( _vshd ); _vshd = 0;
	glDeleteShader( _fshd ); _fshd = 0;
	glDeleteShader( _gshd ); _gshd = 0;

	glDeleteProgram( _objectID );
	glCheck;

	// delete attributes?
	reset();
}

// ------------------------------------------------------------------------------------------------
//bool Program::isSupported()	{ return glewIsSupported("GL_ARB_shader_objects") == GL_TRUE; }

// ------------------------------------------------------------------------------------------------
std::string Program::name() const
{
	return _name;
}

// ------------------------------------------------------------------------------------------------
bool Program::attachVertexShader( const std::string &vertexSource )
{
	if( !isCreated() )
		return false;

	
	_vshd = glCreateShader( GL_VERTEX_SHADER );
	const char * s = vertexSource.c_str();
	glShaderSource( _vshd, 1, &s, 0 );
	glCompileShader( _vshd );

	GLint isCompiled = 0;
	glGetShaderiv( _vshd, GL_COMPILE_STATUS, &isCompiled );
	if( isCompiled == GL_FALSE )
	{
		GLint maxLength = 0;
		glGetShaderiv( _vshd, GL_INFO_LOG_LENGTH, &maxLength );

		std::vector<GLchar> infoLog( maxLength );
		glGetShaderInfoLog( _vshd, maxLength, &maxLength, &infoLog[0] );
		glDeleteShader( _vshd );

		std::cout << std::string( infoLog.begin(), infoLog.end() ) << '\n';
		return false;
	}

	glAttachShader( _objectID, _vshd );

	glCheck;
	return true;
}

// ------------------------------------------------------------------------------------------------
bool Program::attachFragmentShader( const std::string &fragmentSource )
{
	if( !isCreated() )
		return false;

	
	_fshd = glCreateShader( GL_FRAGMENT_SHADER );
	const char * s = fragmentSource.c_str();
	glShaderSource( _fshd, 1, &s, 0 );
	glCompileShader( _fshd );

	GLint isCompiled = 0;
	glGetShaderiv( _fshd, GL_COMPILE_STATUS, &isCompiled );
	if( isCompiled == GL_FALSE )
	{
		GLint maxLength = 0;
		glGetShaderiv( _fshd, GL_INFO_LOG_LENGTH, &maxLength );

		std::vector<GLchar> infoLog( maxLength );
		glGetShaderInfoLog( _fshd, maxLength, &maxLength, &infoLog[0] );
		glDeleteShader( _fshd );

		std::cout << std::string( infoLog.begin(), infoLog.end() ) << '\n';
		return false;
	}

	glAttachShader( _objectID, _fshd );

	glCheck;
	return true;
}

// ------------------------------------------------------------------------------------------------
bool Program::attachGeometryShader( const std::string &geometrySource )
{
	if( !isCreated() )
		return false;

	
	_gshd = glCreateShader( GL_GEOMETRY_SHADER );
	const char * s = geometrySource.c_str();
	glShaderSource( _gshd, 1, &s, 0 );
	glCompileShader( _gshd );

	GLint isCompiled = 0;
	glGetShaderiv( _gshd, GL_COMPILE_STATUS, &isCompiled );
	if( isCompiled == GL_FALSE )
	{
		GLint maxLength = 0;
		glGetShaderiv( _gshd, GL_INFO_LOG_LENGTH, &maxLength );

		std::vector<GLchar> infoLog( maxLength );
		glGetShaderInfoLog( _gshd, maxLength, &maxLength, &infoLog[0] );
		glDeleteShader( _gshd );
		
		std::cout << std::string( infoLog.begin(), infoLog.end() ) << '\n';
		return false;
	}

	glAttachShader( _objectID, _gshd );

	glCheck;
	return true;
}

// ------------------------------------------------------------------------------------------------
bool Program::link( const std::string &name /* = std::string("") */ )
{
	if( !isCreated() )
		return false;

	if( isLinked() )
		return true;

	_name = name;
	if( _name.empty() )
		_name = "Shader_" + std::to_string( _objectID );

	findUniforms();

	
	glLinkProgram( _objectID );

	GLint ls = GL_FALSE;
	glGetProgramiv( _objectID, GL_LINK_STATUS, &ls );
	glCheck;

	_linked = ls == GL_TRUE;

	if( !_linked )
	{
		GLint maxLength = 0;
		glGetProgramiv( _objectID, GL_INFO_LOG_LENGTH, &maxLength );

		std::vector<GLchar> infoLog( maxLength );
		glGetProgramInfoLog( _objectID, maxLength, &maxLength, &infoLog[0] );
		glCheck;

		std::cout << std::string( infoLog.begin(), infoLog.end() ) << '\n';
	}
	else
		setLabel( _name );

	findUniforms();
	return _linked;
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

// ------------------------------------------------------------------------------------------------
Uniform *Program::createUniform( const std::string &name, unsigned int loc, unsigned int type )
{
	switch( type )
	{
		/*TODO: double types*/

	case GL_FLOAT:           return new UniformFloat( loc, name );
	case GL_FLOAT_VEC2:	     return new UniformFloatVector2( loc, name );
	case GL_FLOAT_VEC3:      return new UniformFloatVector3( loc, name );
	case GL_FLOAT_VEC4:      return new UniformFloatVector4( loc, name );

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

	case GL_SAMPLER_2D:
	case GL_INT_SAMPLER_2D:
	case GL_SAMPLER_CUBE:    return new UniformSampler( loc, name );
	}

	return 0;
	// A new Uniform derived class needs to be added to support this uniform type.
	//throw new NotSupportedException("An implementation for uniform type " + type.ToString() + " does not exist.");
}

// ------------------------------------------------------------------------------------------------
static inline std::string extraInfo( int programID, bool attrib )
{
	auto GL_type_to_string = [] ( GLenum type ) -> std::string
	{
		switch( type )
		{
		case GL_BOOL: return "bool";
		case GL_INT: return "int";
		case GL_FLOAT: return "float";
		case GL_FLOAT_VEC2: return "vec2";
		case GL_FLOAT_VEC3: return "vec3";
		case GL_FLOAT_VEC4: return "vec4";
		case GL_FLOAT_MAT2: return "mat2";
		case GL_FLOAT_MAT3: return "mat3";
		case GL_FLOAT_MAT4: return "mat4";
		case GL_SAMPLER_2D: return "sampler2D";
		case GL_SAMPLER_3D: return "sampler3D";
		case GL_SAMPLER_CUBE: return "samplerCube";
		case GL_SAMPLER_2D_SHADOW: return "sampler2DShadow";
		default: break;
		}
		return "other";
	};

	int params =-1;

	if( attrib ) glGetProgramiv( programID, GL_ACTIVE_ATTRIBUTES, &params );
	else         glGetProgramiv( programID, GL_ACTIVE_UNIFORMS, &params );
	glCheck;

	std::stringstream msg;
	for( int i = 0; i < params; i++ )
	{
		char name[64];
		int max_length = 64;
		int actual_length = 0;
		int size = 0;

		GLenum type;
		if( attrib )  glGetActiveAttrib( programID, i, max_length, &actual_length, &size, &type, name );
		else          glGetActiveUniform( programID, i, max_length, &actual_length, &size, &type, name );
		glCheck;

		if( size > 1 )
		{
			for( int j = 0; j < size; j++ )
			{
				std::string longName = "[" + std::to_string( j ) + "]";
				int location = 0;
				if( attrib ) location = glGetAttribLocation( programID, longName.c_str() );
				else         location = glGetUniformLocation( programID, longName.c_str() );
				glCheck;
				msg << "  loc " << location << "] "
					<< GL_type_to_string( type )
					<< " "
					<< longName
					<< std::endl;
			}
		}
		else
		{
			int location = 0;
			if( attrib ) location = glGetAttribLocation( programID, name );
			else         location = glGetUniformLocation( programID, name );
			glCheck;

			msg << "  loc " << location << "] "
				<< GL_type_to_string( type )
				<< " "
				<< name
				<< std::endl;
		}
	}
	return msg.str();
}

// ------------------------------------------------------------------------------------------------
std::string Program::info( bool verbose ) const
{
	
	std::stringstream msg;

	if( !verbose )
	{
		GLint len = 0;
		glGetProgramiv( _objectID, GL_INFO_LOG_LENGTH, &len );

		std::vector<GLchar> errorLog( len + 1 );
		glGetProgramInfoLog( _objectID, len, &len, &errorLog[0] );

		msg << _name << " info:" << std::endl
			<< std::string( errorLog.begin(), errorLog.end() ) << std::endl;
	}
	else
	{
		msg << _name << " info" << std::endl
			<< "--------------------" << std::endl;

		if( _vshd > 0 )
		{
			int isCompiled = 0;
			glGetShaderiv( _vshd, GL_COMPILE_STATUS, &isCompiled );

			int len = 0;
			glGetShaderiv( _vshd, GL_INFO_LOG_LENGTH, &len );

			std::vector<GLchar> errorLog( len + 1 );
			glGetShaderInfoLog( _vshd, len, &len, &errorLog[0] );

			msg << "Vertex Shader Log:" << std::endl << std::string( errorLog.begin(), errorLog.end() ) << std::endl;
		}

		if( _fshd > 0 )
		{
			int isCompiled = 0;
			glGetShaderiv( _fshd, GL_COMPILE_STATUS, &isCompiled );

			int len = 0;
			glGetShaderiv( _fshd, GL_INFO_LOG_LENGTH, &len );

			std::vector<GLchar> errorLog( len + 1 );
			glGetShaderInfoLog( _fshd, len, &len, &errorLog[0] );

			msg << "Fragment Shader Log:" << std::endl << std::string( errorLog.begin(), errorLog.end() ) << std::endl;
		}

		if( _gshd > 0 )
		{
			int isCompiled = 0;
			glGetShaderiv( _gshd, GL_COMPILE_STATUS, &isCompiled );

			int len = 0;
			glGetShaderiv( _gshd, GL_INFO_LOG_LENGTH, &len );

			std::vector<GLchar> errorLog( len + 1 );
			glGetShaderInfoLog( _gshd, len, &len, &errorLog[0] );

			msg << "Geometry Shader Log:" << std::endl << std::string( errorLog.begin(), errorLog.end() ) << std::endl;
		}

		int params = -1;
		glGetProgramiv( _objectID, GL_LINK_STATUS, &params );
		msg << "GL_LINK_STATUS = " << params << std::endl;

		glGetProgramiv( _objectID, GL_ATTACHED_SHADERS, &params );
		msg << "GL_ATTACHED_SHADERS = " << params << std::endl;

		glGetProgramiv( _objectID, GL_ACTIVE_ATTRIBUTES, &params );
		msg << "GL_ACTIVE_ATTRIBUTES = " << params << std::endl;
		msg << extraInfo( _objectID, true ) << std::endl;

		glGetProgramiv( _objectID, GL_ACTIVE_UNIFORMS, &params );
		msg << "GL_ACTIVE_UNIFORMS = " << params << std::endl;
		msg << extraInfo( _objectID, false ) << std::endl;
	}

	glCheck;
	return msg.str();
}

// ------------------------------------------------------------------------------------------------
void Program::prepareUnusedUniforms()
{
	if( !_unusedUniforms.empty() )
		return;

	_unusedUniforms.emplace_back( createUniform( "UnusedFLOAT", -1, GL_FLOAT ) );
	_unusedUniforms.emplace_back( createUniform( "UnusedFLOAT_VEC2", -1, GL_FLOAT_VEC2 ) );
	_unusedUniforms.emplace_back( createUniform( "UnusedFLOAT_VEC3", -1, GL_FLOAT_VEC3 ) );
	_unusedUniforms.emplace_back( createUniform( "UnusedFLOAT_VEC4", -1, GL_FLOAT_VEC4 ) );

	_unusedUniforms.emplace_back( createUniform( "UnusedINT",  -1, GL_INT ) );
	_unusedUniforms.emplace_back( createUniform( "UnusedBOOL", -1, GL_BOOL ) );

	_unusedUniforms.emplace_back( createUniform( "UnusedFLOAT_MAT2", -1, GL_FLOAT_MAT2 ) );
	_unusedUniforms.emplace_back( createUniform( "UnusedFLOAT_MAT3", -1, GL_FLOAT_MAT3 ) );
	_unusedUniforms.emplace_back( createUniform( "UnusedFLOAT_MAT4", -1, GL_FLOAT_MAT4 ) );
	
	
	_unusedUniforms.emplace_back( createUniform( "UnusedSAMPLER",      -1, GL_SAMPLER_2D ) );
	_unusedUniforms.emplace_back( createUniform( "UnusedSAMPLER_CUBE", -1, GL_SAMPLER_CUBE ) );
}

// -------------------------------------------------------------------------------------------------
int Program::objectLabelIdentifier() const 
{
	return GL_PROGRAM;
}
