// VObject.h
//
#ifndef VOBJECT_ONCE
#define VOBJECT_ONCE

#include "S2CAD_API.h"

#include "Common/Types.h"
#include "Common/Color.h"

#include "Math/Box.h"
#include "Design/Observer.h"

#include <string>
#include <vector>

namespace OpenGL {
	class Renderer;
}

namespace s2 
{

class VObjectManager;

DECLARE_PTR(VObject)

class S2CAD_API VObject : public IObservable, public std::enable_shared_from_this<VObject>
{
	friend class       VObjectManager;

public:
	enum ObjectType { Point, Line, PolyLine, Shape, Mesh, Group, Unknown };

	struct State 
	{
		bool Selected;
		bool Hilighted;
		bool Visible;
		bool Culled;
		bool Styled;
		bool Selectable;

		State()
		: Selected(false)
		, Hilighted(false)
		, Visible(true)
		, Culled(false)
		, Styled(false)
		, Selectable(true)
		{}
	};


	VObject();
	bool operator==( const VObjectPtr &other );
	virtual ~VObject();

	virtual void setName( const std::string &name );
	virtual void setState( const State &state );

	unsigned int                     id()          const;
	std::string                      name()        const;
	Math::box3                       boundingBox() const;

	virtual void                     draw( OpenGL::Renderer *r )       const;
	virtual bool                     intersects( const Math::box3 &b ) const;
	virtual Math::dvec3              center()                          const = 0;
	virtual std::vector<Math::dvec3> snapPoints()                      const = 0;
	virtual std::vector<Math::dvec3> points()                          const = 0;
	virtual VObjectPtr               clone()                           const = 0;
	virtual ObjectType               type()                            const;
	virtual State                    state()                           const { return _state; }


protected:
	unsigned int _id;

	std::string  _name;
	Math::box3   _boundingBox;
	State        _state;
};


}

#endif