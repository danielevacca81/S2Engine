// RenderMaterialPropertyApplier.h
//
#ifndef S2_RENDERER_MATERIALPROPERTYAPPLIER_H
#define S2_RENDERER_MATERIALPROPERTYAPPLIER_H

#include "RenderMaterial.h"
#include "RenderCore/Uniform.h"

#include <optional>

namespace s2 {
namespace Renderer {

class RenderMaterialPropertyApplier
{
public:
	// converts a RenderMaterial::Property to a RenderCore::UniformValue if possible, based on the target uniform type
    static std::optional<RenderCore::UniformValue> convert(const RenderMaterial::Property& property,const RenderCore::UniformValue& targetUniformType );

    // Applies all properties of a material to the uniforms of a shader
    static void applyMaterialToShader( const RenderMaterial& material,RenderCore::Shader* shader );
};

} // namespace Renderer
} // namespace s2
#endif // !S2_RENDERER_MATERIALPROPERTYAPPLIER_H