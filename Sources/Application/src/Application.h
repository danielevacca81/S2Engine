// Application.h
//
#ifndef APPLICATION_APPLICATION_H
#define APPLICATION_APPLICATION_H

#include "Application_API.h"

#include <string>


namespace s2 {

// @todo: multiple window application
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