// VPoint.h
//
#ifndef VPOINT_ONCE
#define VPOINT_ONCE

#include "Framework_Exports.h"

#include "VObject.h"

#include "Math/Math.h"

namespace OpenGL {
	class Renderer;
}


class FWK_API VPoint : public VObject
{
private:
	std::vector<Math::dvec3> _coords;

public:
	VPoint();
	VPoint( double x, double y, double z );
	VPoint( const Math::dvec3 &p );

	~VPoint() {}

	ObjectType type() const ;
	std::vector<Math::dvec3> snapPoints() const;
	Math::dvec3 center() const ;

	void draw( OpenGL::Renderer *r ) const;
	void drawForSelection( OpenGL::Renderer *r ) const;
	bool intersects( const Math::box3 &b ) const;

	virtual std::vector<Math::dvec3> getPoints() const;
	virtual VObject* clone() const;
};

#endif