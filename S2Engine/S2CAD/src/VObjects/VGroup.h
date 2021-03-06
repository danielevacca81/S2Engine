// VGroup.h
//
#ifndef VGROUP_ONCE
#define VGROUP_ONCE

#include "Framework_Exports.h"

#include "VObject.h"

#include "Math/Math.h"

#include <map>
#include <vector>



namespace OpenGL {
	class Renderer;
}

// ********************************************************
class VGroup;
typedef std::shared_ptr<VGroup> VGroupPtr;
// ********************************************************

class FWK_API VGroup : public VObject
{
private:
	std::map<unsigned int, VObjectPtr>        _objects;

	virtual void select( bool on );
	virtual void hilight( bool on);

public:
	static VGroupPtr New() { return std::make_shared<VGroup>(); }

	VGroup();
	~VGroup();

	void setName( const std::string &name );
	void setSelected( bool on );
	void setHilighted( bool on );
	void setStyle( const VStyle &s);
	void setColor( const OpenGL::Color &c );
	void enableSelection( bool );

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


#endif