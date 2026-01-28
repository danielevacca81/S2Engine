// VGroup.h
//
#ifndef VGROUP_ONCE
#define VGROUP_ONCE

#include "s2SceneGraph_API.h"

#include "VObject.h"

#include "Math/Math.h"

#include <map>
#include <vector>

namespace s2 {
namespace SceneGraph {


class S2SCENEGRAPH_API VGroup : public VObject
{
private:
	std::map<unsigned int, VObjectPtr>        _objects;

	virtual void select( bool on );
	virtual void hilight( bool on);

public:
	VGroup();
	~VGroup();

	void setName( const std::string &name );
	void setSelected( bool on );
	void setHilighted( bool on );
	void setStyle( const VObjectStyle &s);
	void setColor( const OpenGL::Color &c );
	void setSelectable( bool );


	void                     addObject( const VObjectPtr &obj );
	std::map<unsigned int, VObjectPtr>  &objects();
	std::vector<Math::dvec3> snapPoints() const;
	Math::dvec3              center() const;

	void draw( OpenGL::Renderer *r ) const;
	void drawForSelection( OpenGL::Renderer *r ) const;
	bool intersects( const Math::box3 &b ) const;

	std::vector<Math::dvec3> getPoints() const;
	VObject* clone() const;

	ObjectType type() const   { return Group;}
};


}}
#endif