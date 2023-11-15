// Shaders.h
//
#ifndef SHADERS_ONCE
#define SHADERS_ONCE

#include "RenderSystem_API.h"

#include "Program.h"

namespace RenderSystem {

class RENDERSYSTEM_API Shaders
{
public:
	Shaders() = delete;

	static bool init();
	static void destroy();

public:
	static ProgramPtr FullscreenQuad;
	static ProgramPtr Simple;

private:
	static bool _initialized; //todo: once per context
};



}
#endif // !BUILTIN_ONCE

