// RenderingContext.h
//
#ifndef RENDERER_RENDERINGCONTEXT_H
#define RENDERER_RENDERINGCONTEXT_H

#include "Renderer_API.h"

#include <memory>

namespace Renderer {

class RENDERER_API RenderingContext
{
public:
	RenderingContext();
	~RenderingContext();

	void beginRendering();
	void endRendering();


private:
	class Impl;
	std::unique_ptr<Impl> _impl;
};

}


#endif // !RENDERER_RENDERINGCONTEXT_H
