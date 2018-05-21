// GLWindow_w32.h
//
#pragma once

#include <windows.h>
#include <string>


namespace s2Fwk{
class GLContext;


class GLWindow
{
private:
	GLContext	 *_context;

	HWND		 _hwnd;
	int          _width;
	int          _height;
	std::string  _windowTitle;
	bool         _windowed;


	void	createWindow();
	void	createContext();
	void	destroy();
	void	validateResolution();

	static  LRESULT CALLBACK WndProc(HWND, UINT,WPARAM,LPARAM);

public:
	GLWindow( const std::string &title = std::string("") /*@todo: Window mode*/ );
	virtual ~GLWindow();

	bool processWinMessages();
	void show( bool visible = true );

	void update();
	void resize( int w, int h );
	void setWindowed( bool windowed );

	// Gets
	int          width() const;
	int          height() const;
	unsigned int hwnd() const;
};

}

