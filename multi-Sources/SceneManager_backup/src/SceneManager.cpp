// SceneManager.cpp
// 
#include "SceneManager.h"

#include "RenderSystem/Surface.h"

#include <iostream>

using namespace Scene;

// ------------------------------------------------------------------------------------------------
//#define DEBUG_LOGEVENT

#if defined( DEBUG_LOGEVENT )
static int gDebugLogCount = 0;
#endif



// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
inline void SceneManager::MouseEventObj::clearObj()
{
	object.reset();
	//if( mouseEvent.has_value() )
	//	mouseEvent->pickValue = Renderer::Picker::kInvalidValueBegin;
}

// ------------------------------------------------------------------------------------------------
inline Renderer::Picker::Value SceneManager::MouseEventObj::pickValue() const
{
	return mouseEvent.has_value() ? mouseEvent->pickValue : Renderer::Picker::kInvalidValueBegin;
}

// ------------------------------------------------------------------------------------------------
inline Renderer::Picker::Value SceneManager::MouseEventObj::pickPart() const
{
	return mouseEvent.has_value() ? mouseEvent->pickPart : -1;
}

// ------------------------------------------------------------------------------------------------
inline void SceneManager::MouseEventObj::checkInvariant() const
{
	if( !mouseEvent.has_value() )
		assert( object.lock() == nullptr );

	if( object.lock() )
		assert( mouseEvent.has_value() );
}



// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
struct SceneManager::AutoCheckInvariant
{
#ifdef _DEBUG
	const SceneManager *objToCheck = nullptr;

	AutoCheckInvariant( const SceneManager *o ) : objToCheck( o ) {}
	~AutoCheckInvariant() { if(objToCheck) objToCheck->checkInvariant(); }
#else
	AutoCheckInvariant( const SceneManager * ) {}
#endif
};



// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
SceneManager::SceneManager()
	: _selectionLock( false )
	, _selectionMouseButton( SceneMouseEvent::ButtonLeft )
	, _selectionKeyboardModifiers( SceneMouseEvent::ModifierNone )
	, _startDragDistance( 6 )
{
	AutoCheckInvariant aci( this );
}

// ------------------------------------------------------------------------------------------------
SceneManager::~SceneManager()
{
	removeAllObjects();
}

// ------------------------------------------------------------------------------------------------
bool SceneManager::addObject( const SceneObjectPtr &obj )
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
void SceneManager::removeObject( const SceneObjectPtr &obj )
{
	auto objSharedPtr = obj;
	removeObject( objSharedPtr.get() );
}

// ------------------------------------------------------------------------------------------------
void SceneManager::removeObject( SceneObject *obj )
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

		for( auto &o: _grabMouseStack )
			if( auto grabMouseObj = o.lock() )
				grabMouseObj->_isInGrabMouseStack = false;

		_grabMouseStack.clear();
	}

	_singlePickValueObjects.clear();
	_ranglePickValueObjects.clear();

	std::for_each( _objects.begin(), _objects.end(), [&]( SceneObject* obj ){ obj->removedFromSceneManager( this ); } );
	_objects.clear();
}

// ------------------------------------------------------------------------------------------------
void  SceneManager::setSelectionMouseButton( const SceneMouseEvent::Button button,
											 const SceneMouseEvent::KeyboardModifier modifiers )
{
	AutoCheckInvariant aci( this );

	_selectionMouseButton = button;
	_selectionKeyboardModifiers = modifiers;
}

// ------------------------------------------------------------------------------------------------
void  SceneManager::selectionMouseButton( SceneMouseEvent::Button &button,
										  SceneMouseEvent::KeyboardModifier &mkodifiers ) const
{
	AutoCheckInvariant aci( this );

	button = _selectionMouseButton;
	mkodifiers = _selectionKeyboardModifiers;
}

// ------------------------------------------------------------------------------------------------
void SceneManager::mousePressEvent( SceneMouseEvent *event )
{
	mouseEventHelper( event, &ObjectInteraction::mousePressEvent, "mousePressEvent" );
}

// ------------------------------------------------------------------------------------------------
void SceneManager::mouseReleaseEvent( SceneMouseEvent *event )
{
	mouseEventHelper( event, &ObjectInteraction::mouseReleaseEvent, "mouseReleaseEvent" );
}

// ------------------------------------------------------------------------------------------------
void SceneManager::mouseMoveEvent( SceneMouseEvent *event )
{
	mouseEventHelper( event, &ObjectInteraction::mouseMoveEvent, "mouseMoveEvent" );
}

// ------------------------------------------------------------------------------------------------
template< class T > void SceneManager::mouseEventHelper( SceneMouseEvent *event, T memFunc, const char *dbgLogStr )
{
	AutoCheckInvariant aci( this );

	MouseEventObj newMouseEventObj;
	newMouseEventObj.mouseEvent = *event;
	newMouseEventObj.mouseEvent->scenePos  = _view.worldPointF(  Math::dvec2(newMouseEventObj.mouseEvent->sceneViewportPos) + Math::dvec2(0.5) );

	//updateItemAtMousePos(); TODO sistemare con double buffering
	newMouseEventObj.mouseEvent->pickValue = _lastMouseEventObj.pickValue();
	newMouseEventObj.mouseEvent->pickPart  = _lastMouseEventObj.pickPart();
	newMouseEventObj.object                = _lastMouseEventObj.object;


	//generateAuxiliaryEvents( _lastMouseEventObj, newMouseEventObj );
	sendMouseEvent( newMouseEventObj, memFunc, dbgLogStr );

	*event = *newMouseEventObj.mouseEvent;
}

// ------------------------------------------------------------------------------------------------
void SceneManager::enterEvent( SceneEvent *event )
{
	AutoCheckInvariant aci( this );

	_lastMouseEventObj.reset();
}

// ------------------------------------------------------------------------------------------------
void SceneManager::leaveEvent( SceneEvent *event )
{
	AutoCheckInvariant aci( this );

	MouseEventObj prevMouseEventObj = _lastMouseEventObj;
	MouseEventObj newMouseEventObj;
	generateAuxiliaryEvents( prevMouseEventObj, newMouseEventObj ); 
}

// ------------------------------------------------------------------------------------------------
void SceneManager::setSurface( const Renderer::SurfacePtr &surface )
{
	_surface = surface;
}

// ------------------------------------------------------------------------------------------------
void SceneManager::onDrawBegin( const View &view ) 
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

// ------------------------------------------------------------------------------------------------
void SceneManager::logEvent( std::shared_ptr<ObjectInteraction> &o, const SceneEvent &ev, const char *str )
{
#if defined( DEBUG_LOGEVENT )
	++gDebugLogCount;

	std::cout << gDebugLogCount << " obj:";
	if( !o )
		std::cout << "nullptr";
	else if( o->name().empty() )
		std::cout << o.get();
	else
		std::cout << o->name();
	std::cout << "   ";

	std::cout << str;

	std::cout << "\n";
#endif
}

// ------------------------------------------------------------------------------------------------
void SceneManager::logEvent( std::shared_ptr<ObjectInteraction> &o, const MouseEventObj &evObj, const char *str )
{
#if defined( DEBUG_LOGEVENT )
	++gDebugLogCount;
	static constexpr Renderer::SurfacePicker::Value _kRangePickValueBegin = Renderer::SurfacePicker::kInvalidValueBegin / 3;

	std::cout << gDebugLogCount << " obj:";
	if( !o )
		std::cout << "nullptr";
	else if( o->name().empty() )
		std::cout << o.get();
	else
		std::cout << o->name();
	std::cout << "   ";

	std::cout << str << " ";

	std::cout	  << "ev[ " 
				     "pickObj:";

	const auto obj = evObj.object.lock();
	if( !obj )
		std::cout << "nullptr";
	else if( obj->name().empty() )
		std::cout << obj.get();
	else
		std::cout << obj->name();

	const auto &ev = *evObj.mouseEvent;
	std::cout << " pickValue:";
	if( ev.pickValue == Renderer::SurfacePicker::kClearValue )
		std::cout << "kClearValue";
	else if( ev.pickValue >= Renderer::SurfacePicker::kInvalidValueBegin )
		std::cout << "kInvalidValueBegin+" << ev.pickValue-Renderer::SurfacePicker::kInvalidValueBegin;
	else if( ev.pickValue >= _kRangePickValueBegin )
		std::cout << "_kRangePickValueBegin+" << ev.pickValue-_kRangePickValueBegin;
	else 
		std::cout << ev.pickValue;
	
	std::cout     << " "
		             "pickPart:"  << ev.pickPart  << " "

				  << " "
		             "scenePos:"  << ev.scenePos.x  << "," << ev.scenePos.y  << "," << ev.scenePos.z  << " "
		             "sceneViewportPos:"  << ev.sceneViewportPos.x  << "," << ev.sceneViewportPos.y   << " "
		            "]";

	std::cout << "\n";
#endif
}