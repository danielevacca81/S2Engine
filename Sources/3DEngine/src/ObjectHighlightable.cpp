// ObjectHighlightable.cpp
//
#include "ObjectHighlightable.h"

using namespace Scene;

// ------------------------------------------------------------------------------------------------
ObjectHighlightable::ObjectHighlightable() 
: _highlightPolicy ( HighlightPolicy::Any )
, _highlightByCode ( false )
, _highlightByMouse( false )
{
	updateHighlightSatus( false );
}

// ------------------------------------------------------------------------------------------------
void ObjectHighlightable::setVisible( bool visible )
{
	ObjectInteraction::setVisible( visible );
	updateHighlightSatus( true );
}

// ------------------------------------------------------------------------------------------------
void ObjectHighlightable::setEnabled( bool enabled )
{
	ObjectInteraction::setEnabled( enabled );
	updateHighlightSatus( true );
}

// ------------------------------------------------------------------------------------------------
void ObjectHighlightable::setPolicy( const HighlightPolicy policy )
{
	_highlightPolicy = policy;
	updateHighlightSatus( true );
}

// ------------------------------------------------------------------------------------------------
void ObjectHighlightable::enterEvent( SceneEvent *event )
{
	_highlightByMouse = true;
	updateHighlightSatus( true );
}

// ------------------------------------------------------------------------------------------------
void ObjectHighlightable::leaveEvent( SceneEvent *event ) 
{
	_highlightByMouse = false;
	updateHighlightSatus( true );
}

// ------------------------------------------------------------------------------------------------
void ObjectHighlightable::setHighlighted( bool hilighted )
{
	_highlightByCode = hilighted;
	updateHighlightSatus( true );
}

// ------------------------------------------------------------------------------------------------
void ObjectHighlightable::updateHighlightSatus( bool emitIfChanged )
{
	const auto oldStatus = _highlightStatus;

	_highlightStatus = isVisible() && isEnabled() && (
		_highlightByCode  && ( int(_highlightPolicy) & int(HighlightPolicy::ByCode) ) ||
		_highlightByMouse && ( int(_highlightPolicy) & int(HighlightPolicy::Mouseover) ) );
							
	if( oldStatus != _highlightStatus )
		notifyObservers( HighlightedSignal{_highlightStatus} );
}

