// ShaderCompiler.cpp
//
#include "ShaderCompiler.h"

#include "OpenGL.h"
#include "OpenGLCheck.h"

#include <fstream>
#include <string>

using namespace s2::RenderCore;

// ------------------------------------------------------------------------------------------------
ShaderStageCompilationResult ShaderCompiler::compile( ShaderStageType type, const std::string& source )
{
	ShaderStagePtr stage = ShaderStage::New( type );

	if( !stage )
		return ShaderStageCompilationResult { false, "Failed to create shader object" ,nullptr };

	const char* sourcePtr = source.c_str();
	glShaderSource( stage->id(), 1, &sourcePtr, nullptr );
	glCompileShader( stage->id() );

	GLint isCompiled = 0;
	glGetShaderiv( stage->id(), GL_COMPILE_STATUS, &isCompiled );

	if( isCompiled == GL_FALSE )
	{
		const std::string errorLog = ShaderCompiler::getShaderStageInfoLog( stage );
		stage = nullptr; // release shader object, resources will be freed by shared_ptr destructor

		return ShaderStageCompilationResult { false, errorLog, nullptr };
	}

	return ShaderStageCompilationResult { true, "", std::move( stage ) };
}

// ------------------------------------------------------------------------------------------------
ShaderStageCompilationResult ShaderCompiler::compileFromFile( ShaderStageType type, const std::filesystem::path& filepath )
{
	std::ifstream file( filepath );
	if( !file.is_open() )
		return ShaderStageCompilationResult { false,"Failed to open file: " + filepath.string(),nullptr };

	const std::string content(
		(std::istreambuf_iterator<char>( file )),
		std::istreambuf_iterator<char>()
	);

	return compile( type, content );
}

// ------------------------------------------------------------------------------------------------
ShaderLinkResult ShaderCompiler::linkShader( const ShaderPtr& shader, const std::string& name )
{
	if( !shader || !shader->isCreated() )
		return ShaderLinkResult { false,"Invalid shader" };

	if( shader->isLinked() )
		return ShaderLinkResult { true,"Already linked" };

	// find uniforms before linking to support shader subroutines (if any)?
	// see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glGetActiveUniform.xhtml
	//program->findUniforms(); // needed???

	glLinkProgram( shader->id() );

	GLint linkStatus = GL_FALSE;
	glGetProgramiv( shader->id(), GL_LINK_STATUS, &linkStatus );
	glCheck;

	if( linkStatus == GL_FALSE )
	{
		const std::string errorLog = getShaderInfoLog( shader );
		return ShaderLinkResult { false,errorLog };
	}

	shader->_linked = true;
	shader->setLabel( name );
	shader->findUniforms();

	return ShaderLinkResult { true,"" };
}

// ------------------------------------------------------------------------------------------------
ShaderLinkResult ShaderCompiler::validateShader( const ShaderPtr& shader )
{
	if( !shader )
		return ShaderLinkResult { false,"Invalid shader" };

	glValidateProgram( shader->id() );

	GLint validateStatus = GL_FALSE;
	glGetProgramiv( shader->id(), GL_VALIDATE_STATUS, &validateStatus );
	glCheck;

	if( validateStatus == GL_FALSE )
	{
		const std::string errorLog = getShaderInfoLog( shader );
		return ShaderLinkResult { false,errorLog };
	}

	return ShaderLinkResult { true,"" };
}

// ------------------------------------------------------------------------------------------------
std::string ShaderCompiler::getShaderStageInfoLog( const ShaderStagePtr& stage )
{
	GLint maxLength = 0;
	glGetShaderiv( stage->id(), GL_INFO_LOG_LENGTH, &maxLength );

	if( maxLength == 0 )
		return "";

	std::vector<GLchar> infoLog( maxLength );
	glGetShaderInfoLog( stage->id(), maxLength, &maxLength, infoLog.data() );

	return std::string( infoLog.begin(), infoLog.end() );
}

// ------------------------------------------------------------------------------------------------
std::string ShaderCompiler::getShaderInfoLog( const ShaderPtr& shader )
{
	GLint maxLength = 0;
	glGetShaderiv( shader->id(), GL_INFO_LOG_LENGTH, &maxLength );

	if( maxLength == 0 )
		return "";

	std::vector<GLchar> infoLog( maxLength );
	glGetShaderInfoLog( shader->id(), maxLength, &maxLength, infoLog.data() );

	return std::string( infoLog.begin(), infoLog.end() );
}

// ------------------------------------------------------------------------------------------------
std::string ShaderCompiler::getShaderInfo( const ShaderPtr& shader, bool verbose )
{
	auto extraInfo = [] ( int shaderID, bool attrib )
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

		int params = -1;

		if( attrib ) glGetProgramiv( shaderID, GL_ACTIVE_ATTRIBUTES, &params );
		else         glGetProgramiv( shaderID, GL_ACTIVE_UNIFORMS, &params );
		glCheck;

		std::stringstream msg;
		for( int i = 0; i < params; i++ )
		{
			char name[64];
			int max_length = 64;
			int actual_length = 0;
			int size = 0;

			GLenum type;
			if( attrib )  glGetActiveAttrib( shaderID, i, max_length, &actual_length, &size, &type, name );
			else          glGetActiveUniform( shaderID, i, max_length, &actual_length, &size, &type, name );
			glCheck;

			if( size > 1 )
			{
				for( int j = 0; j < size; j++ )
				{
					std::string longName = "[" + std::to_string( j ) + "]";
					int location = 0;
					if( attrib ) location = glGetAttribLocation( shaderID, longName.c_str() );
					else         location = glGetUniformLocation( shaderID, longName.c_str() );
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
				if( attrib ) location = glGetAttribLocation( shaderID, name );
				else         location = glGetUniformLocation( shaderID, name );
				glCheck;

				msg << "  loc " << location << "] "
					<< GL_type_to_string( type )
					<< " "
					<< name
					<< std::endl;
			}
		}
		return msg.str();
	};

	std::stringstream msg;

	if( !verbose )
	{
		GLint len = 0;
		glGetShaderiv( shader->id(), GL_INFO_LOG_LENGTH, &len );

		std::vector<GLchar> errorLog( len + 1 );
		glGetShaderInfoLog( shader->id(), len, &len, errorLog.data() );

		msg << shader->name() << " info:" << std::endl
			<< std::string( errorLog.begin(), errorLog.end() ) << std::endl;
	}
	else
	{
		msg << shader->name() << " info" << std::endl
			<< "--------------------" << std::endl;

		auto shaderLog = [] ( const ShaderStagePtr& stage )
		{
			int isCompiled = 0;
			glGetShaderiv( stage->id(), GL_COMPILE_STATUS, &isCompiled );

			int len = 0;
			glGetShaderiv( stage->id(), GL_INFO_LOG_LENGTH, &len );

			std::vector<GLchar> errorLog( len + 1 );
			glGetShaderInfoLog( stage->id(), len, &len, errorLog.data() );

			return std::string( errorLog.begin(), errorLog.end() );
		};

		if( shader->_vshd->isValid() )  msg << "Vertex Shader Log:" <<                  shaderLog( shader->_vshd ) << std::endl;
		if( shader->_fshd->isValid() )  msg << "Fragment Shader Log:" <<                shaderLog( shader->_fshd ) << std::endl;
		if( shader->_gshd->isValid() )  msg << "Geometry Shader Log:" <<                shaderLog( shader->_gshd ) << std::endl;
		if( shader->_cshd->isValid() )  msg << "Compute Shader Log:" <<                 shaderLog( shader->_cshd ) << std::endl;
		if( shader->_tshd->isValid() )  msg << "Tessellation Control Shader Log:" <<    shaderLog( shader->_tshd ) << std::endl;
		if( shader->_teshd->isValid() ) msg << "Tessellation Evaluation Shader Log:" << shaderLog( shader->_teshd ) << std::endl;

		int params = -1;
		glGetProgramiv( shader->id(), GL_LINK_STATUS, &params );
		msg << "GL_LINK_STATUS = " << params << std::endl;

		glGetProgramiv( shader->id(), GL_ATTACHED_SHADERS, &params );
		msg << "GL_ATTACHED_SHADERS = " << params << std::endl;

		glGetProgramiv( shader->id(), GL_ACTIVE_ATTRIBUTES, &params );
		msg << "GL_ACTIVE_ATTRIBUTES = " << params << std::endl;
		msg << extraInfo( shader->id(), true ) << std::endl;

		glGetProgramiv( shader->id(), GL_ACTIVE_UNIFORMS, &params );
		msg << "GL_ACTIVE_UNIFORMS = " << params << std::endl;
		msg << extraInfo( shader->id(), false ) << std::endl;
	}

	glCheck;
	return msg.str();
}
