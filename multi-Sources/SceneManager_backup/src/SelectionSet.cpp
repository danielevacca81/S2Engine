// SelectionSet.cpp
//
#include "SelectionSet.h"

#include <cassert>
#include <algorithm>

using namespace Scene;


// --------------------------------------------------------------------------------------------
void SelectionSet::setPolicy( Policy policy, std::vector<ObjectSelectable*> *removedObject )
{
	if( removedObject ) removedObject->clear();

	if( _selectionPolicy == policy )
		return;

	_selectionPolicy = policy;

	if( _selectionPolicy & Disabled ) 
	{
		clear( IgnorePolicy,removedObject );
		return;
	}
	if( _selectionPolicy & Multiple )
		return;
	if( _selectionPolicy & Single )
	{
		if( _selectionSet.size() <= 1 )
			return;

		const auto it = std::max_element( _selectionSet.begin(), _selectionSet.end(), []( const auto &a, const auto &b ) {
			return a.second < b.second;
		} );

		const auto remainedItem = *it;

		if( removedObject )
			for( const auto &item: _selectionSet )
				if( remainedItem.first != item.first )
					removedObject->push_back( item.first );

		clear( IgnorePolicy );
		_selectionSet.insert( remainedItem );
		return;
	}
}

// --------------------------------------------------------------------------------------------
// Add the desired object to the selectionlist based on current selection policy
bool SelectionSet::addItem( ObjectSelectable* obj, std::vector<ObjectSelectable*> *removedObject )
{
	if( removedObject ) removedObject->clear();

	if( _selectionPolicy == Policy::Disabled )
		return false;

	const auto it = _selectionSet.find( obj );
	if( it != _selectionSet.end() )
	{
		it->second = ++_insertionCount;
		return true;
	}

	if( _selectionPolicy & Policy::Single )
	{
		assert( itemsCount() <= 1 );

		if( removedObject && _selectionSet.size() ) 
			removedObject->push_back( _selectionSet.begin()->first );

		_selectionSet.clear();
		_selectionSet[ obj ] = ++_insertionCount;
		return true;
	}

	if( _selectionPolicy & Policy::Multiple )
	{
		_selectionSet[ obj ] = ++_insertionCount;
		return true;
	}
	
	return false;
}

// --------------------------------------------------------------------------------------------
bool SelectionSet::removeItem( ObjectSelectable* obj, const ClearMode &mode,
							   std::vector<ObjectSelectable*> *removedObject )
{
	if( removedObject ) removedObject->clear();

	const auto it = _selectionSet.find( obj );
	if( it == _selectionSet.end() )
		return false;

	if( _selectionPolicy & Policy::NotEmpty && mode == AccordingToPolicy &&
		_selectionSet.size() == 1 )
		return false;

	if( removedObject ) removedObject->push_back( it->first );
	_selectionSet.erase( it );
	return true;
}

// --------------------------------------------------------------------------------------------
bool SelectionSet::clear( const ClearMode &mode, std::vector<ObjectSelectable*> *removedObject )
{
	if( removedObject ) removedObject->clear();

	bool retval =  mode == IgnorePolicy  ||  !(_selectionPolicy & Policy::NotEmpty);
	if( retval )
	{
		if( removedObject )
			for( const auto &item: _selectionSet )
				removedObject->push_back( item.first );

		_selectionSet.clear();
		return true;
	}
	
	return retval;
}

// --------------------------------------------------------------------------------------------
std::vector<ObjectSelectable*> SelectionSet::items() const
{
	std::vector<ObjectSelectable*> retval;

	for( const auto &item: _selectionSet )
		retval.push_back( item.first );

	return retval;
}



