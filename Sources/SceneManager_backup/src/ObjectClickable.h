// ObjectClickable.h
//
#ifndef RENDERENGINE_OBJECTCLICKABLE_H
#define RENDERENGINE_OBJECTCLICKABLE_H

#include "RenderEngine_API.h"

#include "ObjectInteraction.h"

namespace Scene 
{

class ObjectClickable;
typedef std::shared_ptr<ObjectClickable> ObjectClickablePtr;

class RENDERENGINE_API ObjectClickable : public virtual ObjectInteraction
{
public:
	struct MouseClickSignal : public SceneMouseEvent { };

public:
	ObjectClickable();
	~ObjectClickable() { callDestroyedSignal(); }

	void          mousePressEvent  ( SceneMouseEvent *event )  override;
	void          mouseReleaseEvent( SceneMouseEvent *event )  override;
	void          mouseMoveEvent   ( SceneMouseEvent *event )  override;
	void          enterEvent       ( SceneEvent      *event )  override;
	void          leaveEvent       ( SceneEvent      *event )  override;

protected:
	virtual void  onMouseClick( MouseClickSignal *signal );

protected:
	bool				_clickInProgress;
	SceneMouseEvent	    _lastMouseDown;
};

}

#endif
