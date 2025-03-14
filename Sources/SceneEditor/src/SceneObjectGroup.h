// SceneObjectCollection.h
//
#ifndef SCENEOBJECTCOLLECTION_H
#define SCENEOBJECTCOLLECTION_H

#include "RenderPipeline_API.h"

#include "SceneObject.h"
#include "SceneEvent.h"
#include "ObjectInteraction.h"
#include "ObjectClickable.h"
#include "View.h"

#include <set>

namespace SceneManager {

class RENDERPIPELINE_API SceneObjectGroup
{
public:
	virtual bool addObject( const SceneObjectPtr& obj );
	virtual void removeObject( const SceneObjectPtr& obj );
	virtual void removeObject( SceneObject* obj );
	virtual void removeAllObjects();

private:
	std::set< SceneObject* > _objects;

};

}
#endif