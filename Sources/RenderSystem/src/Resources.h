// Resources.h
//
#ifndef RESOURCES_ONCE
#define RESOURCES_ONCE

#include "RenderSystem_API.h"

#include "Program.h"
#include "Sampler.h"

namespace RenderSystem {

class RENDERSYSTEM_API Resources
{
public:
	Resources() = delete;

	static bool init();
	static void destroy();

public:
	struct Shaders
	{
		ProgramPtr FullscreenQuad;
		ProgramPtr Simple;
	};

	struct Samplers
	{
		SamplerPtr NearestClamp;
		SamplerPtr LinearClamp;
		SamplerPtr NearestRepeat;
		SamplerPtr LinearRepeat;
	};

	static Samplers DefaultSamplers;
	static Shaders  DefaultShaders;


private:
	static bool initShaders();
	static bool initSamplers();

private:
	static bool _initialized; //todo: once per context
};



}
#endif // !RESOURCES_ONCE

