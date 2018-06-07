// Program.cpp
//
#include "Program.h"

#include "OpenGL.h"

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

#include <sstream>

using namespace OpenGL;

// ------------------------------------------------------------------------------------------------
Program::Program()
: _linked(false)
, _progID(0)
, _vshd(0)
, _gshd(0)
, _fshd(0)
{
//	create();
}

// ------------------------------------------------------------------------------------------------
bool Program::create()
{
	if( _progID != 0 )
		return true;
	
	//if( isSupported() )
	_progID = glCreateProgram();
		
	if( _name.empty() )
		_name = "Shader";

	return _progID != 0;
}

// ------------------------------------------------------------------------------------------------
Program::~Program()
{
	if( isValid() )
		glDeleteProgram(_progID);

	for( auto it = _uniforms.begin();
		 it != _uniforms.end();
		 ++it )
		 delete it->second;
}

// ------------------------------------------------------------------------------------------------
void Program::detatchAll()
{
	if(_progID == 0) return;

	detatch(_vshd);
	detatch(_fshd);
	detatch(_gshd);
}

// ------------------------------------------------------------------------------------------------
void Program::detatch( unsigned int &shd )
{
	if( shd ) glDeleteShader(shd); shd = 0;
}

// ------------------------------------------------------------------------------------------------
//bool Program::isSupported()	{ return glewIsSupported("GL_ARB_shader_objects") == GL_TRUE; }

// ------------------------------------------------------------------------------------------------
std::string Program::name() const
{
	return _name;
}

// ------------------------------------------------------------------------------------------------
bool Program::attachVertexShader(const std::string &vertexSource)
{
	if( !create() )
		return false;

	_vshd = glCreateShader(GL_VERTEX_SHADER);
	const char * s = vertexSource.c_str();
	glShaderSource(_vshd, 1, &s, 0);
	glCompileShader(_vshd);

	GLint isCompiled = 0;
	glGetShaderiv(_vshd, GL_COMPILE_STATUS, &isCompiled);
	if( isCompiled == GL_FALSE )
	{
		GLint maxLength = 0;
		glGetShaderiv(_vshd, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(_vshd, maxLength, &maxLength, &infoLog[0]);

		_compileMessage = std::string( infoLog.begin(), infoLog.end() );
	
		glDeleteShader(_vshd);
		return false;
	}
	
	_compileMessage = "No compiler errors";

	glAttachShader(_progID, _vshd);

	return true;
}

// ------------------------------------------------------------------------------------------------
bool Program::attachFragmentShader(const std::string &fragmentSource)
{
	if( !create() )
		return false;

	_fshd = glCreateShader(GL_FRAGMENT_SHADER);
	const char * s = fragmentSource.c_str();
	glShaderSource(_fshd, 1, &s, 0);
	glCompileShader(_fshd);

	GLint isCompiled = 0;
	glGetShaderiv(_fshd, GL_COMPILE_STATUS, &isCompiled);
	if( isCompiled == GL_FALSE )
	{
		GLint maxLength = 0;
		glGetShaderiv(_fshd, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(_fshd, maxLength, &maxLength, &infoLog[0]);

		_compileMessage = std::string( infoLog.begin(), infoLog.end() );
	
		glDeleteShader(_fshd);
		return false;
	}

	_compileMessage = "No compiler errors";


	glAttachShader(_progID, _fshd);
	
	return true;
}

// ------------------------------------------------------------------------------------------------
bool Program::attachGeometryShader(const std::string &geometrySource)
{
	if( !create() )
		return false;

	_gshd = glCreateShader(GL_GEOMETRY_SHADER);
	const char * s = geometrySource.c_str();
	glShaderSource(_gshd, 1, &s, 0);
	glCompileShader(_gshd);

	GLint isCompiled = 0;
	glGetShaderiv(_gshd, GL_COMPILE_STATUS, &isCompiled);
	if( isCompiled == GL_FALSE )
	{
		GLint maxLength = 0;
		glGetShaderiv(_gshd, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(_gshd, maxLength, &maxLength, &infoLog[0]);

		_compileMessage = std::string( infoLog.begin(), infoLog.end() );
	
		glDeleteShader(_gshd);
		return false;
	}

	_compileMessage = "No compiler errors";

	glAttachShader(_progID, _gshd);

	return true;
}

// ------------------------------------------------------------------------------------------------
bool Program::link( const std::string &name /* = std::string("") */ )
{
	if( !create() )
		return false;

	if( isLinked() )
		return true;

	_name = name;
	if( _name.empty() )
	{
		std::stringstream ss;
		ss << "Shader_" << _progID;
		_name = ss.str();
	}

	findUniforms();

	glLinkProgram(_progID);

	GLint ls = GL_FALSE;
	glGetProgramiv(_progID, GL_LINK_STATUS, &ls);

	_linked = ls == GL_TRUE;

	if( _linked )
		findUniforms();

	return _linked;
}

// ------------------------------------------------------------------------------------------------
bool Program::isValid() const { return _progID != 0; }

// ------------------------------------------------------------------------------------------------
bool Program::isLinked() const	{ return _linked; }

// ------------------------------------------------------------------------------------------------
void Program::bind() const
{
	glUseProgram(_progID);
	
	for( auto it = _uniforms.begin();
		 it != _uniforms.end();
		 ++it )
		 it->second->set();
}

// ------------------------------------------------------------------------------------------------
void Program::unbind() const
{
	glUseProgram(0);

	for( std::map<std::string,unsigned int>::const_iterator it = _attributes.begin();
		 it != _attributes.end();
		 ++it )
		glDisableVertexAttribArray(it->second);
	//glVertexAttribPointer(location,size,type,normalized,stride,ptr);
}

// ------------------------------------------------------------------------------------------------
void Program::findUniforms()
{
	int numberOfUniforms;
	glGetProgramiv( _progID, GL_ACTIVE_UNIFORMS, &numberOfUniforms);

	int uniformNameMaxLength;
	glGetProgramiv( _progID, GL_ACTIVE_UNIFORM_MAX_LENGTH, &uniformNameMaxLength);

	for( int i = 0; i < numberOfUniforms; ++i )
	{
		int uniformNameLength;
		int uniformSize;
		GLenum uniformType;
		std::vector<GLchar> uniformInternalName(uniformNameMaxLength);
		glGetActiveUniform( _progID, i, uniformNameMaxLength, &uniformNameLength, &uniformSize, &uniformType, &uniformInternalName[0] );
		
		// TODO: need to correct ATI names
		const std::string uniformName( uniformInternalName.begin(), uniformInternalName.begin()+uniformNameLength ); 

		if( uniformName.find("gl_") == 0 )
		{
			// Names starting with the reserved prefix of "gl_" have a location of -1.
			continue;
		}

		// OpenGL 3.1 or above
		// -------------------
		// Skip uniforms in a named block
		//int uniformBlockIndex;
		//glGetActiveUniformsiv(_progID, 1, &i, ActiveUniformParameter.UniformBlockIndex, &uniformBlockIndex);
		//if (uniformBlockIndex != -1)
		//{
		//	continue;
		//}

		if( uniformSize != 1 )
		{
			// TODO:  Support arrays
			//throw new NotSupportedException("Uniform arrays are not supported.");
		}

		int uniformLocation = glGetUniformLocation( _progID, uniformName.c_str() );

		_uniforms[ uniformName ] = createUniform( uniformName, uniformLocation, uniformType);
	}
}

// ------------------------------------------------------------------------------------------------
Uniform *Program::createUniform( const std::string &name, unsigned int loc, unsigned int type )
{
	switch (type)
	{
	case GL_FLOAT:           return new UniformFloat( loc,name );
	case GL_FLOAT_VEC2:	     return new UniformFloatVector2( loc,name );
	case GL_FLOAT_VEC3:      return new UniformFloatVector3( loc,name );
	case GL_FLOAT_VEC4:      return new UniformFloatVector4( loc,name );
	
	case GL_INT:             return new UniformInt( loc,name );
	//case GL_INT_VEC2:        return new UniformIntVector2GL3x(name, location, this);
	//case GL_INT_VEC3:        return new UniformIntVector3GL3x(name, location, this);
	//case GL_INT_VEC4:        return new UniformIntVector4GL3x(name, location, this);

	case GL_BOOL:            return new UniformBool( loc,name );
	//case GL_BOOL_VEC2:       return new UniformBoolGL3x(name, location, this);
	//case GL_BOOL_VEC3:       return new UniformBoolGL3x(name, location, this);
	//case GL_BOOL_VEC4:       return new UniformBoolGL3x(name, location, this);

	case GL_FLOAT_MAT2:      return new UniformFloatMatrix22( loc,name );
	case GL_FLOAT_MAT3:      return new UniformFloatMatrix33( loc,name );
	case GL_FLOAT_MAT4:      return new UniformFloatMatrix44( loc,name );

	case GL_SAMPLER_2D:
	case GL_SAMPLER_CUBE:    return new UniformSampler( loc,name );
	}

	return 0;
	// A new Uniform derived class needs to be added to support this uniform type.
	//throw new NotSupportedException("An implementation for uniform type " + type.ToString() + " does not exist.");
}

// ------------------------------------------------------------------------------------------------
static inline std::string extraInfo( int programID, bool attrib )
{
	auto GL_type_to_string = [] (GLenum type) -> std::string
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
		
		if( size > 1 )
		{
			for( int j = 0; j < size; j++ )
			{
				std::stringstream ss;
				ss << name << "[" << j << "]";
				std::string longName = ss.str();
				int location = 0;
				if( attrib ) location = glGetAttribLocation( programID, longName.c_str() );
				else         location = glGetUniformLocation( programID, longName.c_str() );
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
		glGetProgramiv( _progID, GL_INFO_LOG_LENGTH, &len );

		std::vector<GLchar> errorLog(len+1);
		glGetProgramInfoLog( _progID, len, &len, &errorLog[0] );

		msg << _name << " info:" << std::endl
			<< _compileMessage << std::endl
			<< std::string(errorLog.begin(),errorLog.end()) << std::endl;
	}
	else
	{
		msg << _name << " info" << std::endl
			<< "--------------------" << std::endl;

		msg << _compileMessage << std::endl;

		if( _vshd > 0 )
		{
			int isCompiled = 0;
			glGetShaderiv(_vshd, GL_COMPILE_STATUS, &isCompiled);

			int len = 0;
			glGetShaderiv( _vshd, GL_INFO_LOG_LENGTH, &len );
			
			std::vector<GLchar> errorLog(len+1);
			glGetShaderInfoLog( _vshd, len, &len, &errorLog[0] );

			msg << "Vertex Shader Log:" << std::endl << std::string(errorLog.begin(),errorLog.end()) << std::endl;
		}

		if( _fshd > 0 )
		{
			int isCompiled = 0;
			glGetShaderiv(_fshd, GL_COMPILE_STATUS, &isCompiled);

			int len = 0;
			glGetShaderiv( _fshd, GL_INFO_LOG_LENGTH, &len );
			
			std::vector<GLchar> errorLog(len+1);
			glGetShaderInfoLog( _fshd, len, &len, &errorLog[0] );

			msg << "Fragment Shader Log:" << std::endl << std::string(errorLog.begin(),errorLog.end()) << std::endl;
		}

		if( _gshd > 0 )
		{
			int isCompiled = 0;
			glGetShaderiv(_gshd, GL_COMPILE_STATUS, &isCompiled);

			int len = 0;
			glGetShaderiv( _gshd, GL_INFO_LOG_LENGTH, &len );
			
			std::vector<GLchar> errorLog(len+1);
			glGetShaderInfoLog( _gshd, len, &len, &errorLog[0] );

			msg << "Geometry Shader Log:" << std::endl << std::string(errorLog.begin(),errorLog.end()) << std::endl;
		}

		int params = -1;
		glGetProgramiv( _progID, GL_LINK_STATUS, &params );
		msg << "GL_LINK_STATUS = "<< params <<std::endl;

		glGetProgramiv( _progID, GL_ATTACHED_SHADERS, &params );
		msg << "GL_ATTACHED_SHADERS = "<< params <<std::endl;

		glGetProgramiv( _progID, GL_ACTIVE_ATTRIBUTES, &params );
		msg << "GL_ACTIVE_ATTRIBUTES = " << params << std::endl;
		msg << extraInfo( _progID, true ) << std::endl;
		
		glGetProgramiv( _progID, GL_ACTIVE_UNIFORMS, &params );
		msg << "GL_ACTIVE_UNIFORMS = " << params << std::endl;		
		msg << extraInfo( _progID, false ) << std::endl;
	}
	return msg.str();
}


// ------------------------------------------------------------------------------------------------
//UniformValue *Program::uniform( const std::string &name )
//{
//	auto it = _uniforms.find(name);
//
//	if( it == _uniforms.end() )
//		return 0;
//
//	return it->second;
//}

// ------------------------------------------------------------------------------------------------
//void Program::setVertexAttribute( const std::string &attribName, int size, int type, bool normalized, int stride, const void *ptr )
//{
//	//if( _attributes.find(attribName) == _attributes.end() )
//	//{
//	GLint location = glGetAttribLocation( _progID, attribName.c_str() );
//
//	if( location < 0 )
//		return;
//	
//	glEnableVertexAttribArray(location);
//	glVertexAttribPointer(location,size,type,normalized,stride,ptr);
//
//	//	_attributes[attribName] = location;
//	//}
//	
//	//glEnableVertexAttribArray(_attributes[attribName]);
//	//glVertexAttribPointer(_attributes[attribName],size,type,normalized,stride,ptr);
//}

//// ------------------------------------------------------------------------------------------------
////void Program::addVertexAttribute( const std::string &attribName )
////{
////	GLint location = glGetAttribLocation( _progID, attribName.c_str() );
////
////	if( location > 0 )
////		_attributes[attribName] = location;
////}
//
//
//// ------------------------------------------------------------------------------------------------
//int Program::uniformLocation(const std::string &name) const {return glGetUniformLocation(_progID, name.c_str() );}
//
//// ------------------------------------------------------------------------------------------------
//// Single valued (int)
//void Program::uniform(const std::string &name, int x)                      const { glUniform1i( uniformLocation(name), x );          }
//void Program::uniform(const std::string &name, int x, int y)               const { glUniform2i( uniformLocation(name), x, y );       } 
//void Program::uniform(const std::string &name, int x, int y, int z)        const { glUniform3i( uniformLocation(name), x, y, z );    }
//void Program::uniform(const std::string &name, int x, int y, int z, int w) const { glUniform4i( uniformLocation(name), x, y, z, w ); }
//
//// ------------------------------------------------------------------------------------------------
//// Array values (int) 
//void Program::uniform1v(const std::string &name, int size, const int *x)    const	{ glUniform1iv( uniformLocation(name), size, x);    }
//void Program::uniform2v(const std::string &name, int size, const int *xy)   const	{ glUniform2iv( uniformLocation(name), size, xy);   }
//void Program::uniform3v(const std::string &name, int size, const int *xyz)  const   { glUniform3iv( uniformLocation(name), size, xyz);  }
//void Program::uniform4v(const std::string &name, int size, const int *xyzw) const   { glUniform4iv( uniformLocation(name), size, xyzw); }
//
//// ------------------------------------------------------------------------------------------------
//// Single valued (float)
//void Program::uniform(const std::string &name, float x)                            const { glUniform1f(uniformLocation(name), x); }
//void Program::uniform(const std::string &name, float x, float y)                   const { glUniform2f(uniformLocation(name), x, y); }
//void Program::uniform(const std::string &name, float x, float y, float z)          const { glUniform3f(uniformLocation(name), x, y, z); }
//void Program::uniform(const std::string &name, float x, float y, float z, float w) const { glUniform4f(uniformLocation(name), x, y, z, w); }
//
//// Array values (float)
//void Program::uniform1v(const std::string & name, int size, const float * x)     const { glUniform1fv(uniformLocation(name), size, x);    }
//void Program::uniform2v(const std::string & name, int size, const float * xy)	 const { glUniform2fv(uniformLocation(name), size, xy);   }
//void Program::uniform3v(const std::string & name, int size, const float * xyz)	 const { glUniform3fv(uniformLocation(name), size, xyz);  }
//void Program::uniform4v(const std::string & name, int size, const float * xyzw)  const { glUniform4fv(uniformLocation(name), size, xyzw); }
//
//// ------------------------------------------------------------------------------------------------
//// Matrix uniforms
//void Program::uniformMatrix2( const std::string &name, int count, bool transpose, const float *values ) const {glUniformMatrix2fv(uniformLocation(name),count,transpose,values);}
//void Program::uniformMatrix3( const std::string &name, int count, bool transpose, const float *values ) const {glUniformMatrix3fv(uniformLocation(name),count,transpose,values);}
//void Program::uniformMatrix4( const std::string &name, int count, bool transpose, const float *values ) const {glUniformMatrix4fv(uniformLocation(name),count,transpose,values);}

// ------------------------------------------------------------------------------------------------
bool Program::operator==( const Program &p ) const { return _progID == p._progID; }
bool Program::operator!=( const Program &p ) const { return _progID != p._progID; }
