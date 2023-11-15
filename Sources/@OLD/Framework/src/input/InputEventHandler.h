// InputEventHandler.h
//
#ifndef INPUTEVENTHANDLER_ONCE
#define INPUTEVENTHANDLER_ONCE

#include <OISKeyboard.h>
#include <OISMouse.h>
#include <OISEvents.h>
//#include <OISJoyStick.h>


namespace s2Fwk {

class MouseStatus;
class KeyboardStatus;

class InputEventHandler : public OIS::KeyListener, public OIS::MouseListener//, public OIS::JoyStickListener
{
private:
	MouseStatus    *_mouseStatus;
	KeyboardStatus *_keyboardStatus;

public:
	InputEventHandler( KeyboardStatus *ks, MouseStatus *ms) 
	: _keyboardStatus(ks)
	, _mouseStatus(ms)
	{}
	
	~InputEventHandler()
	{}

	bool keyPressed( const OIS::KeyEvent &arg );
	bool keyReleased( const OIS::KeyEvent &arg );

	bool mouseMoved( const OIS::MouseEvent &arg );
	bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	
	/*bool buttonPressed( const JoyStickEvent &arg, int button ) {
		std::cout << std::endl << arg.device->vendor() << ". Button Pressed # " << button;
		return true;
	}
	bool buttonReleased( const JoyStickEvent &arg, int button ) {
		std::cout << std::endl << arg.device->vendor() << ". Button Released # " << button;
		return true;
	}
	bool axisMoved( const JoyStickEvent &arg, int axis )
	{
		//Provide a little dead zone
		if( arg.state.mAxes[axis].abs > 2500 || arg.state.mAxes[axis].abs < -2500 )
			std::cout << std::endl << arg.device->vendor() << ". Axis # " << axis << " Value: " << arg.state.mAxes[axis].abs;
		return true;
	}
	bool povMoved( const JoyStickEvent &arg, int pov )
	{
		std::cout << std::endl << arg.device->vendor() << ". POV" << pov << " ";

		if( arg.state.mPOV[pov].direction & Pov::North ) //Going up
			std::cout << "North";
		else if( arg.state.mPOV[pov].direction & Pov::South ) //Going down
			std::cout << "South";

		if( arg.state.mPOV[pov].direction & Pov::East ) //Going right
			std::cout << "East";
		else if( arg.state.mPOV[pov].direction & Pov::West ) //Going left
			std::cout << "West";

		if( arg.state.mPOV[pov].direction == Pov::Centered ) //stopped/centered out
			std::cout << "Centered";
		return true;
	}

	bool vector3Moved( const JoyStickEvent &arg, int index)
	{
		std::cout.precision(2);
		std::cout.flags(std::ios::fixed | std::ios::right);
		std::cout << std::endl << arg.device->vendor() << ". Orientation # " << index 
			<< " X Value: " << arg.state.mVectors[index].x
			<< " Y Value: " << arg.state.mVectors[index].y
			<< " Z Value: " << arg.state.mVectors[index].z;
		std::cout.precision();
		std::cout.flags();
		return true;
	}
	*/
};


}

#endif