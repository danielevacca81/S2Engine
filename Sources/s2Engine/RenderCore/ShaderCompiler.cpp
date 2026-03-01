// ShaderCompiler.cpp
//
#include "ShaderCompiler.h"

#include "OpenGL.h"
#include "OpenGLCheck.h"
#include "OpenGLWrap.h"

#include <fstream>
#include <sstream>
#include <cassert>

using namespace s2::RenderCore;

// ------------------------------------------------------------------------------------------------
ShaderStageCompilationResult ShaderCompiler::compile( ShaderStageType type, const std::string& source )
{
    ShaderStagePtr stage = ShaderStage::New( type );

    if( !stage )
        return ShaderStageCompilationResult { false, "Failed to create shader object", nullptr };

    const char* sourcePtr = source.c_str();
    glShaderSource( stage->id(), 1, &sourcePtr, nullptr );
    glCheck;
    
    glCompileShader( stage->id() );
    glCheck;

    GLint isCompiled = 0;
    glGetShaderiv( stage->id(), GL_COMPILE_STATUS, &isCompiled );
    glCheck;

    if( isCompiled == GL_FALSE )
    {
        const std::string errorLog = getShaderStageInfoLog( stage );
        return ShaderStageCompilationResult { false, errorLog, nullptr };
    }

    return ShaderStageCompilationResult { true, "", std::move( stage ) };
}

// ------------------------------------------------------------------------------------------------
ShaderStageCompilationResult ShaderCompiler::compileFromFile( 
    ShaderStageType type, 
    const std::filesystem::path& filepath )
{
    std::ifstream file( filepath );
    if( !file.is_open() )
        return ShaderStageCompilationResult { false, "Failed to open file: " + filepath.string(), nullptr };

    const std::string content(
        (std::istreambuf_iterator<char>( file )),
        std::istreambuf_iterator<char>()
    );

    return compile( type, content );
}

// ------------------------------------------------------------------------------------------------
ShaderLinkResult ShaderCompiler::linkShader( const ShaderPtr& shader, const std::string& name )
{
    if( !shader || !shader->isValid() )
        return ShaderLinkResult { false, "Invalid shader" };

    if( shader->isLinked() )
        return ShaderLinkResult { true, "Already linked" };

    // Link program
    glLinkProgram( shader->id() );
    glCheck;

    GLint linkStatus = GL_FALSE;
    glGetProgramiv( shader->id(), GL_LINK_STATUS, &linkStatus );
    glCheck;

    if( linkStatus == GL_FALSE )
    {
        const std::string errorLog = getShaderInfoLog( shader );
        return ShaderLinkResult { false, errorLog };
    }

    shader->_linked = true;
    
    // Set debug label
    if( !name.empty() )
    {
        shader->_name = name;
        shader->setObjectLabel( name );
    }

    // Find uniforms after linking
    shader->findUniforms();

    return ShaderLinkResult { true, "" };
}

// ------------------------------------------------------------------------------------------------
ShaderLinkResult ShaderCompiler::validateShader( const ShaderPtr& shader )
{
    if( !shader )
        return ShaderLinkResult { false, "Invalid shader" };

    glValidateProgram( shader->id() );
    glCheck;

    GLint validateStatus = GL_FALSE;
    glGetProgramiv( shader->id(), GL_VALIDATE_STATUS, &validateStatus );
    glCheck;

    if( validateStatus == GL_FALSE )
    {
        const std::string errorLog = getShaderInfoLog( shader );
        return ShaderLinkResult { false, errorLog };
    }

    return ShaderLinkResult { true, "" };
}

// ------------------------------------------------------------------------------------------------
// Separable Shaders (OpenGL 4.1+)
// ------------------------------------------------------------------------------------------------

void ShaderCompiler::setSeparable( const ShaderPtr& shader, bool separable )
{
    assert( shader && shader->isValid() && "Shader must be created" );
    assert( !shader->isLinked() && "Must be set before linking" );

    // Mark shader as separable (OpenGL 4.1+)
    glProgramParameteri( shader->id(), GL_PROGRAM_SEPARABLE, separable ? GL_TRUE : GL_FALSE );
    glCheck;
}

// ------------------------------------------------------------------------------------------------
SeparableShaderResult ShaderCompiler::createSeparableShader(
    ShaderStageType type,
    const std::string& source,
    const std::string& name )
{
    // Compile shader stage
    auto compileResult = compile( type, source );
    if( !compileResult )
        return SeparableShaderResult { false, compileResult.errorLog, nullptr };

    // Create shader program
    auto shader = Shader::New();
    
    // Mark as separable BEFORE attaching stages
    setSeparable( shader, true );

    // Attach stage
    bool attached = false;
    switch( type )
    {
    case ShaderStageType::Vertex:
        attached = shader->attachVertexShaderStage( compileResult.stage );
        break;
    case ShaderStageType::Fragment:
        attached = shader->attachFragmentShaderStage( compileResult.stage );
        break;
    case ShaderStageType::Geometry:
        attached = shader->attachGeometryShaderStage( compileResult.stage );
        break;
    case ShaderStageType::Compute:
        attached = shader->attachComputeShaderStage( compileResult.stage );
        break;
    case ShaderStageType::TessellationControl:
        attached = shader->attachTessellationControlShaderStage( compileResult.stage );
        break;
    case ShaderStageType::TessellationEvaluation:
        attached = shader->attachTessellationEvaluationShaderStage( compileResult.stage );
        break;
    }

    if( !attached )
        return SeparableShaderResult { false, "Failed to attach shader stage", nullptr };

    // Link
    auto linkResult = linkShader( shader, name );
    if( !linkResult )
        return SeparableShaderResult { false, linkResult.errorLog, nullptr };

    return SeparableShaderResult { true, "", shader };
}

// ------------------------------------------------------------------------------------------------
SeparableShaderResult ShaderCompiler::createSeparableShaderFromFile(
    ShaderStageType type,
    const std::filesystem::path& filepath,
    const std::string& name )
{
    std::ifstream file( filepath );
    if( !file.is_open() )
        return SeparableShaderResult { false, "Failed to open file: " + filepath.string(), nullptr };

    const std::string content(
        (std::istreambuf_iterator<char>( file )),
        std::istreambuf_iterator<char>()
    );

    const std::string shaderName = name.empty() ? filepath.filename().string() : name;
    return createSeparableShader( type, content, shaderName );
}

// ------------------------------------------------------------------------------------------------
// Shader Binary Support (OpenGL 4.1+)
// ------------------------------------------------------------------------------------------------

std::optional<std::vector<uint8_t>> ShaderCompiler::getBinaryShader( const ShaderPtr& shader )
{
    assert( shader && shader->isLinked() && "Shader must be linked" );

    GLint binaryLength = 0;
    glGetProgramiv( shader->id(), GL_PROGRAM_BINARY_LENGTH, &binaryLength );
    glCheck;

    if( binaryLength == 0 )
        return std::nullopt;

    std::vector<uint8_t> binary( binaryLength );
    GLenum binaryFormat = 0;
    GLsizei length = 0;

    glGetProgramBinary( 
        shader->id(), 
        binaryLength, 
        &length, 
        &binaryFormat, 
        binary.data() 
    );
    glCheck;

    binary.resize( length );
    return binary;
}

// ------------------------------------------------------------------------------------------------
ShaderLinkResult ShaderCompiler::loadFromBinary( 
    const ShaderPtr& shader, 
    const std::vector<uint8_t>& binary,
    uint32_t binaryFormat )
{
    assert( shader && shader->isValid() && "Shader must be created" );
    assert( !binary.empty() && "Binary data cannot be empty" );

    glProgramBinary( 
        shader->id(), 
        binaryFormat, 
        binary.data(), 
        static_cast<GLsizei>( binary.size() ) 
    );
    glCheck;

    GLint linkStatus = GL_FALSE;
    glGetProgramiv( shader->id(), GL_LINK_STATUS, &linkStatus );
    glCheck;

    if( linkStatus == GL_FALSE )
    {
        const std::string errorLog = getShaderInfoLog( shader );
        return ShaderLinkResult { false, errorLog };
    }

    shader->_linked = true;
    shader->findUniforms();

    return ShaderLinkResult { true, "" };
}

// ------------------------------------------------------------------------------------------------
std::string ShaderCompiler::getShaderStageInfoLog( const ShaderStagePtr& stage )
{
    if( !stage || !stage->isValid() )
        return "";

    GLint maxLength = 0;
    glGetShaderiv( stage->id(), GL_INFO_LOG_LENGTH, &maxLength );
    glCheck;

    if( maxLength == 0 )
        return "";

    std::vector<GLchar> infoLog( maxLength );
    glGetShaderInfoLog( stage->id(), maxLength, &maxLength, infoLog.data() );
    glCheck;

    return std::string( infoLog.begin(), infoLog.end() );
}

// ------------------------------------------------------------------------------------------------
std::string ShaderCompiler::getShaderInfoLog( const ShaderPtr& shader )
{
    if( !shader || !shader->isValid() )
        return "";

    GLint maxLength = 0;
    glGetProgramiv( shader->id(), GL_INFO_LOG_LENGTH, &maxLength );
    glCheck;

    if( maxLength == 0 )
        return "";

    std::vector<GLchar> infoLog( maxLength );
    glGetProgramInfoLog( shader->id(), maxLength, &maxLength, infoLog.data() );
    glCheck;

    return std::string( infoLog.begin(), infoLog.end() );
}

// ------------------------------------------------------------------------------------------------
std::string ShaderCompiler::getShaderInfo( const ShaderPtr& shader, bool verbose )
{
    if( !shader )
        return "Invalid shader";

    std::stringstream msg;

    if( !verbose )
    {
        // Simple info log
        msg << shader->name() << " info:" << std::endl
            << getShaderInfoLog( shader ) << std::endl;
    }
    else
    {
        // Detailed info
        msg << shader->name() << " info" << std::endl
            << "--------------------" << std::endl;

        auto shaderStageLog = []( const ShaderStagePtr& stage ) -> std::string
        {
            if( !stage || !stage->isValid() )
                return "Not attached";

            GLint isCompiled = 0;
            glGetShaderiv( stage->id(), GL_COMPILE_STATUS, &isCompiled );
            glCheck;

            GLint len = 0;
            glGetShaderiv( stage->id(), GL_INFO_LOG_LENGTH, &len );
            glCheck;

            if( len == 0 )
                return isCompiled ? "OK" : "Compilation failed";

            std::vector<GLchar> errorLog( len + 1 );
            glGetShaderInfoLog( stage->id(), len, &len, errorLog.data() );
            glCheck;

            return std::string( errorLog.begin(), errorLog.end() );
        };

        // Shader stages info
        if( shader->_vshd )  msg << "Vertex Shader: " << shaderStageLog( shader->_vshd ) << std::endl;
        if( shader->_fshd )  msg << "Fragment Shader: " << shaderStageLog( shader->_fshd ) << std::endl;
        if( shader->_gshd )  msg << "Geometry Shader: " << shaderStageLog( shader->_gshd ) << std::endl;
        if( shader->_cshd )  msg << "Compute Shader: " << shaderStageLog( shader->_cshd ) << std::endl;
        if( shader->_tshd )  msg << "Tessellation Control: " << shaderStageLog( shader->_tshd ) << std::endl;
        if( shader->_teshd ) msg << "Tessellation Evaluation: " << shaderStageLog( shader->_teshd ) << std::endl;

        // Program parameters
        GLint params = -1;
        
        glGetProgramiv( shader->id(), GL_LINK_STATUS, &params );
        msg << "GL_LINK_STATUS = " << (params ? "TRUE" : "FALSE") << std::endl;

        glGetProgramiv( shader->id(), GL_ATTACHED_SHADERS, &params );
        msg << "GL_ATTACHED_SHADERS = " << params << std::endl;

        glGetProgramiv( shader->id(), GL_ACTIVE_ATTRIBUTES, &params );
        msg << "GL_ACTIVE_ATTRIBUTES = " << params << std::endl;

        glGetProgramiv( shader->id(), GL_ACTIVE_UNIFORMS, &params );
        msg << "GL_ACTIVE_UNIFORMS = " << params << std::endl;

        // Check if separable
        glGetProgramiv( shader->id(), GL_PROGRAM_SEPARABLE, &params );
        msg << "GL_PROGRAM_SEPARABLE = " << (params ? "TRUE" : "FALSE") << std::endl;

        // Check binary length
        glGetProgramiv( shader->id(), GL_PROGRAM_BINARY_LENGTH, &params );
        msg << "GL_PROGRAM_BINARY_LENGTH = " << params << " bytes" << std::endl;
    }

    glCheck;
    return msg.str();
}
