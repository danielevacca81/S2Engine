// SelectionSet.cpp
//
#include "SelectionSet.h"

using namespace s2;

// --------------------------------------------------------------------------------------------
SelectionSet::SelectionSet()
: _selectionPolicy( SelectionDefault )
{}

// --------------------------------------------------------------------------------------------
SelectionSet::~SelectionSet()
{}

// --------------------------------------------------------------------------------------------
// Add the desired object to the selectionlist based on current selection policy
SelectionSet::SelectionList SelectionSet::addEntry( unsigned int id )
{
	if( _selectionPolicy == SelectionNone )
		return _selectionList;
	
	if( _selectionPolicy & SelectionSingle )
	{
		if( _selectionPolicy & SelectionNotEmpty )
		{
			// single-non-empty-selection: add the current one only if the set is empty
			if( isEmpty() )
				store( id );
			else
			{
				remove( _selectionList.front() );
				store( id );
			}
		}
		else
		{
			// single-maybe-empty-selection: remove any other selected and add the current one
			reset();
			store( id );
		}
	}
	else if( _selectionPolicy & SelectionMultiple )
	{
		store( id );
	}
	
	return _selectionList;
}

// --------------------------------------------------------------------------------------------
SelectionSet::SelectionList SelectionSet::removeEntry( unsigned int id )
{
	SelectionIndex::iterator it = _selectionIndex.find( id );

	if( it == _selectionIndex.end() )
		return _selectionList; // object not found. should never happen

	if( _selectionPolicy & SelectionSingle )
	{
		if( _selectionPolicy & SelectionNotEmpty )
		{
			// single-non-empty-selection: prevent to remove the selected object
			if( !isEmpty() )
				reset();
		}
		else
		{
			// single-maybe-empty-selection: remove all entries
			reset();
		}
	}
	else if( _selectionPolicy & SelectionMultiple )
	{
		if( _selectionPolicy & SelectionNotEmpty )
		{
			if( size() > 1 )
				remove( id );
		}
		else
		{
			remove( id );
		}
	}

	return _selectionList;
}

// --------------------------------------------------------------------------------------------
void SelectionSet::store( unsigned int id )
{
	_selectionList.push_back( id );
	_selectionIndex[ id ] = (--_selectionList.end());
}

// --------------------------------------------------------------------------------------------
void SelectionSet::remove( unsigned int id )
{
	auto it = _selectionIndex.find(id);

	if( it != _selectionIndex.end() )
	{
		_selectionList.erase( it->second );
		_selectionIndex.erase( it );
	}
}

// --------------------------------------------------------------------------------------------
void SelectionSet::reset()
{
	//if( _selectionPolicy & PolicyNotEmptySelection )
	//	return;

	_selectionList.clear();
	_selectionIndex.clear();
}