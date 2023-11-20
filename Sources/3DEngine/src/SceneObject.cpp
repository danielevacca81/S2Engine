// SceneObject.cpp
//
#include "SceneObject.h"

#include "SceneManager.h"

#include "s2Renderer/Picker.h"

#include <atomic>

using namespace Scene;

static std::atomic<std::uint32_t> IDPool = 0;

// ------------------------------------------------------------------------------------------------
SceneObject::SceneObject( const SceneObjectPtr &parent )
: _id( IDPool.fetch_add( 1 ) )
, _parent( parent )
, _visible ( true )
, _styled( false ) 
, _pickerInteraction( PickerInteraction::NotModify )
, _notifyDestroyedNeeded( true )
{}

// ------------------------------------------------------------------------------------------------
SceneObject::~SceneObject()
{
	callDestroyedSignal();
}

// ------------------------------------------------------------------------------------------------
void SceneObject::setName             ( const std::string &name )       { _name     = name;           }
void SceneObject::setVisible          ( bool visible )                  { _visible  = visible;        }
void SceneObject::setUserData         ( const UserData &data )          { _userData = data;           }
void SceneObject::setPickerInteraction( const PickerInteraction value ) { _pickerInteraction = value; }
void SceneObject::setMatrix           ( const Math::dmat4 &matrix )     { _matrix   = matrix;         }

// ------------------------------------------------------------------------------------------------
std::string                     SceneObject::name()                            const { return _name; }
bool                            SceneObject::isVisible()                       const { return _visible; }
SceneObject::UserData           SceneObject::userData()                        const { return _userData; }
SceneObject::PickerInteraction  SceneObject::pickerInteraction()               const { return _pickerInteraction; }
Math::dmat4                     SceneObject::matrix()                          const { return _matrix; }
Math::box3                      SceneObject::boundingBox()                     const { return _boundingBox; }
bool                            SceneObject::intersects( const Math::box3 &b ) const { return false; }

// ------------------------------------------------------------------------------------------------
void SceneObject::setupPickStuff( const Renderer::SurfacePtr &surface, Renderer::DrawingState &ds ) const
{
	surface->setPickingEnabled( _pickerInteraction == PickerInteraction::Write );
	if( _pickerInteraction == PickerInteraction::Write )
		ds.shaderProgram->uniform<int>( "u_pickValueOrigin" )->set( Renderer::Picker::kInvalidValueBegin );
}

// ------------------------------------------------------------------------------------------------
/**
	avoid to override draw method directly. You can but it is better to override doDraw method instead
*/
void SceneObject::draw( const Renderer::SurfacePtr &surface, const Renderer::DrawingState &ds ) const
{ 
	if( !_visible )
		return;

	Renderer::DrawingState localDs = ds;
	setupPickStuff( surface, localDs );

	doDraw( surface, localDs ); 
}

// ------------------------------------------------------------------------------------------------
/**
	call it in subclasses destructor
*/
void SceneObject::callDestroyedSignal()
{ 
	if( _notifyDestroyedNeeded )
	{
		_notifyDestroyedNeeded = false;

		notifyObservers( DestroyedSignal() ); 

		if( auto sceneManager = _manager.lock() )
			sceneManager->removeObject( this );
	}
}

// ------------------------------------------------------------------------------------------------
void SceneObject::addedToSceneManager( std::weak_ptr<SceneManager> manager )
{ 
	_manager = std::move( manager );
}

// ------------------------------------------------------------------------------------------------
void SceneObject::removedFromSceneManager( const SceneManager *manager )
{ 
	_manager.reset();
}
