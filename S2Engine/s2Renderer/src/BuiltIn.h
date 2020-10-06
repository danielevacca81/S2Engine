// BuiltIn.h
//
#ifndef BUILTIN_ONCE
#define BUILTIN_ONCE

#include "s2Renderer_API.h"

#include "Program.h"
#include "Sampler.h"

namespace Renderer {

class S2RENDERER_API BuiltIn
{
public:
	static bool init();
	static void destroy();

private:
	static bool initSamplers();
	static bool initShaders();

public:
	static ProgramPtr shaderFullscreenQuad;
	static ProgramPtr shaderSimple;

	static SamplerPtr samplerNearestClamp;
	static SamplerPtr samplerLinearClamp;
	static SamplerPtr samplerNearestRepeat;
	static SamplerPtr samplerLinearRepeat;

	static void enableDebugOutput( /*severity, source...*/);
	static void disableDebugOutput();
	static bool isDebugOutputEnabled();


private:
	static bool _initialized; //todo: once per context
	static bool _debugEnabled;

};



}
#endif // !BUILTIN_ONCE

