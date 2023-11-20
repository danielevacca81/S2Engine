// SceneObject.h
//
#ifndef SCENE_SCENEOBJECT_H
#define SCENE_SCENEOBJECT_H

#include "s2Scene_API.h"

#include "s2Renderer/Surface.h"
#include "s2Renderer/DrawingState.h"


#include "Core/Box.h"
#include "Core/Color.h"
#include "Core/Observer.h"




/*
// VObject.h
//
#ifndef VOBJECT_ONCE
#define VOBJECT_ONCE

#include "s2Scene_API.h"

#include "Core/Box.h"
#include "Core/Color.h"
#include "Core/Observer.h"
#include "s2Renderer/Surface.h"

#include "VObjectStyle.h"

#include <string>
#include <vector>
#include <memory>
#include <variant>


namespace Scene {

class VObjectManager;

// ********************************************************
class VObject;
typedef std::shared_ptr<VObject> VObjectPtr;
// ********************************************************

class S2SCENE_API VObject : public Observable, public std::enable_shared_from_this<VObject>
{
public:
	enum ObjectType { Point, Polyline, Polygon, Mesh, Group, Unknown };
	
	typedef std::variant<std::vector<int>, float, double, int, std::string> ObjectData;

public:
	static void setDefaultStyle( const VObjectStyle &style );

public:
	VObject();
	virtual ~VObject() {}

	virtual void setName( const std::string &name );
	virtual void setSelected( bool on );
	virtual void setHilighted( bool on );
	virtual void setStyle( const VObjectStyle &s);
	virtual void setMatrix( const Math::dmat4 &matrix );
	virtual void setUserData( const ObjectData &data );
	virtual void setSelectable( bool selectable );

	uint64_t                      id()           const;
	std::string                   name()         const;
	bool                          isSelected()   const;
	bool                          isSelectable() const;
	bool                          isHilighted()  const;
	bool                          isVisible()    const;
	ObjectData                    userData()     const;
	Color                         color()        const;
	VObjectStyle                  &style()       const;
	//std::weak_ptr<VObjectManager> manager()      const;
	bool                          hasStyle()     const;	

	virtual ObjectType               type()    const;
	virtual Math::dmat4              matrix()  const;
	virtual Math::box3               boundingBox()  const;
	virtual bool                     intersects( const Math::box3 &b ) const;
	virtual std::vector<Math::dvec3> vertices()       const { return {}; }
	virtual void                     draw( const Renderer::SurfacePtr &surface, const Renderer::DrawingState &ds ) const;

protected:
	struct VObjectBuffer
	{
		const int32_t PrimitiveRestartIndex = 0xffff;
		
		std::vector<Math::dvec3> vertices;
		std::vector<Math::vec3>  normals;
		std::vector<Color>       colors;
		std::vector<Math::dvec2> textureCoords;
		std::vector<uint32_t>    indices;

		void operator+=( const VObjectBuffer &other );
	};

	virtual VObjectBuffer toBuffer() const = 0;

	//virtual void drawForSelection( OpenGL::Renderer *r )const;
	//static OpenGL::Color  _selectionColor;
	//static OpenGL::Color  _hilightColor;

protected:
	uint64_t                      _id;
	std::weak_ptr<VObject>        _parent;
	//std::weak_ptr<VObjectManager> _manager; //needed?

	// attributes
	ObjectData            _userData;
	std::string           _name;
	Math::box3            _boundingBox;
	Math::dmat4           _matrix;
	mutable VObjectStyle  _style;
	static VObjectStyle   _defaultStyle;

	// state
	bool _selected;
	bool _hilighted;
	bool _visible;
	bool _styled;
	bool _selectable;	
	
	friend class VObjectManager;
	friend class VGroup;
};

}
#endif


*/




namespace Scene 
{

class SceneManager;

class SceneObject;
typedef std::shared_ptr<SceneObject> SceneObjectPtr;

class S2SCENE_API SceneObject : public Observable, public std::enable_shared_from_this<SceneObject>
{
public:
	typedef std::any UserData;

	struct DestroyedSignal{};
							 
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
	//VObjectStyle          &style()               const;
	//bool                   hasStyle()            const;	
	PickerInteraction        pickerInteraction()   const;
						    
	virtual Math::dmat4      matrix()                          const;
	virtual Math::box3       boundingBox()                     const;
	virtual bool             intersects( const Math::box3 &b ) const;
						    
	virtual void			 draw( const Renderer::SurfacePtr &surface, const Renderer::DrawingState &ds ) const;

protected:
	void                     setupPickStuff( const Renderer::SurfacePtr &surface, Renderer::DrawingState &ds ) const;
	virtual void			 doDraw( const Renderer::SurfacePtr &surface, Renderer::DrawingState &ds ) const {};
	void                     callDestroyedSignal();
							 
	friend class SceneManager;
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
	Math::box3                  _boundingBox;
	//mutable VObjectStyle      _style;
	//static VObjectStyle       _defaultStyle;
							    
	// state				    
	bool                        _visible;
	bool                        _styled;
	PickerInteraction			_pickerInteraction;	
	bool                        _notifyDestroyedNeeded;
};

}

#endif
