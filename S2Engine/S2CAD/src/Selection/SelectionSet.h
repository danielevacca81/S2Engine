// SelectionSet.h
// 
// SelectionSet class
//
// @author D. Vacca
// @updated 14-sep-2009
//
#ifndef SELECTIONSET_H
#define SELECTIONSET_H

#include "S2CAD_API.h"

#include <list>
#include <map>

namespace s2 {

class S2CAD_API SelectionSet
{
public:
	enum SelectionPolicy 
	{ 
		SelectionNone     = 0x0000, 
		SelectionSingle   = 0x0001, 
		SelectionMultiple = 0x0002, 
		SelectionNotEmpty = 0x0010,
		SelectionDefault  = SelectionMultiple,
	};

	typedef std::list<unsigned int>							 SelectionList;		// list of object identifiers
	typedef std::map<unsigned int, SelectionList::iterator>  SelectionIndex;	// auxiliary indexing map for faster searches <Key=objID,value=>




	SelectionSet();
	~SelectionSet();

	SelectionList addEntry   ( unsigned int id );
	SelectionList removeEntry( unsigned int id );
	SelectionList &get()					       { return _selectionList; }
	void setPolicy( const SelectionPolicy &policy) { _selectionPolicy = policy; }
	SelectionPolicy getPolicy()				 const { return _selectionPolicy; }
	bool isEmpty()							 const { return _selectionList.empty(); }
	int  size()                              const { return (int)_selectionList.size(); }
	void reset();						    

private:
	SelectionList   _selectionList;
	SelectionIndex  _selectionIndex;
	SelectionPolicy _selectionPolicy;

	void store ( unsigned int );
	void remove( unsigned int );
};

}

#endif