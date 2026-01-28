// VCircle.h
//
#ifndef VCIRCLE_ONCE
#define VCIRCLE_ONCE

#include "VObject.h"

namespace s2 {

DECLARE_PTR(VCircle)

class S2CAD_API VCircle : public VObject
{
public:
	static VCirclePtr New() { return std::make_shared<VCircle>(); }
	static VCirclePtr New(const Math::dvec3 &center, double radius) { return std::make_shared<VCircle>(center,radius); }

	VCircle();
	VCircle( const Math::dvec3 &center, double radius );
	~VCircle();

	ObjectType type() const ;
	std::vector<Math::dvec3> snapPoints() const;
	Math::dvec3 center() const;

	void draw( OpenGL::Renderer *r ) const;
	bool intersects( const Math::box3 &b ) const;

	std::vector<Math::dvec3> points() const;
	VObjectPtr clone() const;

private:
	Math::dvec3              _center;
	double                   _radius;
	std::vector<Math::dvec3> _circlePoints;
	std::vector<int>         _indices;
	void createCircle( int LOD );
};

}
#endif