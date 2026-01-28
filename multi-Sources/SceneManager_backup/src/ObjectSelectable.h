// ObjectSelectable.h
//
#ifndef SCENE_OBJECTSELECTABLE_H
#define SCENE_OBJECTSELECTABLE_H

#include "RenderEngine_API.h"

#include "ObjectClickable.h"
#include "SelectableGroup.h"

namespace Scene 
{

class ObjectSelectable;
typedef std::shared_ptr<ObjectSelectable> ObjectSelectablePtr;

class RENDERENGINE_API ObjectSelectable : public ObjectClickable
{
public:
	struct SelectedSignal { bool selected; };

public:
	ObjectSelectable();
	~ObjectSelectable() { callDestroyedSignal(); }

	void                     setVisible( bool visible ) override;
	void                     setEnabled( bool enabled ) override; 
	virtual bool             setSelected( bool selected );

	bool                     isSelected() const { return _selected; }

	const SelectableGroupPtr &selectableGroup() const { return _selectableGroup; }


protected:
	void			         onMouseClick( MouseClickSignal *signal ) override;

	friend SelectableGroup;
	virtual void             doSetSelected( bool selected );

protected:
	bool                _selected;
	SelectableGroupPtr	_selectableGroup;	
};

}

#endif


