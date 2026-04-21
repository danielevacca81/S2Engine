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

    // Binding (REQUIRED for rendering)
    void bind()   const;
    void unbind() const;

    // ===== DSA API for Uniforms (OpenGL 4.1+) =====
    
    // Set uniform and apply immediately (DSA - no binding required)
	// Note: This method performs a lookup by uniform name, which can be inefficient if called frequently.
    // For performance-critical code, consider caching uniform handles and using the setUniform(UniformHandle, UniformValue) overload.
    void setUniform( const std::string& uniformName, const UniformValue& value );

	// directly set uniform by handle
	void setUniform( UniformHandle handle, const UniformValue& value );
    
    // Get uniform by name (read-only access)
    const Uniform* uniform( const std::string& name ) const;
	//Uniform*       uniform( const std::string& name );
    
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

	// Uniforms are stored in a vector indexed by their location for O(1) access during rendering.
	// The location is determined at link time and is used as the index in the vector.
	// This allows for very fast uniform updates without the overhead of name lookups or bindings.
    // 
	// Note: Uniform can also be accessed by name via the uniform() method, which performs a lookup in the vector based on the uniform's location.
	// Todo: use unique_ptr (ownership of uniforms is tied to the shader, they are created at link time and destroyed with the shader)
	std::vector<Uniform*> _uniforms; // List of active uniforms

    //std::map<std::string, Uniform*>     _uniforms;
    
    // Bindless texture tracking
    //std::vector<Texture2DPtr> _residentTextures;

    friend class ShaderCompiler;
};

} // namespace RenderCore
} // namespace s2
#endif // !S2_RENDERCORE_SHADER_H