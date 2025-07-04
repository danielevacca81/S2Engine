// Resources.h
//
#ifndef RESOURCES_H
#define RESOURCES_H

#include "RenderCore_API.h"

#include "Program.h"
#include "Sampler.h"

namespace RenderCore {
// rename Default
class RENDERCORE_API Resources
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
		//ProgramPtr Debug;
		//ProgramPtr Phong
		// ...
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
	//static PrimitiveBufferPtr FullscreenQuad; //@todo
	//static Texture2D WhiteTexture; //@todo


private:
	static bool initShaders();
	static bool initSamplers();

private:
	static bool _initialized; //todo: once per context
};



}
#endif // !RESOURCES_H

