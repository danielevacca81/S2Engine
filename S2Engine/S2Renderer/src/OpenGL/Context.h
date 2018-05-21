// Context.h
//
#ifndef CONTEXT_ONCE
#define CONTEXT_ONCE

#include "S2RendererAPI.h"

#include <vector>
#include <string>

namespace OpenGL {

class S2RENDERER_API Context
{
protected:
	void *_hwnd;
	void *_hDC;
	void *_hRC;

public:
	Context( void *winID = 0 );
	~Context();

	void create( void *winID );
	void destroy();

	bool initExtensions();
	void makeCurrent();
	void swapBuffers();
	void enableVSync(bool enable);

	std::vector<std::string> extensions() const;
	std::string              info()       const;
};
}

#endif