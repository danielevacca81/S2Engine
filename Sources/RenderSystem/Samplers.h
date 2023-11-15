// Samplers.h
//
#ifndef SAMPLERS_ONCE
#define SAMPLERS_ONCE

#include "RenderSystem_API.h"

#include "Sampler.h"

namespace RenderSystem {

class RENDERSYSTEM_API Samplers
{
public:
	Samplers() = delete;

	static bool init();
	static void destroy();

public:
	static SamplerPtr NearestClamp;
	static SamplerPtr LinearClamp;
	static SamplerPtr NearestRepeat;
	static SamplerPtr LinearRepeat;

private:
	static bool _initialized; //todo: once per context
};



}
#endif // !SAMPLERS_ONCE

