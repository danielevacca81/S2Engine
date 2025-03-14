// ObjectStandard.cpp
//
#include "ObjectStandard.h"

using namespace Scene;

// ------------------------------------------------------------------------------------------------
ObjectStandard::ObjectStandard() 
{
}

// ------------------------------------------------------------------------------------------------
void ObjectStandard::setVisible( bool visible )
{
	ObjectHighlightable::setVisible( visible );
	ObjectSelectable::setVisible( visible );
}

// ------------------------------------------------------------------------------------------------
void ObjectStandard::setEnabled( bool enabled )
{
	ObjectHighlightable::setEnabled( enabled );
	ObjectSelectable::setEnabled( enabled );
}

// ------------------------------------------------------------------------------------------------
void ObjectStandard::enterEvent( SceneEvent *event )
{
	ObjectHighlightable::enterEvent( event );
	ObjectSelectable::enterEvent( event );
}

// ------------------------------------------------------------------------------------------------
void ObjectStandard::leaveEvent( SceneEvent *event ) 
{
	ObjectHighlightable::leaveEvent( event );
	ObjectSelectable::leaveEvent( event );
}
