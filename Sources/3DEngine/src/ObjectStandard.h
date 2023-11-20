// ObjectStandard.h
//
#ifndef SCENE_OBJECTSTANDARD_H
#define SCENE_OBJECTSTANDARD_H

#include "s2Scene_API.h"

#include "ObjectSelectable.h"
#include "ObjectHighlightable.h"

namespace Scene 
{

class ObjectStandard;
typedef std::shared_ptr<ObjectStandard> ObjectStandardPtr;

class S2SCENE_API ObjectStandard : public ObjectSelectable, public ObjectHighlightable
{
public:
	ObjectStandard();
	~ObjectStandard() { callDestroyedSignal(); }

	void            setVisible( bool visible )       override; 	    
	void            setEnabled( bool enabled )       override; 

	void            enterEvent( SceneEvent *event )  override;
	void            leaveEvent( SceneEvent *event )  override;
};

}

#endif


