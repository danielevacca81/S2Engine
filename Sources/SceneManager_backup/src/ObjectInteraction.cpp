// ObjectInteraction.cpp
//
#include "ObjectInteraction.h"

#include "SceneManager.h"

using namespace Scene;

// ------------------------------------------------------------------------------------------------
ObjectInteraction::ObjectInteraction() 
: _isEnabled( true )
, _isInGrabMouseStack( false )
{
	resetNegotiatedPickValues();
	_pickerInteraction = SceneObject::PickerInteraction::Write;
}

// ------------------------------------------------------------------------------------------------
void ObjectInteraction::setupPickStuff( const Renderer::SurfacePtr &surface, Renderer::DrawingState &ds ) const
{
	resetNegotiatedPickValues();	

	surface->setPickingEnabled( _pickerInteraction == PickerInteraction::Write );
	if( _pickerInteraction == PickerInteraction::Write && isEnabled() )
		if( auto managerPtr = _manager.lock() )
		{
			_negotiatedPickValueCount = neededPickValueCount();
			_negotiatedPickValueOrigin = managerPtr->negotiatePickValues( 
				                                              const_cast<ObjectInteraction*>(this)->objectInteractionPtrFromThis(), 
															  _negotiatedPickValueCount );
		}

	ds.shaderProgram->uniform<int>( "u_pickValueOrigin" )->set( _negotiatedPickValueOrigin );
}

// ------------------------------------------------------------------------------------------------
void ObjectInteraction::setVisible( bool value ) 
{
	if( !value )
		ungrabMouse();

	SceneObject::setVisible( value ); 
}			

// ------------------------------------------------------------------------------------------------
void ObjectInteraction::setEnabled( bool enabled ) 
{
	if( !enabled )
		ungrabMouse();

	_isEnabled = enabled;
}			    

// ------------------------------------------------------------------------------------------------
bool ObjectInteraction::grabMouse()
{
	if( auto sceneManager = _manager.lock() )
		return sceneManager->pushToGrabMouseStack( objectInteractionPtrFromThis() );
	else
		return false;
}

// ------------------------------------------------------------------------------------------------
bool ObjectInteraction::ungrabMouse()
{
	if( auto sceneManager = _manager.lock() )
		return sceneManager->removeFromGrabMouseStack( objectInteractionPtrFromThis() );
	else
		return false;
}

// ------------------------------------------------------------------------------------------------
int ObjectInteraction::startDragDistance() const
{
	const auto sm = _manager.lock();
	return sm ? sm->startDragDistance() : 6 ;
}	

// ------------------------------------------------------------------------------------------------
/**
	avoid to override draw method directly. You can but it is better to override doDraw method instead
*/
void ObjectInteraction::draw( const Renderer::SurfacePtr &surface, const Renderer::DrawingState &ds ) const
{ 
	if( !_visible )
	{
		resetNegotiatedPickValues();	
		return;	
	}

	Renderer::DrawingState localDs = ds;
	setupPickStuff( surface, localDs );
	
	doDraw( surface, localDs );
}

// ------------------------------------------------------------------------------------------------
inline ObjectInteractionPtr  ObjectInteraction::objectInteractionPtrFromThis()
{
	return 	std::dynamic_pointer_cast<ObjectInteraction>( shared_from_this() );
}

// ------------------------------------------------------------------------------------------------
void ObjectInteraction::addedToSceneManager( std::weak_ptr<SceneManager> manager )
{ 
	SceneObject::addedToSceneManager( std::move(manager) );
	resetNegotiatedPickValues();
}

// ------------------------------------------------------------------------------------------------
void ObjectInteraction::removedFromSceneManager( const SceneManager *manager )
{ 
	SceneObject::removedFromSceneManager( manager );
	resetNegotiatedPickValues();
}

// ------------------------------------------------------------------------------------------------
void ObjectInteraction::resetNegotiatedPickValues() const
{ 
	_negotiatedPickValueOrigin = Renderer::Picker::kInvalidValueBegin;
	_negotiatedPickValueCount = 1u;
}