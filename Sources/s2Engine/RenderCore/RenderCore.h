// RenderCore.h
//
#ifndef S2_RENDERCORE_RENDERCORE_H
#define S2_RENDERCORE_RENDERCORE_H

#include "s2Engine_API.h"

#include "Shader.h"
#include "Sampler.h"

namespace s2 {
namespace RenderCore {

bool S2ENGINE_API init();
void S2ENGINE_API destroy();

struct Shaders
{
	ShaderPtr FullscreenQuad;
	ShaderPtr Simple;
	ShaderPtr Phong;
	ShaderPtr BlinnPhong;
	//ShaderPtr Wireframe;
	//ShaderPtr Picking;
	//ShaderPtr Debug;
	// ...
};
extern S2ENGINE_API Shaders  DefaultShaders;


struct Samplers
{
	SamplerPtr NearestClamp;
	SamplerPtr LinearClamp;
	SamplerPtr NearestRepeat;
	SamplerPtr LinearRepeat;
};
extern S2ENGINE_API Samplers DefaultSamplers;


} // namespace RenderCore
} // namespace s2
#endif // !S2_RENDERCORE_RENDERCORE_H
