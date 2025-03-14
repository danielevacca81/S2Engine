// ObjectInteraction.h
//
#ifndef SCENE_OBJECTINTERACTION_H
#define SCENE_OBJECTINTERACTION_H

#include "RenderEngine_API.h"

#include "SceneObject.h"
#include "SceneEvent.h"

#include "s2Renderer/Picker.h"

#include <memory>

namespace Scene 
{

class SceneManager;

class ObjectInteraction;
typedef std::shared_ptr<ObjectInteraction> ObjectInteractionPtr;

class RENDERENGINE_API ObjectInteraction : public SceneObject
{
public:
	ObjectInteraction();
	~ObjectInteraction() { callDestroyedSignal(); }

	void					 setVisible( bool value )   override;		    
	virtual void			 setEnabled( bool enabled );		    
	virtual bool			 isEnabled() const          { return _isEnabled; }

	virtual bool             grabMouse();
	virtual bool             ungrabMouse();
	bool					 grabMouseRequested() const { return _isInGrabMouseStack; }
							 
	virtual int              startDragDistance() const;					 
							 
	virtual void			 draw( const Renderer::SurfacePtr &surface, const Renderer::DrawingState &ds ) const;

protected:
	friend class SceneManager;

	ObjectInteractionPtr     objectInteractionPtrFromThis();

	virtual void			 mousePressEvent  ( SceneMouseEvent *event ) {}
	virtual void			 mouseReleaseEvent( SceneMouseEvent *event ) {}
	virtual void			 mouseMoveEvent   ( SceneMouseEvent *event ) {}
	virtual void			 enterEvent       ( SceneEvent *event )      {}
	virtual void			 leaveEvent       ( SceneEvent *event )      {}
	virtual void			 grabMouseEvent   ( SceneEvent *event )      {}
	virtual void			 ungrabMouseEvent ( SceneEvent *event )      {}

							 
	void                     setupPickStuff( const Renderer::SurfacePtr &surface, Renderer::DrawingState &ds ) const;
	virtual void			 doDraw( const Renderer::SurfacePtr &surface, Renderer::DrawingState &ds ) const {}; // avoid to override draw method directly. You can but it is better to override doDraw method instead
							 
	virtual void			 addedToSceneManager( std::weak_ptr<SceneManager> manager );
	virtual void			 removedFromSceneManager( const SceneManager *manager );

	virtual Renderer::Picker::Value	neededPickValueCount() const { return 1; }
	Renderer::Picker::Value  negotiatedPickValueOrigin() const { return _negotiatedPickValueOrigin; }
	void                     resetNegotiatedPickValues() const;

protected:
	bool					         _isEnabled;
	bool                             _isInGrabMouseStack; // modified by SceneManager only

	mutable Renderer::Picker::Value  _negotiatedPickValueOrigin;
	mutable Renderer::Picker::Value  _negotiatedPickValueCount;
};

}

#endif




