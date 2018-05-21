// InputManager.cpp
//
#include "InputManager.h"

#include "InputEventHandler.h"

#include <OIS.h>
#include <iostream>
#include <sstream>
#include <assert.h>

#include <string>

using namespace s2Fwk;

// ------------------------------------------------------------------------------------------------
InputManager::InputManager()
: _mouse(0)
, _kb(0)
, _im(0)
{
	_handler = new InputEventHandler( &_keyboardStatus,&_mouseStatus );
}

// ------------------------------------------------------------------------------------------------
InputManager::~InputManager()
{
	destroy();
}

// ------------------------------------------------------------------------------------------------
bool InputManager::init(unsigned int hwnd)
{
	std::ostringstream wndstr;
	wndstr << (size_t)hwnd;

	OIS::ParamList pl;
	pl.insert( std::make_pair("WINDOW", wndstr.str())); // first parameter is WINDOW
	pl.insert( std::make_pair( std::string( "w32_mouse" ),    "DISCL_FOREGROUND"   ) );
	pl.insert( std::make_pair( std::string( "w32_mouse" ),    "DISCL_NONEXCLUSIVE" ) );
	pl.insert( std::make_pair( std::string( "w32_keyboard" ), "DISCL_FOREGROUND"   ) );
	pl.insert( std::make_pair( std::string( "w32_keyboard" ), "DISCL_NONEXCLUSIVE" ) );
	pl.insert( std::make_pair( std::string( "w32_keyboard" ), "DISCL_NOWINKEY"     ) );

	_im = OIS::InputManager::createInputSystem(pl);
	if(!_im)
	{
		//std::cout << "[InputManager]" << std::endl
		//	      << "   /!\ Error: createInputSystem failed!" << std::endl;
		return false;
	}

	_im->enableAddOnFactory(OIS::InputManager::AddOn_All);


	//Creat buffered keyboard
	_kb = static_cast<OIS::Keyboard*> (_im->createInputObject( OIS::OISKeyboard, true ));
	_kb->setEventCallback(_handler);
	//memset(keyBuffer,false, sizeof(keyBuffer));

	//Creat buffered mouse
	_mouse = static_cast<OIS::Mouse*> (_im->createInputObject( OIS::OISMouse, true ));
	_mouse->setEventCallback(_handler);

	{
		const unsigned int v = _im->getVersionNumber();

		std::cout << "[InputManager]" << std::endl 
				  << "   OIS Version:     " << (v>>16 ) << "." << ((v>>8) & 0x000000FF) << "." << (v & 0x000000FF) << std::endl
				  << "   Release Name:    " << _im->getVersionName() << std::endl
				  << "   Manager:         " << _im->inputSystemName() << std::endl
				  << "   Total Keyboards: " << _im->getNumberOfDevices(OIS::OISKeyboard) << std::endl
				  << "   Total Mouses:    " << _im->getNumberOfDevices(OIS::OISMouse) << std::endl
				  << "   Total JoySticks: " << _im->getNumberOfDevices(OIS::OISJoyStick) << std::endl << std::endl;
	}
	return true;
}

// ------------------------------------------------------------------------------------------------
void InputManager::destroy()
{
	delete _handler;

	if(!_im)
		return;

	if(_kb)		_im->destroyInputObject(_kb);
	if(_mouse)	_im->destroyInputObject(_mouse);

	OIS::InputManager::destroyInputSystem(_im);
	_im    = 0;
	_kb    = 0;
	_mouse = 0;
}

// ------------------------------------------------------------------------------------------------
void InputManager::update()
{
	_kb->capture();
	_mouse->capture();
}

// ------------------------------------------------------------------------------------------------
void InputManager::resize( int w, int h )
{
	const OIS::MouseState &ms = _mouse->getMouseState();
	ms.width  = w;
	ms.height = h;
}


//using namespace s2Fwk;
//
//inline S2CORE_DLL_API bool s2Input::isKeyHit(const OIS::KeyCode key)     {return Application::theApp()->isKeyHit(key);}
//inline S2CORE_DLL_API bool s2Input::isKeyPressed(const OIS::KeyCode key) {return Application::theApp()->isKeyPressed(key);}
