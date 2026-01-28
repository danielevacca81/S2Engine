// SceneObjectCollection.cpp
//
#include "SceneObjectCollection.h"

using namespace SceneManager;

// ------------------------------------------------------------------------------------------------
bool SceneObjectCollection::addObject( const SceneObjectPtr& obj )
{
	AutoCheckInvariant aci( this );

	if( !obj )
		return false;

	auto oldManager = obj->manager().lock();
	if( oldManager.get() == this )
		return true;

	if( oldManager )
		oldManager->removeObject( obj );

	_objects.emplace( obj.get() );
	obj->addedToSceneManager( weak_from_this() );

	return true;
}

// ------------------------------------------------------------------------------------------------
void SceneObjectCollection::removeObject( const SceneObjectPtr& obj )
{
	auto objSharedPtr = obj;
	removeObject( objSharedPtr.get() );
}

// ------------------------------------------------------------------------------------------------
void SceneObjectCollection::removeObject( SceneObject* obj )
{
	if( !obj )
		return;

	AutoCheckInvariant aci( this );

	const auto it = _objects.find( obj );
	if( it == _objects.end() )
	{
		return;
	}

	if( auto objInteract = dynamic_cast<ObjectInteraction*>( obj ) )
	{
		if( _lastMouseEventObj.object.lock().get() == objInteract )
			_lastMouseEventObj.clearObj();

		removeFromGrabMouseStack( objInteract );

		resetObjectMatchingPickValue( objInteract->negotiatedPickValueOrigin() );
	}

	obj->removedFromSceneManager( this );

	_objects.erase( it );
}

// ------------------------------------------------------------------------------------------------
void SceneManager::removeAllObjects()
{
	AutoCheckInvariant aci( this );

	_lastMouseEventObj.clearObj();

	if( !_grabMouseStack.empty() )
	{
		if( auto oldGrabMouseObj = _grabMouseStack.back().lock() )
		{
			SceneEvent ev;
			oldGrabMouseObj->ungrabMouseEvent( &ev );
			oldGrabMouseObj->_isInGrabMouseStack = false;
			_grabMouseStack.pop_back();
		}

		for( auto& o : _grabMouseStack )
			if( auto grabMouseObj = o.lock() )
				grabMouseObj->_isInGrabMouseStack = false;

		_grabMouseStack.clear();
	}

	_singlePickValueObjects.clear();
	_ranglePickValueObjects.clear();

	std::for_each( _objects.begin(), _objects.end(), [&] ( SceneObject* obj ) { obj->removedFromSceneManager( this ); } );
	_objects.clear();
}