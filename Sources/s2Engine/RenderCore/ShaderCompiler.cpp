// ShaderCompiler.cpp
//
#include "ShaderCompiler.h"

#include "OpenGL.h"
#include "OpenGLCheck.h"

#include <fstream>
#include <sstream>

using namespace RenderCore;

// ------------------------------------------------------------------------------------------------
ShaderCompilationResult ShaderCompiler::compile( ShaderType type, const std::string& source )
{
	ShaderPtr shader = Shader::New( type );

	if( !shader )
		return ShaderCompilationResult { false, "Failed to create shader object" ,nullptr };

	const char* sourcePtr = source.c_str();
	glShaderSource( shader->id(), 1, &sourcePtr, nullptr );
	glCompileShader( shader->id() );

	GLint isCompiled = 0;
	glGetShaderiv( shader->id(), GL_COMPILE_STATUS, &isCompiled );

	if( isCompiled == GL_FALSE )
	{
		const std::string errorLog = ShaderCompiler::getShaderInfoLog( shader );
		shader = nullptr; // release shader object, resources will be freed by shared_ptr destructor

		return ShaderCompilationResult { false, errorLog, nullptr };
	}

	return ShaderCompilationResult { true, "", std::move( shader ) };
}

// ------------------------------------------------------------------------------------------------
ShaderCompilationResult ShaderCompiler::compileFromFile( ShaderType type, const std::filesystem::path& filepath )
{
	std::ifstream file( filepath );
	if( !file.is_open() )
		return ShaderCompilationResult { false,"Failed to open file: " + filepath.string(),nullptr };

	std::stringstream buffer;
	buffer << file.rdbuf();

	return compile( type, buffer.str() );
}

// ------------------------------------------------------------------------------------------------
ProgramLinkResult ShaderCompiler::linkProgram( const ProgramPtr& program, const std::string& name )
{
	if( !program || !program->isCreated() )
		return ProgramLinkResult { false,"Invalid program" };

	if( program->isLinked() )
		return ProgramLinkResult { true,"Already linked" };

	// find uniforms before linking to support shader subroutines (if any).
	// see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glGetActiveUniform.xhtml
	//program->findUniforms(); // needed???

	glLinkProgram( program->id() );

	GLint linkStatus = GL_FALSE;
	glGetProgramiv( program->id(), GL_LINK_STATUS, &linkStatus );
	glCheck;

	if( linkStatus == GL_FALSE )
	{
		const std::string errorLog = getProgramInfoLog( program );
		return ProgramLinkResult { false,errorLog };
	}

	program->_linked = true;
	program->setLabel( name );
	program->findUniforms();

	return ProgramLinkResult { true,"" };
}

// ------------------------------------------------------------------------------------------------
ProgramLinkResult ShaderCompiler::validateProgram( const ProgramPtr& program )
{
	if( !program )
		return ProgramLinkResult { false,"Invalid program" };

	glValidateProgram( program->id() );

	GLint validateStatus = GL_FALSE;
	glGetProgramiv( program->id(), GL_VALIDATE_STATUS, &validateStatus );
	glCheck;

	if( validateStatus == GL_FALSE )
	{
		const std::string errorLog = getProgramInfoLog( program );
		return ProgramLinkResult { false,errorLog };
	}

	return ProgramLinkResult { true,"" };
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
std::string ShaderCompiler::getProgramInfoLog( const ProgramPtr& program )
{
	GLint maxLength = 0;
	glGetProgramiv( program->id(), GL_INFO_LOG_LENGTH, &maxLength );

	if( maxLength == 0 )
		return "";

	std::vector<GLchar> infoLog( maxLength );
	glGetProgramInfoLog( program->id(), maxLength, &maxLength, infoLog.data() );

	return std::string( infoLog.begin(), infoLog.end() );
}

// ------------------------------------------------------------------------------------------------
std::string ShaderCompiler::getProgramInfo( const ProgramPtr& program, bool verbose )
{
	auto extraInfo = [] ( int programID, bool attrib )
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
	};

	std::stringstream msg;

	if( !verbose )
	{
		GLint len = 0;
		glGetProgramiv( program->id(), GL_INFO_LOG_LENGTH, &len );

		std::vector<GLchar> errorLog( len + 1 );
		glGetProgramInfoLog( program->id(), len, &len, errorLog.data() );

		msg << program->name() << " info:" << std::endl
			<< std::string( errorLog.begin(), errorLog.end() ) << std::endl;
	}
	else
	{
		msg << program->name() << " info" << std::endl
			<< "--------------------" << std::endl;

		auto shaderLog = [] ( const ShaderPtr& shader )
		{
			int isCompiled = 0;
			glGetShaderiv( shader->id(), GL_COMPILE_STATUS, &isCompiled );

			int len = 0;
			glGetShaderiv( shader->id(), GL_INFO_LOG_LENGTH, &len );

			std::vector<GLchar> errorLog( len + 1 );
			glGetShaderInfoLog( shader->id(), len, &len, errorLog.data() );

			return std::string( errorLog.begin(), errorLog.end() );
		};

		if( program->_vshd->isValid() ) msg << "Vertex Shader Log:" << shaderLog( program->_vshd ) << std::endl;
		if( program->_fshd->isValid() ) msg << "Fragment Shader Log:" << shaderLog( program->_fshd ) << std::endl;
		if( program->_gshd->isValid() ) msg << "Geometry Shader Log:" << shaderLog( program->_gshd ) << std::endl;
		if( program->_cshd->isValid() ) msg << "Compute Shader Log:" << shaderLog( program->_cshd ) << std::endl;
		if( program->_tshd->isValid() ) msg << "Tessellation Control Shader Log:" << shaderLog( program->_tshd ) << std::endl;
		if( program->_teshd->isValid() ) msg << "Tessellation Evaluation Shader Log:" << shaderLog( program->_teshd ) << std::endl;

		int params = -1;
		glGetProgramiv( program->id(), GL_LINK_STATUS, &params );
		msg << "GL_LINK_STATUS = " << params << std::endl;

		glGetProgramiv( program->id(), GL_ATTACHED_SHADERS, &params );
		msg << "GL_ATTACHED_SHADERS = " << params << std::endl;

		glGetProgramiv( program->id(), GL_ACTIVE_ATTRIBUTES, &params );
		msg << "GL_ACTIVE_ATTRIBUTES = " << params << std::endl;
		msg << extraInfo( program->id(), true ) << std::endl;

		glGetProgramiv( program->id(), GL_ACTIVE_UNIFORMS, &params );
		msg << "GL_ACTIVE_UNIFORMS = " << params << std::endl;
		msg << extraInfo( program->id(), false ) << std::endl;
	}

	glCheck;
	return msg.str();
}
