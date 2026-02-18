// Shader.h
//
#ifndef S2_RENDERCORE_SHADER_H
#define S2_RENDERCORE_SHADER_H

#include "s2Engine_API.h"

#include "OpenGLObject.h"
#include "Uniform.h"
#include "ShaderStage.h"

#include <map>
#include <string>
#include <memory>
#include <vector>

namespace s2 {
namespace RenderCore {

/************************************************************************************************/
/*                                           Shader                                             */
/************************************************************************************************/
class Shader;
typedef std::shared_ptr<Shader>   ShaderPtr;

class S2ENGINE_API Shader : public OpenGLObject
{
public:
	static ShaderPtr New();

public:
	Shader();
	~Shader();

	bool attachVertexShaderStage( const ShaderStagePtr& shader );
	bool attachFragmentShaderStage( const ShaderStagePtr& shader );
	bool attachGeometryShaderStage( const ShaderStagePtr& shader );
	bool attachComputeShaderStage( const ShaderStagePtr& shader );
	bool attachTessellationControlShaderStage( const ShaderStagePtr& shader );
	bool attachTessellationEvaluationShaderStage( const ShaderStagePtr& shader );

	bool        isLinked()    const;
	std::string name() const;

	void bind()    const override;
	void unbind()  const override;
	void applyUniforms();

	// shortcut for setting uniforms value by name.
	// it will search for uniform by name and set its value.
	// warning: if uniform is not found, it will do nothing.
	void setUniformValue( const std::string& uniformName, const UniformValue& value );

	// used to get uniform by name
	// warning: returns nullptr if not found. check before use!
	Uniform* uniform( const std::string& name );

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

	ShaderStagePtr _cshd; // compute shader
	ShaderStagePtr _tshd; // tessellation shader
	ShaderStagePtr _teshd;// tessellation evaluation shader


	bool         _linked { false };
	std::string  _name;

	std::map< std::string, unsigned int > _attributes;
	std::map< std::string, Uniform*>      _uniforms;


	friend class ShaderCompiler;
};

} // namespace RenderCore
} // namespace s2
#endif // !S2_RENDERCORE_SHADER_H