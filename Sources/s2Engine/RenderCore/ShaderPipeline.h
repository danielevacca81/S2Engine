// ShaderPipeline.h
//
#ifndef RENDERCORE_SHADERPIPELINE_H
#define RENDERCORE_SHADERPIPELINE_H

#include "s2Engine_API.h"

#if 0
#include "OpenGLObject.h"
#include "Shader.h"

namespace s2{
namespace RenderCore {

class S2ENGINE_API ShaderPipeline : public OpenGLObject
{
public:
    void useStages(uint32_t stageBits, const ShaderPtr& shader);
    void bind() const override;
    void unbind() const override;
    
private:
    std::map<ShaderStageType, ShaderPtr> _stages;
};

}
}
#endif

#endif // !RENDERCORE_SHADERPIPELINE_H