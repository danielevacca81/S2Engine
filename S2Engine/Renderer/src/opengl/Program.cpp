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
	
		glDeleteShader(_vshd);
		return false;
	}

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
	
		glDeleteShader(_fshd);
		return false;
	}

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
	
		glDeleteShader(_gshd);
		return false;
	}

	glAttachShader(_progID, _gshd);

	return true;
}

// ------------------------------------------------------------------------------------------------
bool Program::compile( const std::string &name /* = std::string("") */ )
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
std::string Program::info() const
{
	GLint len = 0;

	glGetProgramiv(_progID, GL_INFO_LOG_LENGTH, &len);

	char * ch = new char[len + 1];

	glGetProgramInfoLog(_progID, len, &len, ch);
	ch[len] = '\0';

	std::string log = ch;
	delete [] ch;

	if( log.empty() )
		return std::string("No Errors");
	
	return log;
}

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
		GLchar *uniformInternalName = new GLchar[uniformNameMaxLength];
		glGetActiveUniform( _progID, i, uniformNameMaxLength, &uniformNameLength, &uniformSize, &uniformType, uniformInternalName );

		const std::string uniformName(uniformInternalName); // TODO: need to correct ATI names
		delete [] uniformInternalName;

		if( uniformName.find_first_of("gl_") == 0 )
		{
			// Names starting with the reserved prefix of "gl_" have a location of -1.
			continue;
		}

		// OpenGL 3.1 or above
		// -------------------
		// Skip uniforms in a named block
		//int uniformBlockIndex;
		//glGetActiveUniformsiv(_progID, 1, ref i, ActiveUniformParameter.UniformBlockIndex, out uniformBlockIndex);
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
