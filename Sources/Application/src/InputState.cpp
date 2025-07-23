// InputState.cpp
//
#include "InputState.h"

#include <chrono>

using namespace s2;

static constexpr auto kDoubleClickTime = std::chrono::milliseconds( 200 ); // 200 ms for double click

// helper to visit multiple types in a variant
template<class... T>
struct MouseEventVisitorHelper : T... { using T::operator()...; };


// ------------------------------------------------------------------------------------------------
void InputState::updateMouseState( const Input::MouseEvent &event )
{
	auto onMouseMove = [this]( const Input::MouseMoveEvent &e )
	{
		_mouseState.onMove( e.x, e.y );
	};

	auto onMouseButton = [this]( const Input::MouseButtonEvent &e )
	{
		if( e.eventType == Input::MouseButtonEvent::Press )
			_mouseState.onPress(  e.button, e.modifiers );
		else if( e.eventType == Input::MouseButtonEvent::Release )
		{
			static auto before = std::chrono::system_clock::now();
			const auto now     = std::chrono::system_clock::now();
			const auto diff    = std::chrono::duration_cast<std::chrono::milliseconds>( now - before );
			static uint32_t lastButtonClicked = 0; // last pressed button

			// check for double click
			if( e.button == lastButtonClicked && diff < kDoubleClickTime )
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
		//else if( e.eventType == Input::MouseButtonEvent::DoubleClick )  _mouseState.onDoubleClick( e.button, e.modifiers );
	};

	auto onMouseWheel = [this]( const Input::MouseWheelEvent &e )
	{
		_mouseState.onWheel( e.deltax, e.deltay, 0/*, e.modifiers*/ );
	};




	// Update the mouse state based on the event type
	// use visitor pattern to handle different mouse events instead of using if-else chains
	std::visit( MouseEventVisitorHelper 
				{
					[&] ( const Input::MouseMoveEvent   &e ) { onMouseMove( e ); },
					[&] ( const Input::MouseButtonEvent &e ) { onMouseButton( e ); },
					[&] ( const Input::MouseWheelEvent  &e ) { onMouseWheel( e ); },
				}, event );
}