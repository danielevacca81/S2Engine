// Application.cpp
//
#include "Application.h"

#include "system/GLWindow.h"
#include "system/GLContext.h"
#include "input/InputManager.h"

#include <iostream>
#include <sstream>

using namespace s2Fwk;

Application *Application::_ptr = 0;

// ------------------------------------------------------------------------------------------------
Application::Application( const std::string &name )
{
	_ptr     = this;
	_window  = new GLWindow( name );
	_input   = new InputManager;

	_input->init( _window->hwnd() );
}

// ------------------------------------------------------------------------------------------------
Application::~Application()
{
	destroy();
}

// ------------------------------------------------------------------------------------------------
void Application::destroy()
{
	delete _window;
	delete _input;
}

// ------------------------------------------------------------------------------------------------
void Application::paint()
{}

// ------------------------------------------------------------------------------------------------
void Application::update()
{}

// ------------------------------------------------------------------------------------------------
void Application::updateInput()
{
	_input->update();
}

// ------------------------------------------------------------------------------------------------
void Application::resizeWindow(int w, int h)
{
	_window->resize(w,h);
	_input->resize(w,h);
}

// ------------------------------------------------------------------------------------------------
void Application::mainLoop()
{
	defaultMainLoop();
}

// ------------------------------------------------------------------------------------------------
void Application::defaultMainLoop()
{
	// @todo:
	// mainthread: read user input
	// thread 1) update application logic at fixed framerate (parameter)
	// thread 2) update animation/physics according to elapsed time (t)
	// render thread runs at maximum speed
	
	updateInput();
	update();
	paint();
}

// ------------------------------------------------------------------------------------------------
void Application::run( )
{
	_window->show();

	while( _window->processWinMessages() )
	{
		mainLoop();

		_window->update();
	}
}


// ------------------------------------------------------------------------------------------------
bool Application::init()
{
	return true;
}

// ------------------------------------------------------------------------------------------------
void Application::onMousePressed()  {}
void Application::onMouseReleased() {}
void Application::onMouseMoved()    {}

// ------------------------------------------------------------------------------------------------
void Application::onKeyPressed()    {}
void Application::onKeyReleased()   {}



//void      Application::setwindow(GLWindow *w)	{window = w;}
//GLWindow* Application::getwindow()            {return Application::theApp()->window;}
//
//inline bool Application::isKeyHit(const OIS::KeyCode key)
//{
//	if(kb->isKeyDown(key) && !keyBuffer[key])
//	{
//		keyBuffer[key] = true;
//		return true;
//	}
//
//	if(!kb->isKeyDown(key))
//		keyBuffer[key] = false;
//
//	return false;
//}
//
//inline bool Application::isKeyPressed(const OIS::KeyCode key)
//{
//	return kb->isKeyDown(key);
//}
