// InputState.cpp
//
#include "InputState.h"

#include <chrono>

using namespace s2::Input;

// helper to visit multiple types in a variant
template<class... T>
struct MouseEventVisitorHelper : T... { using T::operator()...; };


// ------------------------------------------------------------------------------------------------
void InputState::updateMouseState( const MouseEvent &event )
{
	auto onMouseMove = [this]( const MouseMoveEvent &e )
	{
		_mouseState.onMove( e.x, e.y );
	};

	auto onMouseButton = [this]( const MouseButtonEvent &e )
	{
		if( e.eventType == MouseButtonEvent::Press )
			_mouseState.onPress(  e.button, e.modifiers );
		else if( e.eventType == MouseButtonEvent::Release )
		{
			static auto before = std::chrono::system_clock::now();
			const auto now     = std::chrono::system_clock::now();
			const auto diff    = std::chrono::duration_cast<std::chrono::milliseconds>( now - before );
			static uint32_t lastButtonClicked = 0; // last pressed button

			// check for double click
			if( e.button == lastButtonClicked &&
				diff < std::chrono::milliseconds( Input::Settings::kDoubleClickTime_ms ) )
			{
				// if the same button was pressed within the double click time, register it as a double click
				_mouseState.onDoubleClick( e.button, e.modifiers ); // handle double click
				lastButtonClicked = 0; // reset last button clicked
			}
			else
			{
				_mouseState.onRelease( e.button, e.modifiers );
				lastButtonClicked = e.button; // update last button pressed
			}		
			before = now; // reset the timer
		}
	};

	auto onMouseWheel = [this]( const MouseWheelEvent &e )
	{
		_mouseState.onWheel( e.deltax, e.deltay, 0/*, e.modifiers*/ );
	};




	// Update the mouse state based on the event type
	// use visitor pattern to handle different mouse events instead of using if-else chains
	std::visit( MouseEventVisitorHelper 
				{
					[&] ( const MouseMoveEvent   &e ) { onMouseMove( e ); },
					[&] ( const MouseButtonEvent &e ) { onMouseButton( e ); },
					[&] ( const MouseWheelEvent  &e ) { onMouseWheel( e ); },
				}, event );
}