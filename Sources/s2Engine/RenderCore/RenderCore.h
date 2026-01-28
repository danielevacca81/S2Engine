// RenderCore.h
//
#ifndef RENDERCORE_H
#define RENDERCORE_H

#include "s2Engine_API.h"

#include "Program.h"
#include "Sampler.h"

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


}
#endif // !RENDERCORE_H

