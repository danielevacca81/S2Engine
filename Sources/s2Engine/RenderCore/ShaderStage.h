// ShaderStage.h
//
#ifndef S2_RENDERCORE_SHADERSTAGE_H
#define S2_RENDERCORE_SHADERSTAGE_H

#include "s2Engine_API.h"

#include "OpenGLObject.h"

#include <string>
#include <memory>

namespace s2 {
namespace RenderCore {

enum class ShaderStageType
{
    Vertex,
    Fragment,
    Geometry,
    Compute,
    TessellationControl,
    TessellationEvaluation
};


class ShaderStage;
typedef std::shared_ptr<ShaderStage> ShaderStagePtr;

// ------------------------------------------------------------------------------------------------
class S2ENGINE_API ShaderStage : public OpenGLObject
{
public:
	static ShaderStagePtr New( const ShaderStageType& type );

public:
    ShaderStage( const ShaderStageType& type );
	~ShaderStage();

	ShaderStageType type() const { return _type; }

private:
    void create()  override;
    void destroy() override;

	int objectLabelIdentifier() const override;

private:
	ShaderStageType _type;


	friend class ShaderCompiler;
};

} // namespace RenderCore
} // namespace s2
#endif // !S2_RENDERCORE_SHADER_H