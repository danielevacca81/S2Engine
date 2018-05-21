// Program.cpp
//
#include "Program.h"

#include "OpenGL.h"

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

	glLinkProgram(_progID);

	GLint ls = GL_FALSE;
	glGetProgramiv(_progID, GL_LINK_STATUS, &ls);

	_linked = ls == GL_TRUE;

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

	std::stringstream ss;
	ss  << "******************************************" << std::endl
		<< _name << ":" << std::endl
		<< "------------------------------------------" << std::endl
		<< (log.empty() ? " No Error" : log)            << std::endl
		<< "******************************************" << std::endl
		<< std::endl << std::endl;

	return ss.str();
}

// ------------------------------------------------------------------------------------------------
void Program::bind() const
{
	glUseProgram(_progID);	
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
void Program::setVertexAttribute( const std::string &attribName, int size, int type, bool normalized, int stride, const void *ptr )
{
	//if( _attributes.find(attribName) == _attributes.end() )
	//{
	GLint location = glGetAttribLocation( _progID, attribName.c_str() );

	if( location < 0 )
		return;
	
	glEnableVertexAttribArray(location);
	glVertexAttribPointer(location,size,type,normalized,stride,ptr);

	//	_attributes[attribName] = location;
	//}
	
	//glEnableVertexAttribArray(_attributes[attribName]);
	//glVertexAttribPointer(_attributes[attribName],size,type,normalized,stride,ptr);
}

// ------------------------------------------------------------------------------------------------
//void Program::addVertexAttribute( const std::string &attribName )
//{
//	GLint location = glGetAttribLocation( _progID, attribName.c_str() );
//
//	if( location > 0 )
//		_attributes[attribName] = location;
//}


// ------------------------------------------------------------------------------------------------
int Program::uniformLocation(const std::string &name) const {return glGetUniformLocation(_progID, name.c_str() );}

// ------------------------------------------------------------------------------------------------
// Single valued (int)
void Program::uniform(const std::string &name, int x)                      const { glUniform1i( uniformLocation(name), x );          }
void Program::uniform(const std::string &name, int x, int y)               const { glUniform2i( uniformLocation(name), x, y );       } 
void Program::uniform(const std::string &name, int x, int y, int z)        const { glUniform3i( uniformLocation(name), x, y, z );    }
void Program::uniform(const std::string &name, int x, int y, int z, int w) const { glUniform4i( uniformLocation(name), x, y, z, w ); }

// ------------------------------------------------------------------------------------------------
// Array values (int) 
void Program::uniform1v(const std::string &name, int size, const int *x)    const	{ glUniform1iv( uniformLocation(name), size, x);    }
void Program::uniform2v(const std::string &name, int size, const int *xy)   const	{ glUniform2iv( uniformLocation(name), size, xy);   }
void Program::uniform3v(const std::string &name, int size, const int *xyz)  const   { glUniform3iv( uniformLocation(name), size, xyz);  }
void Program::uniform4v(const std::string &name, int size, const int *xyzw) const   { glUniform4iv( uniformLocation(name), size, xyzw); }

// ------------------------------------------------------------------------------------------------
// Single valued (float)
void Program::uniform(const std::string &name, float x)                            const { glUniform1f(uniformLocation(name), x); }
void Program::uniform(const std::string &name, float x, float y)                   const { glUniform2f(uniformLocation(name), x, y); }
void Program::uniform(const std::string &name, float x, float y, float z)          const { glUniform3f(uniformLocation(name), x, y, z); }
void Program::uniform(const std::string &name, float x, float y, float z, float w) const { glUniform4f(uniformLocation(name), x, y, z, w); }

// Array values (float)
void Program::uniform1v(const std::string & name, int size, const float * x)     const { glUniform1fv(uniformLocation(name), size, x);    }
void Program::uniform2v(const std::string & name, int size, const float * xy)	 const { glUniform2fv(uniformLocation(name), size, xy);   }
void Program::uniform3v(const std::string & name, int size, const float * xyz)	 const { glUniform3fv(uniformLocation(name), size, xyz);  }
void Program::uniform4v(const std::string & name, int size, const float * xyzw)  const { glUniform4fv(uniformLocation(name), size, xyzw); }

// ------------------------------------------------------------------------------------------------
// Matrix uniforms
void Program::uniformMatrix2( const std::string &name, int count, bool transpose, const float *values ) const {glUniformMatrix2fv(uniformLocation(name),count,transpose,values);}
void Program::uniformMatrix3( const std::string &name, int count, bool transpose, const float *values ) const {glUniformMatrix3fv(uniformLocation(name),count,transpose,values);}
void Program::uniformMatrix4( const std::string &name, int count, bool transpose, const float *values ) const {glUniformMatrix4fv(uniformLocation(name),count,transpose,values);}

// ------------------------------------------------------------------------------------------------
bool Program::operator==( const Program &p ) const { return _progID == p._progID; }
bool Program::operator!=( const Program &p ) const { return _progID != p._progID; }
