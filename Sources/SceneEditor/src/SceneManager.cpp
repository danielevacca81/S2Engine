// Scene.cpp
// 
#include "Scene.h"

#include "RenderCore/Surface.h"

using namespace RenderPipeline;

// ------------------------------------------------------------------------------------------------
void Scene::onDrawBegin( const View &view ) 
{
	AutoCheckInvariant aci( this );

	_view = view;

	assert( _surface );

	if( !_backgroundClickObject )
	{
		_backgroundClickObject = std::make_shared<ObjectClickable>();
		_backgroundClickObject->registerObserver( this );
		_backgroundClickObject->setName( "sceneManager_backgroundObj" );
		addObject( _backgroundClickObject );
	}
		
	_surface->setPickingEnabled( true );
	_surface->clearPickBuffer();
	//_itemAtMousePos.reset();
	_singlePickValueObjects.clear();
	_ranglePickValueObjects.clear();
}

// ------------------------------------------------------------------------------------------------
void SceneManager::onDrawEnd() 
{
	AutoCheckInvariant aci( this );

	_surface->setPickingEnabled( true );
	if( _lastMouseEventObj.mouseEvent.has_value() )
	{
		MouseEventObj prevMouseEventObj = _lastMouseEventObj;
		MouseEventObj newMouseEventObj  = _lastMouseEventObj;
		newMouseEventObj.mouseEvent->scenePos = _view.worldPointF( Math::dvec2(newMouseEventObj.mouseEvent->sceneViewportPos) + Math::dvec2(0.5) );
		pickItemAtPos( newMouseEventObj.mouseEvent->sceneViewportPos,
					   newMouseEventObj.object, newMouseEventObj.mouseEvent->pickValue, newMouseEventObj.mouseEvent->pickPart );

		generateAuxiliaryEvents( prevMouseEventObj, newMouseEventObj );
	}
}

// ------------------------------------------------------------------------------------------------
Renderer::Picker::Value SceneManager::negotiatePickValues( const ObjectInteractionPtr &object, 
														   const Renderer::Picker::Value valuesCount /*= 1*/ )
{
	AutoCheckInvariant aci( this );

	assert( _objects.find(object.get()) != _objects.end() );

	Renderer::Picker::Value retval;
	if( valuesCount == 0 )
		retval = Renderer::Picker::kInvalidValueBegin;
	else if( valuesCount == 1 )
	{
		retval = Renderer::Picker::Value( _singlePickValueObjects.size() );
		_singlePickValueObjects.push_back( object );
	}
	else 
	{
		retval = _ranglePickValueObjects.empty() ? _kRangePickValueBegin : _ranglePickValueObjects.back().rangeEnd;
		_ranglePickValueObjects.push_back( ObjectPickRange{retval, retval+valuesCount, object } );
	}

	return retval;
}

// ------------------------------------------------------------------------------------------------
inline void SceneManager::pickItemAtPos( const Math::ivec2 &viewportCoord, 
										 ObjectInteractionWeakPtr &obj, Renderer::Picker::Value &pickValue, int &pickPart )
{
	obj.reset();
	pickValue = Renderer::Picker::kInvalidValueBegin;
	pickPart  = 0;
	
	if( !_surface  )
		return;

	pickValue = _surface->pickValueAt( viewportCoord.x, viewportCoord.y );

	//std::cout << "pickItemAtPos " << pickValue << "\n";

	if( pickValue == Renderer::Picker::kClearValue )
		obj = _backgroundClickObject;
	else if( pickValue >= Renderer::Picker::kInvalidValueBegin )
		return;
	else if( pickValue < _kRangePickValueBegin )
	{
		if( pickValue >= _singlePickValueObjects.size() )
        { /*assert( 0 );*/ return; }

		obj = _singlePickValueObjects[ pickValue ];
	}
	else
	{
		if( _ranglePickValueObjects.empty() )  
		{ /*assert( 0 );*/ return; }

		auto it = std::upper_bound( _ranglePickValueObjects.begin(), _ranglePickValueObjects.end(), pickValue, 
									[]( const Renderer::Picker::Value &a, const ObjectPickRange &b ) { return a < b.rangeBegin; } );

		if( it == _ranglePickValueObjects.begin() ) 
		{ /*assert( 0 );*/ return; }

		it--;

		if( pickValue < it->rangeBegin || pickValue >=  it->rangeEnd ) 
		{ /*assert( 0 );*/ return; }

		obj = it->object;
		pickPart = pickValue - it->rangeBegin;
	}

	//assert( retval.first ); // se e' nullo stai cercando di fare picking di un oggetto rimosso dalla scena
}

// ------------------------------------------------------------------------------------------------
inline void SceneManager::resetObjectMatchingPickValue( const Renderer::Picker::Value value )
{
	if( value >= Renderer::Picker::kInvalidValueBegin )
		return;
	if( value < _kRangePickValueBegin )
	{
		if( value >= _singlePickValueObjects.size() )
        { /*assert( 0 );*/ return; }
		_singlePickValueObjects[ value ].reset();
	}
	else
	{
		if( _ranglePickValueObjects.empty() )
		{ /*assert( 0 );*/ return; }

		auto it = std::upper_bound( _ranglePickValueObjects.begin(), _ranglePickValueObjects.end(), value, 
									[]( const Renderer::Picker::Value &a, const ObjectPickRange &b ) { return a < b.rangeBegin; } );
		if( it == _ranglePickValueObjects.begin() ) 
		{ /*assert( 0 );*/ return; }

		it--;

		if( value < it->rangeBegin || value >=  it->rangeEnd ) 
		{ /*assert( 0 );*/ return; }

		it->object.reset();
	}
}

// ------------------------------------------------------------------------------------------------
bool SceneManager::equivalentMouseEvent( const std::optional<SceneMouseEvent> &a, const std::optional<SceneMouseEvent> &b )
{
	if( a.has_value() != b.has_value() )
		return false;
	if( !a.has_value() && !b.has_value() )
		return true;

	return	a->action           == b->action           &&
			a->button           == b->button           &&
			a->pressedButtons   == b->pressedButtons   &&
			a->modifiers        == b->modifiers        &&
			a->sceneViewportPos == b->sceneViewportPos &&
			a->scenePos         == b->scenePos         &&
			a->pickPart         == b->pickPart         ;
}

// ------------------------------------------------------------------------------------------------
void SceneManager::generateAuxiliaryEvents( MouseEventObj &oldMouseEventObj, MouseEventObj &newMouseEventObj )
{
	auto oldObj = oldMouseEventObj.object.lock();
	auto newObj = newMouseEventObj.object.lock();

	if( oldObj == newObj )
	{
		if( equivalentMouseEvent( oldMouseEventObj.mouseEvent, newMouseEventObj.mouseEvent ) )
			return;

		_lastMouseEventObj = newMouseEventObj;

		if( newMouseEventObj.mouseEvent.has_value() )
			sendMouseEvent( newMouseEventObj, &ObjectInteraction::mouseMoveEvent, "autogen_mouseMoveEvent" );
	}
	else
	{
		if( oldObj )
		{
			SceneEvent ev;
			logEvent( oldObj, ev, "leaveEvent" );
			oldObj->leaveEvent( &ev );
		}

		if( newObj )
		{
			SceneEvent ev;
			logEvent( newObj, ev, "enterEvent" );
			newObj->enterEvent( &ev );
		}

		_lastMouseEventObj = newMouseEventObj;

		if( newMouseEventObj.mouseEvent.has_value() )
			sendMouseEvent( newMouseEventObj, &ObjectInteraction::mouseMoveEvent, "autogen_mouseMoveEvent" );
	}
}

// ------------------------------------------------------------------------------------------------
template< class T > void SceneManager::sendMouseEvent( MouseEventObj &mouseEventObj, T memFunc, const char *dbgLogStr )
{
	assert( mouseEventObj.mouseEvent.has_value() );

	_lastMouseEventObj = mouseEventObj;

	// managing mouse grab
	const int grabMouseStackBackIdx = int(_grabMouseStack.size()) -1;
	for( int i= grabMouseStackBackIdx; i>= 0; --i )
		if( auto grabMouseObj = _grabMouseStack[i].lock() )
		{
			if( i != grabMouseStackBackIdx )
			{
				SceneEvent ev2( mouseEventObj.mouseEvent->timeMs );
				grabMouseObj->grabMouseEvent( &ev2 );
			}

			logEvent( grabMouseObj, mouseEventObj, dbgLogStr );
			(grabMouseObj.get()->*memFunc)( &mouseEventObj.mouseEvent.value() );
			return;
		}
		else
			_grabMouseStack.pop_back();

	if( auto objPtr = mouseEventObj.object.lock() )
	{
		logEvent( objPtr, mouseEventObj, dbgLogStr );
		(objPtr.get()->*memFunc)( &mouseEventObj.mouseEvent.value() );
	}
}

// ------------------------------------------------------------------------------------------------
bool SceneManager::pushToGrabMouseStack( const ObjectInteractionPtr &obj )
{
	if( !obj )
		return false;

	AutoCheckInvariant aci( this );

	assert( _objects.find( obj.get() ) != _objects.end() );

	if( !obj->isVisible() || !obj->isEnabled() || obj->_isInGrabMouseStack )
		return false;

	ObjectInteractionPtr theObject = obj;

	if( !_grabMouseStack.empty() )
		if( auto oldGrabMouseObj = _grabMouseStack.back().lock() )
		{ 
			SceneEvent ev;
			oldGrabMouseObj->ungrabMouseEvent( &ev );
		}
		else
		{
			assert( 0 );
			_grabMouseStack.pop_back();
		}

	_grabMouseStack.push_back( theObject );
	theObject->_isInGrabMouseStack = true;
	SceneEvent ev2;
	theObject->grabMouseEvent( &ev2 );
	return true;
}

// ------------------------------------------------------------------------------------------------
bool SceneManager::removeFromGrabMouseStack( const ObjectInteractionPtr &obj )
{
	AutoCheckInvariant aci( this );

	ObjectInteractionPtr theObject = obj;
	return removeFromGrabMouseStack( theObject.get() );
}

// ------------------------------------------------------------------------------------------------
bool SceneManager::removeFromGrabMouseStack( ObjectInteraction *obj )
{
	if( !obj )
		return false;

	AutoCheckInvariant aci( this );

	assert( _objects.find( obj ) != _objects.end() );

	if( !obj->_isInGrabMouseStack )
		return false;

	assert( _grabMouseStack.size() );

	if( auto oldGrabMouseObj = _grabMouseStack.back().lock() )
	{
		if( oldGrabMouseObj.get() == obj )
		{
			SceneEvent ev;
			obj->ungrabMouseEvent( &ev );
			obj->_isInGrabMouseStack = false;
			_grabMouseStack.pop_back();

			if( !_grabMouseStack.empty() )
			{
				auto newGrabMouseObj = _grabMouseStack.back().lock();
				assert( newGrabMouseObj );
				SceneEvent ev2;
				newGrabMouseObj->grabMouseEvent( &ev2 );
			}

			return true;
		}
	}
	else
	{
		assert( 0 );
		_grabMouseStack.pop_back();
	}	

	_grabMouseStack.erase( 
		std::find_if( _grabMouseStack.begin(), _grabMouseStack.end(), [obj]( auto &o ){
			return o.lock().get() == obj;
		} )
	);
	obj->_isInGrabMouseStack = false;
	return true;
}

// ------------------------------------------------------------------------------------------------
void SceneManager::notify( Observable *o, const std::any &msg )
{
	AutoCheckInvariant aci( this );

	if( msg.type() == typeid( ObjectClickable::MouseClickSignal ) && o == _backgroundClickObject.get() )
	{
		BackgroundClickSignal ev;
		ev.SceneMouseEvent::operator=( std::any_cast<ObjectClickable::MouseClickSignal>(msg) );
		notifyObservers( ev );
	}
}

// ------------------------------------------------------------------------------------------------
inline void SceneManager::checkInvariant() const
{
#ifdef _DEBUG
	_lastMouseEventObj.checkInvariant();
#endif
}