// RenderCore.h
//
#ifndef RENDERCORE_H
#define RENDERCORE_H

#include "RenderCore_API.h"

#include "Program.h"
#include "Sampler.h"

namespace RenderCore {

bool RENDERCORE_API init();
void RENDERCORE_API destroy();

struct Shaders
{
	ProgramPtr FullscreenQuad;
	ProgramPtr Simple;
	ProgramPtr Phong;
	ProgramPtr BlinnPhong;
	//ProgramPtr Debug;
	// ...
};
extern RENDERCORE_API Shaders  DefaultShaders;


struct Samplers
{
	SamplerPtr NearestClamp;
	SamplerPtr LinearClamp;
	SamplerPtr NearestRepeat;
	SamplerPtr LinearRepeat;
};
extern RENDERCORE_API Samplers DefaultSamplers;


}
#endif // !RENDERCORE_H

