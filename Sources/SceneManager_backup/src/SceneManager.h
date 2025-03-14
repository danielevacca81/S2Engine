// SceneManager.h
//
#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "RenderEngine_API.h"

#include "SceneObject.h"
#include "ObjectInteraction.h"
#include "ObjectClickable.h"
#include "View.h"

#include "RenderSystem/Picker.h"

#include <map>
#include <set>
#include <vector>
#include <tuple>
#include <limits>
#include <memory>
#include <optional>

namespace RenderEngine {

class SceneManager;
typedef std::shared_ptr<SceneManager> SceneManagerPtr;

class RENDERENGINE_API SceneManager : public Observer, public Observable, public std::enable_shared_from_this<SceneManager>
{
public:
	struct BackgroundClickSignal : public SceneMouseEvent { };

public:
	static SceneManagerPtr New() { 
		struct MakeSharedEnabler : public SceneManager {};
		return std::make_shared<MakeSharedEnabler>(); 
	}

private:
	SceneManager();

public:
	SceneManager( const SceneManager & ) = delete;
	SceneManager( SceneManager && )      = delete;
	virtual ~SceneManager();

	SceneManager            &operator=( const SceneManager & ) = delete;
	SceneManager            &operator=( SceneManager && )      = delete;
				            

	virtual bool             addObject   ( const SceneObjectPtr &obj );
	virtual void             removeObject( const SceneObjectPtr &obj );
	virtual void             removeObject( SceneObject    *obj );
	virtual void             removeAllObjects();
				           
	ObjectInteractionPtr     objectAtMousePos()              const { return _lastMouseEventObj.object.lock(); }
	const std::optional<SceneMouseEvent> &lastMouseEvent()   const { return _lastMouseEventObj.mouseEvent; }

	virtual void             setSelectionLock( bool enabled ) { _selectionLock = enabled; } 	
	virtual void             setSelectionMouseButton( const SceneMouseEvent::Button button,  
													  const SceneMouseEvent::KeyboardModifier modifiers );
	virtual void             setStartDragDistance( const int dist ) { _startDragDistance = dist; }

	bool                     selectionLock() const            { return _selectionLock; }
	void                     selectionMouseButton( SceneMouseEvent::Button &button,
												   SceneMouseEvent::KeyboardModifier &mkodifiers ) const;
	int                      startDragDistance() const { return _startDragDistance; }

				           
	virtual void             mousePressEvent  ( SceneMouseEvent *event );
	virtual void             mouseReleaseEvent( SceneMouseEvent *event );
	virtual void             mouseMoveEvent   ( SceneMouseEvent *event );
	virtual void             enterEvent       ( SceneEvent      *event );
	virtual void             leaveEvent       ( SceneEvent      *event );
	//virtual void			 grabMouseEvent   ( SceneEvent      *event );
	//virtual void			 ungrabMouseEvent ( SceneEvent      *event );

				           
	virtual void             setSurface( const RenderSystem::Surface &surface );
	virtual void             onDrawBegin( const RenderSystem::View &view ); // tmp eventi di notifica. il disegno andrebbe fatto qui dentro
	virtual void             onDrawEnd();   // tmp eventi di notifica. il disegno andrebbe fatto qui dentro
	RenderSystem::Picker::Value  negotiatePickValues( const ObjectInteractionPtr &object, const RenderSystem::Picker::Value = 1 );

protected:
	template< class T > void mouseEventHelper( SceneMouseEvent *event, T memFunc, const char *dbgLogStr );
	template< class T > void sendMouseEvent( MouseEventObj &mouseEventObj, T memFunc, const char *dbgLogStr );

	void                     pickItemAtPos( const Math::ivec2 &viewportCoord, 
											ObjectInteractionWeakPtr &obj, Renderer::Picker::Value &pickValue, int &pickPart );
	void                     resetObjectMatchingPickValue( const Renderer::Picker::Value value );
	static bool              equivalentMouseEvent( const std::optional<SceneMouseEvent> &a, const std::optional<SceneMouseEvent> &b );
	void                     generateAuxiliaryEvents( MouseEventObj &oldMouseEventObj, MouseEventObj &newMouseEventObj );

	void                     notify( Observable *o, const std::any &message ) override;

	friend bool ObjectInteraction::grabMouse();
	friend bool ObjectInteraction::ungrabMouse();
	bool				     pushToGrabMouseStack( const ObjectInteractionPtr &obj );
	bool                     removeFromGrabMouseStack( const ObjectInteractionPtr &obj );
	bool                     removeFromGrabMouseStack( ObjectInteraction *obj );

	void                     checkInvariant() const;
	static void              logEvent( std::shared_ptr<ObjectInteraction> &o, const SceneEvent    &ev, const char *str );
	static void              logEvent( std::shared_ptr<ObjectInteraction> &o, const MouseEventObj &ev, const char *str );

protected:
	static constexpr Renderer::Picker::Value _kRangePickValueBegin = Renderer::Picker::kInvalidValueBegin / 3;

private:
	using SceneObjectWeakPtr = std::weak_ptr<SceneObject>;
	using ObjectInteractionWeakPtr = std::weak_ptr<ObjectInteraction>;
	using ObjectSet = std::set< SceneObject* >;

	struct ObjectPickRange
	{
		RenderSystem::Picker::Value	 rangeBegin;
		RenderSystem::Picker::Value	 rangeEnd;
		ObjectInteractionWeakPtr object;
	};

	struct MouseEventObj
	{
		std::optional<SceneMouseEvent>	mouseEvent;
		ObjectInteractionWeakPtr        object;

		void                    reset() { *this = MouseEventObj(); }
		void                    clearObj();
		RenderSystem::Picker::Value pickValue() const;
		int                     pickPart() const;
		//bool                  operator==( const MouseEventObj &o ) const;
		void                    checkInvariant() const;
	};

	struct AutoCheckInvariant;

protected:
	ObjectSet							   _objects;	
							     
	bool								   _selectionLock;
	SceneMouseEvent::Button				   _selectionMouseButton;
	SceneMouseEvent::KeyboardModifier      _selectionKeyboardModifiers;
	int									   _startDragDistance;
						
	View                                   _view;
	Renderer::SurfacePtr				   _surface; // fare double buffering per picking buffer? un buffer da cui leggere e uno su cui scrivere?
							     
	MouseEventObj                          _lastMouseEventObj;
	std::vector<ObjectInteractionWeakPtr>  _grabMouseStack;

	ObjectClickablePtr                     _backgroundClickObject;
	std::vector<ObjectInteractionWeakPtr>  _singlePickValueObjects;
	std::vector<ObjectPickRange>		   _ranglePickValueObjects;
};

}
#endif