// GLContext_w32.h
//
#pragma once

#include <windows.h>

namespace s2Fwk{

class GLContext
{
private:
	HWND	hwnd;
	HDC		hDC;
	HGLRC	hRC;

	void initExtensions();
	void create();
	void destroy();

public:
	GLContext( unsigned int hWnd ); //, const std::string &version, unsigned int flags );
	virtual ~GLContext();

	void makeCurrent();
	void swapBuffers();
	void enableVSync(bool enable);
};
}
