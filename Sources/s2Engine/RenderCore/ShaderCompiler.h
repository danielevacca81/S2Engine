// ShaderCompiler.h
//
#ifndef S2_RENDERCORE_SHADERCOMPILER_H
#define S2_RENDERCORE_SHADERCOMPILER_H

#include "s2Engine_API.h"

#include "ShaderStage.h"
#include "Shader.h"

#include <string>
#include <vector>
#include <filesystem>

namespace s2 {
namespace RenderCore {

// ------------------------------------------------------------------------------------------------
struct ShaderStageCompilationResult
{
    bool           success;
    std::string    errorLog;
    ShaderStagePtr stage;  // nullptr if failed

    explicit operator bool() const { return success; }
};

// ------------------------------------------------------------------------------------------------
struct ShaderLinkResult
{
    bool        success;
    std::string errorLog;

    explicit operator bool() const { return success; }
};

// ------------------------------------------------------------------------------------------------
class S2ENGINE_API ShaderCompiler
{
public:
    /// Compile a shader from source code
    /// Returns compilation result with error log if failed
    static ShaderStageCompilationResult compile( ShaderStageType type, const std::string& source );

    /// Compile shader from file
    static ShaderStageCompilationResult compileFromFile( ShaderStageType type, const std::filesystem::path &filepath );

    /// Get shader info log
    static std::string getShaderStageInfoLog( const ShaderStagePtr &stage );

    /// Link the shader with all attached shader stages
    static ShaderLinkResult linkShader( const ShaderPtr& shader, const std::string& name = "" );

    /// Validate the shader (check if it can execute given current OpenGL state)
    static ShaderLinkResult validateShader( const ShaderPtr& shader );

    /// Get detailed info about a program (attributes, uniforms, etc.)
    static std::string getShaderInfo( const ShaderPtr &shader, bool verbose = false );

    /// Get shader info log
    static std::string getShaderInfoLog( const ShaderPtr &shader );
};

} // namespace RenderCore
} // namespace s2
#endif // !S2_RENDERCORE_SHADERCOMPILER_H