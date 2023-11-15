// SelectionSet.cpp
//
#include "SelectionSet.h"

#include "Utils/MemCheck.h"

// --------------------------------------------------------------------------------------------
SelectionSet::SelectionSet()
: _selectionPolicy(PolicyMultipleSelection)
{}

// --------------------------------------------------------------------------------------------
SelectionSet::~SelectionSet()
{};

// --------------------------------------------------------------------------------------------
// Add the desired object to the selectionlist based on current selection policy
SelectionSet::SelectionList SelectionSet::addObject( unsigned int objID )
{
	if( _selectionPolicy == PolicyNoSelection )
		return _selectionList;
	
	//SelectionIndex::const_iterator it = _selectionIndex.find(objID);
	
	//bool added = false;
	if( _selectionPolicy & PolicySingleSelection )
	{
		if( _selectionPolicy & PolicyNotEmptySelection )
		{
			// single-non-empty-selection: add the current one only if the set is empty
			if( isEmpty() )
				addEntry( objID );
			else
			{
				removeEntry( _selectionList.front() );
				addEntry( objID );
			}
		}
		else
		{
			// single-maybe-empty-selection: remove any other selected and add the current one
			reset();
			addEntry(objID);
		}
	}
	else if( _selectionPolicy & PolicyMultipleSelection )
	{
		addEntry(objID);
		//if( _selectionPolicy & PolicyNotEmptySelection )
		//{
		//	addEntry( objID );
		//}
		//else
		//{
		//	addEntry( objID );
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
SelectionSet::SelectionList SelectionSet::removeObject( unsigned int objID )
{
	SelectionIndex::iterator it = _selectionIndex.find(objID);

	if( it == _selectionIndex.end() )
		return _selectionList; // object not found. should never happen

	if( _selectionPolicy & PolicySingleSelection )
	{
		if( _selectionPolicy & PolicyNotEmptySelection )
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
	else if( _selectionPolicy & PolicyMultipleSelection )
	{
		if( _selectionPolicy & PolicyNotEmptySelection )
		{
			if( size() > 1 )
				removeEntry( objID );
		}
		else
		{
			removeEntry( objID );
		}
	}

	return _selectionList;
}

// --------------------------------------------------------------------------------------------
void SelectionSet::addEntry( unsigned int objID )
{
	_selectionList.push_back( objID );
	_selectionIndex[ objID ] = (--_selectionList.end());
}

// --------------------------------------------------------------------------------------------
void SelectionSet::removeEntry( unsigned int objID )
{
	SelectionIndex::iterator it = _selectionIndex.find(objID);

	if( it != _selectionIndex.end() )
	{
		_selectionList.erase( it->second );
		_selectionIndex.erase( it );
	}
}


// --------------------------------------------------------------------------------------------
void SelectionSet::reset()
{
	if( _selectionPolicy & PolicyNotEmptySelection )
		return;

	_selectionList.clear();
	_selectionIndex.clear();
}