// Shader.cpp
//
#include "Shader.h"

#include "OpenGL.h"
#include "OpenGLCheck.h"
#include "OpenGLWrap.h"

using namespace s2::RenderCore;

// -------------------------------------------------------------------------------------------------
ShaderPtr Shader::New( const ShaderType& type ) { return std::make_shared<Shader>( type ); }

// -------------------------------------------------------------------------------------------------
Shader::Shader( const ShaderType& type )
    : _type( type )
{
	create();
}

// -------------------------------------------------------------------------------------------------
Shader::~Shader()
{
	destroy();
}


// -------------------------------------------------------------------------------------------------
void Shader::create()
{
	OpenGLObject::create();

	_objectID = glCreateShader( glWrap( _type ) );
	glCheck;
	
	_created = _objectID != 0;
}

// -------------------------------------------------------------------------------------------------
void Shader::destroy()
{
	glDeleteShader( _objectID );
	glCheck;

	OpenGLObject::reset();
}

// -------------------------------------------------------------------------------------------------
int Shader::objectLabelIdentifier() const
{
	return GL_SHADER;
}