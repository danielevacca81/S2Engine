// Application.h
//
#ifndef APPLICATION_APPLICATION_H
#define APPLICATION_APPLICATION_H

#include "Application_API.h"

#include <string>


/**
*  WIP: spostare opengl_extensions in Application.dll
*  * class Window detiene una lista di Renderer::Renderer
*    * each Renderer::Renderer has 
*        .StateManager
*        .Surface
*        .Context
*    * Window::addRenderer()
* 
*  * class Application detiene contextRegistry?
*  
*  Contexts manager?
* 
*  * class Surface unaware of context
* 
* 


*/

namespace s2 {

class Window;

class APPLICATION_API Application
{
public:
	Application( const std::string &name );
	virtual ~Application();
	
	Application( const Application& a )    = delete;
	Application( Application&& a )         = delete;
	void operator=( const Application& a ) = delete;
	void operator=( Application&& a )      = delete;

	int run( Window *w );

private:
	void* _instance = nullptr;
};

}

#endif