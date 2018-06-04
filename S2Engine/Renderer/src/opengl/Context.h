// Context.h
//
#ifndef CONTEXT_ONCE
#define CONTEXT_ONCE

#include "Renderer_API.h"

#include <vector>
#include <string>

namespace OpenGL {

class RENDERER_API Context
{
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

protected:
	void *_hwnd;
	void *_hDC;
	void *_hRC;
};
}

#endif