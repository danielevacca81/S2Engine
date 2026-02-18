// ShaderStage.cpp
//
#include "ShaderStage.h"

#include "OpenGL.h"
#include "OpenGLCheck.h"
#include "OpenGLWrap.h"

using namespace s2::RenderCore;

// -------------------------------------------------------------------------------------------------
ShaderStagePtr ShaderStage::New( const ShaderStageType& type ) { return std::make_shared<ShaderStage>( type ); }

// -------------------------------------------------------------------------------------------------
ShaderStage::ShaderStage( const ShaderStageType& type )
    : _type( type )
{
	create();
}

// -------------------------------------------------------------------------------------------------
ShaderStage::~ShaderStage()
{
	destroy();
}


// -------------------------------------------------------------------------------------------------
void ShaderStage::create()
{
	OpenGLObject::create();

	_objectID = glCreateShader( glWrap( _type ) );
	glCheck;
	
	_created = _objectID != 0;
}

// -------------------------------------------------------------------------------------------------
void ShaderStage::destroy()
{
	glDeleteShader( _objectID );
	glCheck;

	OpenGLObject::reset();
}

// -------------------------------------------------------------------------------------------------
int ShaderStage::objectLabelIdentifier() const
{
	return GL_SHADER;
}