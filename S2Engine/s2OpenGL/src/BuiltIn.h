// BuiltIn.h
//
#ifndef BUILTIN_ONCE
#define BUILTIN_ONCE

#include "s2OpenGL_API.h"

#include "Program.h"
#include "Sampler.h"

namespace s2 {
namespace OpenGL {

class S2OPENGL_API BuiltIn
{
public:
	static bool init();

private:
	static bool initSamplers();
	static bool initShaders();

public:
	static Program shaderFullscreenQuad;
	static Program shaderSimple;

	static Sampler samplerNearestClamp;
	static Sampler samplerLinearClamp;
	static Sampler samplerNearestRepeat;
	static Sampler samplerLinearRepeat;


private:
	static bool _initialized; //todo: once per context

};



}
}
#endif // !BUILTIN_ONCE

