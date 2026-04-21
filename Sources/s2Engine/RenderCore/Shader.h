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
#include <vector>
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

    // Binding (REQUIRED for rendering - cannot be avoided!)
    void bind()   const;
    void unbind() const;

    // ===== DSA API for Uniforms (OpenGL 4.1+) =====
    
    // Set uniform and apply immediately (DSA - no binding required)
    void setUniform( const std::string& uniformName, const UniformValue& value );
    
    // Apply all changed uniforms (DSA - no binding required)
    void applyUniforms();

    // ===== Bindless Texture Support (OpenGL 4.5+) =====
    
    // Set bindless texture handle directly
    void setTextureHandle( const std::string& uniformName, uint64_t handle );
    
    // Convenience: Set texture (makes resident automatically)
    void setTexture( const std::string& uniformName, const Texture2DPtr& texture );

    // ===== Query =====
    
    // Get uniform by name (read-only access)
    const Uniform* uniform( const std::string& name ) const;

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

    std::map<std::string, unsigned int> _attributes;
    std::map<std::string, Uniform*>     _uniforms;
    
    // Bindless texture tracking
    std::vector<Texture2DPtr> _residentTextures;

    friend class ShaderCompiler;
};

} // namespace RenderCore
} // namespace s2
#endif // !S2_RENDERCORE_SHADER_H