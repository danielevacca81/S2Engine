// InputEventHandler.cpp
//
#include "InputEventHandler.h"

#include "MouseStatus.h"
#include "KeyboardStatus.h"

#include "application/Application.h"

#include <iostream>

using namespace s2Fwk;


// ------------------------------------------------------------------------------------------------
bool InputEventHandler::keyPressed( const OIS::KeyEvent &arg )
{ 
	std::cout << " KeyPressed {" << arg.key
			<< ", " << ((OIS::Keyboard*)(arg.device))->getAsString(arg.key)
			<< "} || Character (" << (char)arg.text << ")" << std::endl;

	Application::s2App()->onKeyPressed();

	return true;
}

// ------------------------------------------------------------------------------------------------
bool InputEventHandler::keyReleased( const OIS::KeyEvent &arg )
{
	std::cout << "KeyReleased {" << ((OIS::Keyboard*)(arg.device))->getAsString(arg.key) << "}" << std::endl;

	Application::s2App()->onKeyReleased();

	return true;
}

// ------------------------------------------------------------------------------------------------
bool InputEventHandler::mouseMoved( const OIS::MouseEvent &event )
{ 
	_mouseStatus->update(event);

	Application::s2App()->onMouseMoved();	
	return true;
}

// ------------------------------------------------------------------------------------------------
bool InputEventHandler::mousePressed( const OIS::MouseEvent &event, OIS::MouseButtonID id )
{ 
	_mouseStatus->update(event, id);

	Application::s2App()->onMousePressed();
	return true;
}

// ------------------------------------------------------------------------------------------------
bool InputEventHandler::mouseReleased( const OIS::MouseEvent &event, OIS::MouseButtonID id )
{ 
	_mouseStatus->update(event, id);

	Application::s2App()->onMouseReleased();
	return true;
}