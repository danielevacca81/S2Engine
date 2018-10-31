// SelectionSet.h
//
#ifndef SELECTIONSET_ONCE
#define SELECTIONSET_ONCE

#include "s2SceneGraph_API.h"


#include <list>
#include <map>

namespace s2 {
namespace SceneGraph {

class S2SCENEGRAPH_API SelectionSet
{
public:
	enum Policy
	{
		Disabled = 0x0000,
		Single   = 0x0001,
		Multiple = 0x0002,
		NotEmpty = 0x0010,
		Default  = Multiple
	};

	enum ClearMode
	{
		AccordingToPolicy,
		IgnorePolicy,
	};


	typedef std::list<uint64_t>							 Items;		// list of object identifiers
	typedef std::map<uint64_t, Items::iterator>  Index;	// auxiliary indexing map for faster searches <Key=objID,value=>

public:
	SelectionSet();
	~SelectionSet();

	Items addItem( uint64_t objID );
	Items removeItem( uint64_t objID );

	const Items &items()             const { return _selectionList; }
	void setPolicy( Policy policy )                { _selectionPolicy = policy; }
	Policy policy()				             const { return _selectionPolicy; }
	bool isEmpty()							 const { return _selectionList.empty(); }
	int itemsCount()                         const { return (int) _selectionList.size(); }

	void clear( const ClearMode &mode );

private:
	void add( uint64_t );
	void remove( uint64_t );


private:
	Items  _selectionList;
	Index  _selectionIndex;
	Policy _selectionPolicy;
};


}}
#endif