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
#include <optional>

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
struct SeparableShaderResult
{
    bool           success;
    std::string    errorLog;
    ShaderPtr      shader;  // nullptr if failed

    explicit operator bool() const { return success; }
};

// ------------------------------------------------------------------------------------------------
class S2ENGINE_API ShaderCompiler
{
public:
    // ===== Shader Stage Compilation =====
    
    /// Compile a shader stage from source code
    static ShaderStageCompilationResult compile( 
        ShaderStageType type, 
        const std::string& source );

    /// Compile shader stage from file
    static ShaderStageCompilationResult compileFromFile( 
        ShaderStageType type, 
        const std::filesystem::path& filepath );

    /// Get shader stage info log
    static std::string getShaderStageInfoLog( const ShaderStagePtr& stage );

    // ===== Traditional Shader Linking =====
    
    /// Link the shader with all attached shader stages
    static ShaderLinkResult linkShader( 
        const ShaderPtr& shader, 
        const std::string& name = "" );

    /// Validate the shader (check if it can execute given current OpenGL state)
    static ShaderLinkResult validateShader( const ShaderPtr& shader );

    // ===== Separable Shaders (OpenGL 4.1+) =====
    
    /// Create and compile a separable shader program from single stage
    /// Useful for shader program pipelines
    static SeparableShaderResult createSeparableShader(
        ShaderStageType type,
        const std::string& source,
        const std::string& name = "" );

    /// Create separable shader from file
    static SeparableShaderResult createSeparableShaderFromFile(
        ShaderStageType type,
        const std::filesystem::path& filepath,
        const std::string& name = "" );

    /// Mark existing shader as separable (must be done before linking)
    static void setSeparable( const ShaderPtr& shader, bool separable = true );

    // ===== Shader Introspection =====
    
    /// Get detailed info about a program (attributes, uniforms, etc.)
    static std::string getShaderInfo( const ShaderPtr& shader, bool verbose = false );

    /// Get shader program info log
    static std::string getShaderInfoLog( const ShaderPtr& shader );

    /// Get binary shader representation (OpenGL 4.1+)
    static std::optional<std::vector<uint8_t>> getBinaryShader( const ShaderPtr& shader );

    /// Load shader from binary (OpenGL 4.1+)
    static ShaderLinkResult loadFromBinary( 
        const ShaderPtr& shader, 
        const std::vector<uint8_t>& binary,
        uint32_t binaryFormat );
};

} // namespace RenderCore
} // namespace s2
#endif // !S2_RENDERCORE_SHADERCOMPILER_H