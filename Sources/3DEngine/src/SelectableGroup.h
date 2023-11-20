// SelectableGroup.h
//
#ifndef SCENE_SELECTABLEGROUP_H
#define SCENE_SELECTABLEGROUP_H

#include "s2Scene_API.h"

#include "SelectionSet.h"

#include "Core/Observer.h"

#include <memory>
#include <set>
#include <vector>
#include <any>

namespace Scene 
{

class ObjectSelectable;

class SelectableGroup;
typedef std::shared_ptr<SelectableGroup> SelectableGroupPtr;

// ------------------------------------------------------------------------------------------------
/**
	it manages the selection status of any object added to the group according to the policy

	it does not contribute to ObjectSelectable lifetime by smart pointer
	an object is automatically removed from its SelectableGroup when on destruction

*/
class S2SCENE_API SelectableGroup : public Observer, public std::enable_shared_from_this<SelectableGroup>
{
private:
	using ObjectSelectablePtr     = std::shared_ptr<ObjectSelectable>;
	using ObjectSelectableWeakPtr = std::weak_ptr<ObjectSelectable>;


public:
	static SelectableGroupPtr	New() { return std::make_shared<SelectableGroup>(); }

public:
	SelectableGroup();
	SelectableGroup( SelectableGroup & ) = delete;
	virtual ~SelectableGroup();

	SelectableGroup &operator=( SelectableGroup & ) = delete;


	void								 enableSelection    ( bool enable );
	bool								 setSelectionPolicy ( const SelectionSet::Policy &policyMask );

	bool								 isSelectionEnabled () const { return _selectionEnabled; }
	SelectionSet::Policy				 selectionPolicy    () const { return _selectionSet.policy(); }


	bool								 addObject          ( ObjectSelectable *obj );
	void								 removeObject       ( ObjectSelectable *obj );
	void								 removeAllObjects   ();

	std::vector< ObjectSelectable* >     objects            () const { return {_selectableObject.begin(), _selectableObject.end()}; }
	unsigned int                         objectCount        () const { return int(_selectableObject.size()); }


	void								 clearSelection     ();
	bool								 selectObject       ( ObjectSelectable* obj, bool select = true );

	unsigned int                         selectedObjectCount() const { return _selectionSet.itemsCount(); }
	std::vector< ObjectSelectable* >     selectedObjects    () const { return _selectionSet.items(); };

	
	void                                 notify( Observable *o, const std::any &message ) override;

protected:
	void								 onObjectDestroyed( ObjectSelectable* obj );
	void                                 fulfillSelectionNotEmpty();

	void                                 checkInvariant();

protected:
	SelectionSet						_selectionSet;
	bool								_selectionEnabled;

	std::set< ObjectSelectable* >		_selectableObject;
};

}

#endif
