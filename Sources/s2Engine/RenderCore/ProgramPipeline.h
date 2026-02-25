// ProgramPipeline.h
//
#ifndef RENDERCORE_PROGRAMPIPELINE_H
#define RENDERCORE_PROGRAMPIPELINE_H

#include "s2Engine_API.h"

#include "OpenGLObject.h"
#include "Shader.h"

namespace s2{
namespace RenderCore {

class S2ENGINE_API ProgramPipeline : public OpenGLObject
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
#endif // !RENDERCORE_PROGRAMPIPELINE_H