// VObject.h
//
#ifndef VOBJECT_ONCE
#define VOBJECT_ONCE

#include "s2SceneGraph_API.h"

#include "math/Box.h"
#include "graphics/Color.h"
#include "renderer/Surface.h"
#include "patterns/Observer.h"

#include "VObjectStyle.h"

#include <string>
#include <vector>
#include <memory>
#include <variant>


namespace s2 {
namespace SceneGraph {

class VObjectManager;

// ********************************************************
class VObject;
typedef std::shared_ptr<VObject> VObjectPtr;
// ********************************************************

class S2SCENEGRAPH_API VObject : public Observable, public std::enable_shared_from_this<VObject>
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
	virtual std::vector<Math::dvec3> points()       const { return {}; }
	virtual void                     draw( const Renderer::SurfacePtr &surface, const Renderer::DrawingState &ds ) const;
	
	virtual void       set( const VObjectPtr &o ) = 0;	
	virtual VObjectPtr clone()   const = 0;

protected:
	struct VObjectBuffer
	{
		const int32_t PrimitiveRestartIndex = 0xffff;
		
		std::vector<Math::dvec3> vertices;
		std::vector<Math::vec3>  normals;
		std::vector<Color>       colors;
		std::vector<Math::dvec2> textureCoords;
		std::vector<int32_t>     indices;

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

}}
#endif