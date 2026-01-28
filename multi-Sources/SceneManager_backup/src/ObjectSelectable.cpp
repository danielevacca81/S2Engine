// ObjectSelectable.cpp
//
#include "ObjectSelectable.h"

#include "SceneManager.h"

using namespace Scene;

// ------------------------------------------------------------------------------------------------
ObjectSelectable::ObjectSelectable() 
: _selected ( false )
{
}

// ------------------------------------------------------------------------------------------------
void ObjectSelectable::setVisible( bool visible )
{
	ObjectInteraction::setVisible( visible );
	if( !isVisible() )
		setSelected( false );
}

// ------------------------------------------------------------------------------------------------
void ObjectSelectable::setEnabled( bool enabled )
{
	ObjectInteraction::setEnabled( enabled );
	if( !isEnabled() )
		setSelected( false );
}

// ------------------------------------------------------------------------------------------------
bool ObjectSelectable::setSelected( bool selected )
{
	if( selected && (!isVisible() || !isEnabled()) )
	{
		assert( !_selected );
		return selected == _selected;
	}

	if( _selectableGroup )
		_selectableGroup->selectObject( this, selected );
	else
		doSetSelected( selected );

	return selected == _selected;
}

// ------------------------------------------------------------------------------------------------
void ObjectSelectable::doSetSelected( bool selected )
{
	if( selected == _selected )
		return;

	_selected = selected;
	notifyObservers( SelectedSignal{selected} );
}

// ------------------------------------------------------------------------------------------------
void ObjectSelectable::onMouseClick( MouseClickSignal *signal )
{
	if( auto sceneMgr = manager().lock() )
	{
		SceneMouseEvent::Button           selectMouseButton;
		SceneMouseEvent::KeyboardModifier selectModifiers;

		sceneMgr->selectionMouseButton( selectMouseButton, selectModifiers );

		if( signal->button == selectMouseButton && signal->modifiers == selectModifiers )
			setSelected( true );
	}

	ObjectClickable::onMouseClick( signal );
}


