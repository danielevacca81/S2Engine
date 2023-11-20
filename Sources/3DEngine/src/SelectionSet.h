// SelectionSet.h
//
#ifndef SELECTIONSET_ONCE
#define SELECTIONSET_ONCE

#include "s2Scene_API.h"

#include <map>
#include <cstdint>
#include <vector>

namespace Scene {

class ObjectSelectable;

class S2SCENE_API SelectionSet //@todo:template al posto di ObjectSelectable
{
public:
	enum Policy
	{
		Disabled = 0x0001,
		Single   = 0x0002,
		Multiple = 0x0004,
		NotEmpty = 0x0010,
		Default  = Multiple
	};

	enum ClearMode
	{
		AccordingToPolicy,
		IgnorePolicy,
	};

private:
	using ObjectOrederMap = std::map< ObjectSelectable*, uint64_t >;

public:
	void						  setPolicy ( Policy policy, std::vector<ObjectSelectable*> *removedObject = nullptr );
	Policy						  policy()	const { return _selectionPolicy; }
								  
	bool						  addItem   ( ObjectSelectable* obj, std::vector<ObjectSelectable*> *removedObject = nullptr );
	bool						  removeItem( ObjectSelectable* obj, const ClearMode &mode, std::vector<ObjectSelectable*> *removedObject = nullptr );
	bool						  clear     ( const ClearMode &mode, std::vector<ObjectSelectable*> *removedObject = nullptr );
								  
								  
	std::vector<ObjectSelectable*>  items()      const;
	bool                          isEmpty()	   const { return _selectionSet.empty(); }
	int                           itemsCount() const { return (int) _selectionSet.size(); }

private:
	Policy           _selectionPolicy = Policy::Multiple;
	uint64_t         _insertionCount = 0;
	ObjectOrederMap  _selectionSet;
};


}
#endif