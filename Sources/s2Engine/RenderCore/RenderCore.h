// RenderCore.h
//
#ifndef S2_RENDERCORE_RENDERCORE_H
#define S2_RENDERCORE_RENDERCORE_H

#include "s2Engine_API.h"

#include "Program.h"
#include "Sampler.h"

namespace s2 {
namespace RenderCore {

bool S2ENGINE_API init();
void S2ENGINE_API destroy();

struct Shaders
{
	ProgramPtr FullscreenQuad;
	ProgramPtr Simple;
	ProgramPtr Phong;
	ProgramPtr BlinnPhong;
	//ProgramPtr Debug;
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
