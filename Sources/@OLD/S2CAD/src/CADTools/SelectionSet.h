// SelectionSet.h
//
#ifndef SELECTIONSET_H
#define SELECTIONSET_H

#include "Framework_Exports.h"

#include <list>
#include <map>

////---------------------------------------------------------------------------------------------
///**
//  * Selection Policy.
//  * Enumerator containing selection policies
//  *
//  * @author D. Vacca
//  * @updated 14-sep-2009
//  */
//
//// --------------------------------------------------------------------------------------------
//// Simple structure to handle multiple requests
//template <typename T>
//class SelectionRequest
//{
//public:
//	// --------------------------------------------------------------------------------------------
//	std::vector<T> types;
//	std::vector<int> reqQty;
//	std::vector<int> currQty;
//	int totObjectRequested;
//	// --------------------------------------------------------------------------------------------
//	
//public:
//	// --------------------------------------------------------------------------------------------
//	SelectionRequest(T t, int qty)
//	{
//		// ignore quantities on negative values of qty
//
//		types.clear();
//		reqQty.clear();
//		currQty.clear();
//
//		types.push_back(t);
//		reqQty.push_back(qty);
//		currQty.push_back(qty);
//		totObjectRequested = qty;
//	}
//
//	// --------------------------------------------------------------------------------------------
//	SelectionRequest(std::vector<T> t, std::vector<int> qty)
//	{
//		types.clear();
//		reqQty.clear();
//		currQty.clear();
//		totObjectRequested = 0;
//
//		for(size_t i=0; i<t.size(); ++i)
//		{
//			types.push_back(t[i]);
//			reqQty.push_back(qty[i]);
//			currQty.push_back(qty[i]);
//			totObjectRequested += qty[i];
//		}
//	}
//};

class FWK_API SelectionSet
{
public:
	enum SelectionPolicy
	{
		PolicyNoSelection       = 0x0000,
		PolicySingleSelection   = 0x0001,
		PolicyMultipleSelection = 0x0002,
		PolicyNotEmptySelection = 0x0010,
	};

	typedef std::list<unsigned int>							 SelectionList;		// list of object identifiers
	typedef std::map<unsigned int, SelectionList::iterator>  SelectionIndex;	// auxiliary indexing map for faster searches <Key=objID,value=>


private:
	SelectionList   _selectionList;
	SelectionIndex  _selectionIndex;
	SelectionPolicy _selectionPolicy;

	void addEntry( unsigned int );
	void removeEntry( unsigned int );

public:
	SelectionSet();
	~SelectionSet();

	SelectionList addObject( unsigned int objID );
	SelectionList removeObject( unsigned int objID );
	SelectionList &get()					      { return _selectionList; }
	void setPolicy(SelectionPolicy policy)	      { _selectionPolicy = policy; }
	SelectionPolicy getPolicy()				const { return _selectionPolicy; }
	bool isEmpty()							const { return _selectionList.empty(); }
	void reset();
	size_t size()                           const { return _selectionList.size(); }
};

#endif