// SceneObject.h
//
#ifndef SCENE_SCENEOBJECT_H
#define SCENE_SCENEOBJECT_H

#include "RenderPipeline_API.h"

#include "RenderCore/Surface.h"
#include "RenderCore/DrawState.h"

#include "Math/Box.h"

#include "sigslot/signal.hpp"


#include <any>

namespace SceneManager {

class SceneManager;

class SceneObject;
typedef std::shared_ptr<SceneObject> SceneObjectPtr;

class RENDERPIPELINE_API SceneObject : public std::enable_shared_from_this<SceneObject>
{
public:
	typedef std::any UserData;

	sigslot::signal<> objectDestroyedEvent;
							 
	enum class PickerInteraction{ NotModify, Write }; // SceneObject cannot be picked but can occlude other items. Use ObjectInteraction to take feedback from picker

public:
	SceneObject( const SceneObjectPtr &parent = nullptr );
	virtual ~SceneObject();

	const std::weak_ptr<SceneManager> &manager() const { return _manager; }

	virtual void             setName               ( const std::string &name );
	virtual void             setVisible            ( bool visible );
	virtual void             setUserData           ( const UserData &data );
	virtual void             setPickerInteraction  ( const PickerInteraction value );
	virtual void             setMatrix             ( const Math::dmat4 &matrix );
						    
	uint32_t                 id()                  const { return _id; }
	std::string              name()                const;
	bool                     isVisible()           const;
	UserData                 userData()            const;
	PickerInteraction        pickerInteraction()   const;
						    
	virtual Math::dmat4      matrix()                          const;
	virtual Math::dbox3      boundingBox()                     const;
	virtual bool             intersects( const Math::dbox3 &b ) const;
						    
	virtual void			 draw( RenderCore::Surface &surface, const RenderCore::DrawState &ds ) const;

protected:
	void                     prepareForPicking( RenderCore::Surface &surface, RenderCore::DrawState &ds ) const;
	virtual void			 doDraw( RenderCore::Surface &surface, RenderCore::DrawState &ds ) const {};
	void                     callDestroyedSignal();
							 
	virtual void			 addedToSceneManager( std::weak_ptr<SceneManager> manager );
	virtual void			 removedFromSceneManager( const SceneManager *manager );

protected:
	uint32_t					 _id;
	std::weak_ptr<SceneManager>  _manager;
	std::weak_ptr<SceneObject>   _parent;

	// attributes			    
	std::string                 _name;
	UserData                    _userData;
	Math::dmat4                 _matrix;
	Math::dbox3                 _boundingBox;
	//mutable VObjectStyle      _style;
	//static VObjectStyle       _defaultStyle;
							    
	// state				    
	bool                        _visible;
	bool                        _styled;
	PickerInteraction			_pickerInteraction;	
	bool                        _notifyDestroyedNeeded;
	
	friend class SceneManager;
};

}

#endif
