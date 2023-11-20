// SelectionSet.cpp
//
#include "SelectionSet.h"

using namespace s2::SceneGraph;

// --------------------------------------------------------------------------------------------
SelectionSet::SelectionSet()
: _selectionPolicy( Policy::Multiple )
{}

// --------------------------------------------------------------------------------------------
SelectionSet::~SelectionSet()
{};

// --------------------------------------------------------------------------------------------
// Add the desired object to the selectionlist based on current selection policy
SelectionSet::Items SelectionSet::addItem( uint64_t objID )
{
	if( _selectionPolicy == Policy::Disabled )
		return _selectionList;
	
	//Index::const_iterator it = _selectionIndex.find(objID);
	
	//bool added = false;
	if( _selectionPolicy & Policy::Single )
	{
		if( _selectionPolicy & Policy::NotEmpty )
		{
			// single-non-empty-selection: add the current one only if the set is empty
			if( isEmpty() )
				add( objID );
			else
			{
				remove( _selectionList.front() );
				add( objID );
			}
		}
		else
		{
			// single-maybe-empty-selection: remove any other selected and add the current one
			clear( AccordingToPolicy );
			add(objID);
		}
	}
	else if( _selectionPolicy & Policy::Multiple )
	{
		add(objID);
		//if( _selectionPolicy & PolicyNotEmptySelection )
		//{
		//	add( objID );
		//}
		//else
		//{
		//	add( objID );
		//}
	}
	
	return _selectionList;

	//case PolicyExclusive:	// Remove it if already present
	//	if( it != _selectionIndex.end() )
	//	{
	//		_selectionList.erase( it->second );
	//		_selectionIndex.erase( it );
	//	}
	//	else
	//	{
	//		_selectionList.push_back( objID );
	//		_selectionIndex[ objID ] = (--_selectionList.end());
	//		added = true;
	//	}
	//	break; 
	//
	//case PolicyAdd:
	//	added = true;
	//	if( it == _selectionIndex.end() )
	//	{
	//		_selectionList.push_back( objID );
	//		_selectionIndex[ objID ] = (--_selectionList.end());
	//	}
	//	break;
	//}

	//return added;
}

// --------------------------------------------------------------------------------------------
SelectionSet::Items SelectionSet::removeItem( uint64_t objID )
{
	Index::iterator it = _selectionIndex.find(objID);

	if( it == _selectionIndex.end() )
		return _selectionList; // object not found. nothing changes

	if( _selectionPolicy & Policy::Single )
	{
		if( _selectionPolicy & Policy::NotEmpty )
		{
			// single-non-empty-selection: prevent to remove the selected object
			if( !isEmpty() )
				clear( AccordingToPolicy );
		}
		else
		{
			// single-maybe-empty-selection: remove all entries
			clear( AccordingToPolicy );
		}
	}
	else if( _selectionPolicy & Policy::Multiple )
	{
		if( _selectionPolicy & Policy::NotEmpty )
		{
			if( itemsCount() > 1 )
				remove( objID );
		}
		else
		{
			remove( objID );
		}
	}

	return _selectionList;
}

// --------------------------------------------------------------------------------------------
void SelectionSet::add( uint64_t objID )
{
	_selectionList.push_back( objID );
	_selectionIndex[ objID ] = (--_selectionList.end());
}

// --------------------------------------------------------------------------------------------
void SelectionSet::remove( uint64_t objID )
{
	Index::iterator it = _selectionIndex.find(objID);

	if( it != _selectionIndex.end() )
	{
		_selectionList.erase( it->second );
		_selectionIndex.erase( it );
	}
}

// --------------------------------------------------------------------------------------------
void SelectionSet::clear( const ClearMode &mode )
{
	if( mode == IgnorePolicy )
	{
		_selectionList.clear();
		_selectionIndex.clear();
		return;
	}


	if( _selectionPolicy & Policy::NotEmpty )
		return;

	//if( _selectionPolicy & PolicyMultipleSelection )
	//	return;

	_selectionList.clear();
	_selectionIndex.clear();
}