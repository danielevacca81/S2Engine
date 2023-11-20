// SelectableGroup.cpp
//
#include "SelectableGroup.h"

#include "ObjectSelectable.h"


using namespace Scene;

// ------------------------------------------------------------------------------------------------
SelectableGroup::SelectableGroup()
:  	_selectionEnabled( true )
{
}

// ------------------------------------------------------------------------------------------------
SelectableGroup::~SelectableGroup()
{
	removeAllObjects();
}

// ------------------------------------------------------------------------------------------------
void SelectableGroup::enableSelection( bool enable )                              
{ 
	if( enable == _selectionEnabled )
		return;

	_selectionEnabled = enable; 
	if( !enable ) 
		clearSelection(); 
	else 
		fulfillSelectionNotEmpty();
}

// ------------------------------------------------------------------------------------------------
bool SelectableGroup::setSelectionPolicy( const SelectionSet::Policy &policyMask )
{ 
	if( !_selectableObject.empty() )
		return false;

	_selectionSet.setPolicy( policyMask );
	return true;
}

// ------------------------------------------------------------------------------------------------
bool SelectableGroup::addObject( ObjectSelectable *obj )
{
	// Null objects are discarded
	if( !obj )
		return false;

	// If object with the same id exists, this means that object is already inserted
	const auto it = _selectableObject.find( obj );
	if( it != _selectableObject.end() )
		return true;

	if( obj->_selectableGroup )
	{
		assert( obj->_selectableGroup.get() != this );
		obj->_selectableGroup->removeObject( obj );
	}

	obj->registerObserver( this );
	obj->_selectableGroup = shared_from_this();
	_selectableObject.insert( obj );
	if( obj->isSelected() )
	{
		std::vector<ObjectSelectable*> unselectedObject;
		if( !_selectionSet.addItem( obj, &unselectedObject ) ) 
			unselectedObject.push_back( obj );

		for( auto *obj: unselectedObject )
			obj->doSetSelected( false );
	}

	fulfillSelectionNotEmpty();

	return true;
}

// ------------------------------------------------------------------------------------------------
void SelectableGroup::removeObject( ObjectSelectable *obj )
{
	const auto it = _selectableObject.find( obj );
	if( it == _selectableObject.end() )
		return;

	obj->unregisterObserver( this );
	_selectableObject.erase( obj );
	_selectionSet.removeItem( obj, SelectionSet::IgnorePolicy );

	fulfillSelectionNotEmpty();

	obj->_selectableGroup = nullptr;
}

// ------------------------------------------------------------------------------------------------
void SelectableGroup::removeAllObjects( )
{
	auto tmpSelectableObject = std::move( _selectableObject );

	for( auto *obj: tmpSelectableObject )
		obj->unregisterObserver( this );

	_selectableObject.clear();
	_selectionSet.clear( SelectionSet::IgnorePolicy );

	for( auto *obj: tmpSelectableObject )
		obj->_selectableGroup = nullptr;
}

// ------------------------------------------------------------------------------------------------
void SelectableGroup::clearSelection()
{
	std::vector<ObjectSelectable*> unselectedObject;
	_selectionSet.clear( SelectionSet::AccordingToPolicy, &unselectedObject );

	for( auto *obj: unselectedObject )
		obj->doSetSelected( false );
}

// ------------------------------------------------------------------------------------------------
bool SelectableGroup::selectObject( ObjectSelectable* obj, bool select )
{
	const auto it = _selectableObject.find( obj );
	if( it == _selectableObject.end() )
	{
		checkInvariant();
		return false;
	}

	if( select == obj->isSelected() )
	{
		obj->doSetSelected( select );
		checkInvariant();
		return true;
	}

	std::vector<ObjectSelectable*> unselectedObject;
	if( select )
	{	
		if( !_selectionEnabled )
		{
			checkInvariant();
			return false;
		}

		bool success = _selectionSet.addItem( obj, &unselectedObject );

		for( auto *uObj: unselectedObject )
			uObj->doSetSelected( false );

		if( success )
			obj->doSetSelected( true );
	}
	else
	{
		_selectionSet.removeItem( obj, SelectionSet::AccordingToPolicy, &unselectedObject );
		for( auto *uObj: unselectedObject )
			uObj->doSetSelected( false );
	}

	checkInvariant();
	return obj->isSelected() == select;
}

// ------------------------------------------------------------------------------------------------
void SelectableGroup::notify( Observable *o, const std::any &message )
{
	if( message.type() == typeid( ObjectInteraction::DestroyedSignal ) )
	{
		auto destroyedSignal = std::any_cast<ObjectInteraction::DestroyedSignal>( message );
		auto obj = dynamic_cast<ObjectSelectable*>( o );
		onObjectDestroyed( obj );
	} 
}

// ------------------------------------------------------------------------------------------------
void SelectableGroup::onObjectDestroyed( ObjectSelectable* obj )
{
	assert( _selectableObject.find( obj ) != _selectableObject.end() );
	_selectableObject.erase( obj );
	_selectionSet.removeItem( obj, SelectionSet::IgnorePolicy );

	fulfillSelectionNotEmpty();
}

// ------------------------------------------------------------------------------------------------
void SelectableGroup::fulfillSelectionNotEmpty()
{
	if( _selectionSet.policy() & SelectionSet::NotEmpty &&
		_selectionSet.policy() & (SelectionSet::Single|SelectionSet::Multiple) && 
		_selectionSet.itemsCount() == 0 && !_selectableObject.empty() )
	{
		selectObject( *_selectableObject.begin() );
	}
}

// ------------------------------------------------------------------------------------------------
void SelectableGroup::checkInvariant()
{
#ifndef _DEBUG
	return;
#endif
	
	std::vector<ObjectSelectable*> selected =  _selectionSet.items();

	for( auto *o: _selectableObject )
	{
		const auto it = std::find( selected.begin(), selected.end(), o );

		if( o->isSelected() )
			assert( it != selected.end() );
		else
			assert( it == selected.end() );
	}

	if( _selectionSet.policy() == SelectionSet::Single )
		assert( selected.size() <= 1 );
	else
	{
		// todo
	}
}



