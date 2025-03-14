// ObjectClickable.cpp
//
#include "ObjectClickable.h"

#include "SceneManager.h"

#include <iostream>

using namespace Scene;

//int countgg = 0;

// ------------------------------------------------------------------------------------------------
ObjectClickable::ObjectClickable() 
	: _clickInProgress( false )
{
}

// ------------------------------------------------------------------------------------------------
void ObjectClickable::mousePressEvent( SceneMouseEvent *event ) 
{
	ObjectInteraction::mousePressEvent( event );

	_clickInProgress = true;
	_lastMouseDown = *event;

	//std::cout << ++countgg << " mousePressEvent " << id() << "\n";
}

// ------------------------------------------------------------------------------------------------
void ObjectClickable::mouseReleaseEvent( SceneMouseEvent *event ) 
{
	ObjectInteraction::mouseReleaseEvent( event );

	//std::cout << ++countgg << " mouseReleaseEvent " << id() << "\n";


	if( _clickInProgress && _lastMouseDown.button == event->button )
	{
		_clickInProgress = false;

		auto deltaPos = Math::abs( event->sceneViewportPos - _lastMouseDown.sceneViewportPos );
		if( Math::max( deltaPos.x, deltaPos.y ) > startDragDistance() )
			return;

		MouseClickSignal ev;
		static_cast<SceneMouseEvent&>(ev) = *event;
		onMouseClick( &ev );
		event->accepted = ev.accepted;

		std::any msg( ev );
		notifyObservers( msg );	
		event->accepted = std::any_cast<Scene::ObjectClickable::MouseClickSignal>( &msg )->accepted;
	}
}

// ------------------------------------------------------------------------------------------------
void ObjectClickable::mouseMoveEvent( SceneMouseEvent *event ) 
{
	ObjectInteraction::mouseMoveEvent( event );

	//std::cout << ++countgg << " mouseMoveEvent " << id() << "\n";

	if( _clickInProgress )
	{
		auto deltaPos = Math::abs( event->sceneViewportPos - _lastMouseDown.sceneViewportPos );
		if( Math::max( deltaPos.x, deltaPos.y ) > startDragDistance() )
			_clickInProgress = false;
	}
}

// ------------------------------------------------------------------------------------------------
void ObjectClickable::enterEvent( SceneEvent *event ) 
{
	//std::cout << ++countgg << " enterEvent " << id() << "\n";

	_clickInProgress = false;
}

// ------------------------------------------------------------------------------------------------
void ObjectClickable::leaveEvent( SceneEvent *event ) 
{
	//std::cout << ++countgg << " leaveEvent " << id() << "\n";

	_clickInProgress = false;
}

// ------------------------------------------------------------------------------------------------
void  ObjectClickable::onMouseClick( MouseClickSignal *signal )
{
	//std::cout << ++countgg << " onMouseClick " << id() << "\n";
}
