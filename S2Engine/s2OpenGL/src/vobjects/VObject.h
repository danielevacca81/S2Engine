// VObject.h
//
#ifndef VOBJECT_ONCE
#define VOBJECT_ONCE

#include "CoreAPI.h"

#include "Math/Box.h"
#include "OpenGL/Color.h"

#include "VStyle.h"

#include <string>
#include <vector>

namespace OpenGL {
	class Renderer;
}

class VObjectManager;

class S2CORE_API VObject : public std::enable_shared_from_this<VObject>
{
public:
	enum ObjectType { Point, Line, PolyLine, Shape, Mesh, Group, Unknown };

protected:
	VObjectManager     *_manager;
	unsigned int       _id;
	std::string        _name;
	Math::box3 _boundingBox;
	//OpenGL::Color      _color;
	mutable VStyle     _style;
	static VStyle      _defaultStyle;

	bool               _selected;
	bool               _hilighted;
	bool               _visible;
	bool               _styled;
	bool               _selectionEnabled;

	//static OpenGL::Color  _selectionColor;
	//static OpenGL::Color  _hilightColor;

	friend class VObjectManager;
	void setManager( VObjectManager *mgr );
	virtual void select( bool on );
	virtual void hilight( bool on);

public:
	static void setDefaultStyle( const VStyle &style );
	//static void setSelectionColor( const OpenGL::Color &c );
	//static void setHilightColor( const OpenGL::Color &c );

	VObject();
	virtual ~VObject() {}

	virtual void setName( const std::string &name );
	virtual void setSelected( bool on );
	virtual void setHilighted( bool on );
	virtual void setStyle( const VStyle &s);
	virtual void setColor( const OpenGL::Color &c );
	virtual void enableSelection( bool );

	unsigned int id() const;
	std::string name() const;
	bool isSelected() const;
	bool isSelectionEnabled() const;
	bool isHilighted() const;
	bool isVisible() const;
	Math::box3 boundingBox() const;
	virtual Math::dvec3 center() const = 0;
	virtual std::vector<Math::dvec3> snapPoints() const = 0;

	OpenGL::Color color() const;
	VStyle &style() const;
	
	virtual ObjectType type() const ;
	virtual void draw( OpenGL::Renderer *r )const;
	virtual void draw( OpenGL::Renderer *r, const VStyle &style )const;
	virtual void drawForSelection( OpenGL::Renderer *r )const;
	virtual bool intersects( const Math::box3 &b )const;

	virtual std::vector<Math::dvec3> getPoints() const = 0;
	virtual VObject* clone() const = 0;

	void copyDataIn( VObject* newObj ) const;
};

typedef std::shared_ptr<VObject> VObjectPtr;

#endif