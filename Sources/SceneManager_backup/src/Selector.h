/*

// ------------------------------------------------------------------------------------------------
void Selector::setSelectionEnabled( bool enabled ) { _selectionEnabled = enabled;  }

// ------------------------------------------------------------------------------------------------
void Selector::setTargetSelected( const GPR::TargetPtr &t, bool selected )
{
	if( !_selectionEnabled )
		return;

	const auto found = findByTarget( t );
		
	if( !found.has_value() )
		return; // nothing to select

	found.value()->setSelected( selected );
}

// ------------------------------------------------------------------------------------------------
void Selector::setTargetHilighted( const GPR::TargetPtr &t )
{
	if( !_selectionEnabled )
		return;

	const auto found = findByTarget( t );
		
	if( !found.has_value() )
		return; // nothing tohilight

	auto obj = found.value();

	if( _hilightedObject == obj )
		return;

	// undo highlighting on the current object
	if( _hilightedObject )
		_hilightedObject->setHilighted( false );

	// set highlighting to the hovered object (if any)
	if( obj )
		obj->setHilighted( true );

	// get new highlighted object (it may be NULL)
	_hilightedObject = obj;
}

// ------------------------------------------------------------------------------------------------
void Selector::clearHilight()
{
	// undo highlighting on the current object
	if( _hilightedObject )
        _hilightedObject->setHilighted( false );
	
	_hilightedObject = nullptr;
}




*/