// Shader.h
//
#ifndef S2_RENDERCORE_SHADER_H
#define S2_RENDERCORE_SHADER_H

#include "s2Engine_API.h"

#include "OpenGLObject.h"
#include "Uniform.h"
#include "ShaderStage.h"
#include "Texture.h"

#include <map>
#include <string>
#include <memory>
#include <unordered_map>
#include <cstdint>

namespace s2 {
namespace RenderCore {

/************************************************************************************************/
/*                                           Shader                                             */
/************************************************************************************************/
class Shader;
typedef std::shared_ptr<Shader> ShaderPtr;

class S2ENGINE_API Shader : public OpenGLObject
{
public:
    static ShaderPtr New();

public:
    Shader();
    ~Shader();

    // Shader stage attachment
    bool attachVertexShaderStage( const ShaderStagePtr& shader );
    bool attachFragmentShaderStage( const ShaderStagePtr& shader );
    bool attachGeometryShaderStage( const ShaderStagePtr& shader );
    bool attachComputeShaderStage( const ShaderStagePtr& shader );
    bool attachTessellationControlShaderStage( const ShaderStagePtr& shader );
    bool attachTessellationEvaluationShaderStage( const ShaderStagePtr& shader );

    // State queries
    bool        isLinked() const;
    std::string name() const;

    // Binding (REQUIRED for rendering)
    void bind()   const;
    void unbind() const;

    // ===== DSA API for Uniforms (OpenGL 4.1+) =====
    
    // Get uniform handle by name (O(1) lookup via cache)
    // Returns Invalid handle if uniform does not exist.
    // Cache the result for subsequent fast access via setUniform(UniformHandle, UniformValue).
    UniformHandle getUniformHandle( const std::string& uniformName ) const;
    
    // Set uniform and apply immediately (DSA - no binding required)
	// Note: This method performs a lookup by uniform name (O(n) iteration).
    // For performance-critical code, use getUniformHandle() once and cache the result,
    // then use setUniform(UniformHandle, UniformValue) for fast O(1) access.
    void setUniform( const std::string& uniformName, const UniformValue& value );

	// Set uniform by handle (O(1) fast path - no lookup needed)
	void setUniform( UniformHandle handle, const UniformValue& value );
    
    // Get uniform by name (read-only access)
    // Deprecated: Use getUniformHandle() for better performance.
    const Uniform* uniform( const std::string& name ) const;
    
    // Apply all changed uniforms (DSA - no binding required)
    void applyUniforms();

 //   // ===== Bindless Texture Support (OpenGL 4.5+) =====
 //   
	//// Set bindless texture handle directly (texture must be made resident before this call)
 //   void setTextureHandle( const std::string& uniformName, uint64_t handle );
 //   
 //   // Convenience: Set texture (makes resident automatically)
 //   void setTexture( const std::string& uniformName, const Texture2DPtr& texture );

    // ===== Query =====
    

private:
    void create()  override;
    void destroy() override;
    int  objectLabelIdentifier() const override;
    void reset() override;

    void findUniforms();

private:
    ShaderStagePtr _vshd;
    ShaderStagePtr _fshd;
    ShaderStagePtr _gshd;
    ShaderStagePtr _cshd;   // compute shader
    ShaderStagePtr _tshd;   // tessellation control
    ShaderStagePtr _teshd;  // tessellation evaluation

    bool         _linked { false };
    std::string  _name;

	// Uniforms are stored in an unordered_map keyed by their location for O(1) access during rendering.
	// The location is determined at link time by OpenGL and serves as the unique key.
	// Only valid uniforms are stored (built-in and array uniforms are skipped).
    // 
	// This avoids the nullptr-holes problem of vector-based storage while maintaining fast lookups.
	// Ownership: Uniforms are created at link time and destroyed when the shader is destroyed.
	std::unordered_map<int, Uniform*> _uniforms; // key = uniformLocation, value = Uniform*
    
    // Cache for O(1) uniform name lookups: uniformName -> uniformLocation
    // Populated during findUniforms() to enable fast getUniformHandle(name) queries.
	std::unordered_map<std::string, int> _uniformsByName; // key = uniformName, value = uniformLocation

    friend class ShaderCompiler;
};

} // namespace RenderCore
} // namespace s2
#endif // !S2_RENDERCORE_SHADER_H