// MouseState.cpp
//
#include "MouseState.h"

#include "Input.h"

#if _DEBUG
#include <iostream>
#include <bitset>
#endif

using namespace s2::Input;

//// ------------------------------------------------------------------------------------------------
//void MouseState::dumpStatus( const std::string &s ) const
//{
//	std::cout
//		<< s << '\n'
//		<< "   PressedButtons: 0b" << std::bitset<16>(_pressedButtons) << '\n'
//		<< "   DoubleClick:    0b" << std::bitset<16>( _doubleClickButton ) << '\n'
//		<< "   KeyModifier:   "    << _keyModifier << '\n'
//		<< "   ClickPos:     ("    << _clickPos.x << ", " << _clickPos.y << ")" << '\n'
//		<< "   CurrPos:      ("    << _currPos.x << ", "<< _currPos.y << ")" << '\n'
//		<< "   Dragging:      "    << ( _dragging ? "true" : "false" ) << '\n'
//		<< "   Wheel:         "    << _wheel << '\n'
//		<< std::endl;
//}



// ------------------------------------------------------------------------------------------------
void MouseState::onPress( /*double x, double y,*/ uint32_t button, uint32_t modifiers )
{
	_pressedButtons   |= button;
	_keyModifier       = modifiers;
	_doubleClickButton = ButtonNone;
	
	// click position, store it only if it is not set yet
	if( _clickPos.x < 0.0 && _clickPos.y < 0.0 )
		_clickPos = _currPos;
}

// ------------------------------------------------------------------------------------------------
void MouseState::onRelease( /*double x, double y,*/ uint32_t button, uint32_t modifiers )
{
	_pressedButtons &= ~button;
	_keyModifier     = modifiers;
	_clickPos        = Math::vec2( -1.f, -1.f );

	_doubleClickButton = ButtonNone; // reset double click state

	// end dragging?
	if( _dragging && _pressedButtons == ButtonNone )
		_dragging = false;

	//dumpStatus( "MouseState::onRelease: " );
}


// ------------------------------------------------------------------------------------------------
void MouseState::onMove( double x, double y )
{
	_currPos           = Math::vec2( x, y );
	_doubleClickButton = ButtonNone;

	// if we were dragging before, continue dragging
	// if we were not dragging before, check if we should start dragging
	_dragging        = _dragging ? _dragging : 
		_pressedButtons != ButtonNone &&
		Math::length( _clickPos - _currPos ) > Input::Settings::kDragThreshold_px;

	//dumpStatus( "MouseState::onMove: " );
}

// ------------------------------------------------------------------------------------------------
void MouseState::onDoubleClick( /*double x, double y,*/ uint32_t button, uint32_t modifiers )
{	
	// double click is a special case of release
	onRelease( /*x, y,*/ button, modifiers ); // release the button first
	_doubleClickButton = button;

	//dumpStatus( "MouseState::onDoubleClick: " );
}

// ------------------------------------------------------------------------------------------------
void MouseState::onWheel( double deltax, double deltay, uint32_t modifiers )
{
	_keyModifier       = modifiers;
	_wheel             = deltay; // @todo: use deltax if needed
	_doubleClickButton = ButtonNone; // reset double click state

	//_wheel = [] (double delta)
	//{
	//	static constexpr double kWheelStep { 1. };		
	//	return Math::pow( 1.25, delta / kWheelStep );
	//}( deltay );
	
	//dumpStatus( "MouseState::onWheel: " );
}