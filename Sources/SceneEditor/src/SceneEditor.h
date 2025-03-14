// SceneInteractive.h
//
#ifndef SCENEINTERACTIVE_H
#define SCENEINTERACTIVE_H

#include "RenderPipeline_API.h"

#include "SceneObject.h"
#include "SceneEvent.h"
#include "ObjectInteraction.h"
#include "ObjectClickable.h"
#include "View.h"

#include "RenderCore/PickerConstants.h"
#include "RenderCore/Surface.h"

#include <map>
#include <set>
#include <vector>
#include <tuple>
#include <limits>
#include <memory>
#include <optional>

namespace SceneManager {


// todo
struct SelectionSettings
{
	uint16_t dragDistance;
	uint16_t selectionMouseButton; 
	uint16_t selectionLock;
};

/*
SceneFactory::createScene(); // createscene from context with a valid surface inside

*/

class RENDERPIPELINE_API SceneIntractive
{
public:
	sigslot::signal<> signal_BackgroundClicked;
	//struct BackgroundClickSignal : public SceneMouseEvent { };

//public:
//	static SceneManagerPtr New() { 
//		struct MakeSharedEnabler : public SceneManager {};
//		return std::make_shared<MakeSharedEnabler>(); 
//	}
//private:
//	SceneManager();

public:
	ObjectInteractionPtr     objectAtMousePos()              const { return _lastMouseEventObj.object.lock(); }
	const std::optional<SceneMouseEvent> &lastMouseEvent()   const { return _lastMouseEventObj.mouseEvent; }
													  const SceneMouseEvent::KeyboardModifier modifiers );
	virtual void             onMousePressEvent  ( SceneMouseEvent *event );
	virtual void             onMouseReleaseEvent( SceneMouseEvent *event );
	virtual void             onMouseMoveEvent   ( SceneMouseEvent *event );
	virtual void             onEnterEvent       ( SceneEvent      *event );
	virtual void             onLeaveEvent       ( SceneEvent      *event );
	//virtual void			 grabMouseEvent   ( SceneEvent      *event );
	//virtual void			 ungrabMouseEvent ( SceneEvent      *event );

	RenderCore::PickerConstants::Value  negotiatePickValues( const ObjectInteractionPtr &object, const RenderCore::PickerConstants::Value = 1 );

protected:
	template< class T > void mouseEventHelper( SceneMouseEvent *event, T memFunc, const char *dbgLogStr );
	template< class T > void sendMouseEvent( MouseEventObj &mouseEventObj, T memFunc, const char *dbgLogStr );

	void                     pickItemAtPos( const Math::ivec2 &viewportCoord, 
											ObjectInteractionWeakPtr &obj, Renderer::Picker::Value &pickValue, int &pickPart );
	void                     resetObjectMatchingPickValue( const Renderer::Picker::Value value );
	static bool              equivalentMouseEvent( const std::optional<SceneMouseEvent> &a, const std::optional<SceneMouseEvent> &b );
	void                     generateAuxiliaryEvents( MouseEventObj &oldMouseEventObj, MouseEventObj &newMouseEventObj );

	//void                     notify( Observable *o, const std::any &message ) override;

	friend bool ObjectInteraction::grabMouse();
	friend bool ObjectInteraction::ungrabMouse();
	bool				     pushToGrabMouseStack( const ObjectInteractionPtr &obj );
	bool                     removeFromGrabMouseStack( const ObjectInteractionPtr &obj );
	bool                     removeFromGrabMouseStack( ObjectInteraction *obj );

	void                     checkInvariant() const;
protected:
	static constexpr RenderCore::PickerConstants::Value _kRangePickValueBegin = RenderCore::PickerConstants::kInvalidValueBegin / 3;

private:
	using SceneObjectWeakPtr = std::weak_ptr<SceneObject>;
	using ObjectInteractionWeakPtr = std::weak_ptr<ObjectInteraction>;

	struct ObjectPickRange
	{
		RenderCore::PickerConstants::Value	 rangeBegin;
		RenderCore::PickerConstants::Value	 rangeEnd;
		ObjectInteractionWeakPtr object;
	};

	struct MouseEventObj
	{
		std::optional<SceneMouseEvent>	mouseEvent;
		ObjectInteractionWeakPtr        object;

		void                    reset() { *this = MouseEventObj(); }
		void                    clearObj();
		RenderCore::PickerConstants::Value pickValue() const;
		int                     pickPart() const;
		//bool                  operator==( const MouseEventObj &o ) const;
		void                    checkInvariant() const;
	};

	struct AutoCheckInvariant;

protected:							     
	bool								   _selectionLock;
	SceneMouseEvent::Button				   _selectionMouseButton;
	SceneMouseEvent::KeyboardModifier      _selectionKeyboardModifiers;
	int									   _startDragDistance;
						
	View                                   _view;
	RenderCore::Surface			       _surface; // fare double buffering per picking buffer? un buffer da cui leggere e uno su cui scrivere?
							     
	MouseEventObj                          _lastMouseEventObj;
	std::vector<ObjectInteractionWeakPtr>  _grabMouseStack;

	ObjectClickablePtr                     _backgroundClickObject;
	std::vector<ObjectInteractionWeakPtr>  _singlePickValueObjects;
	std::vector<ObjectPickRange>		   _ranglePickValueObjects;
};

}
#endif