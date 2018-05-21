// VCircle.h
//
#ifndef VCIRCLE_ONCE
#define VCIRCLE_ONCE

#include "Framework_Exports.h"

#include "VObject.h"

#include "Math/Math.h"

namespace OpenGL {
	class Renderer;
}

class FWK_API VCircle : public VObject
{
private:
	Math::dvec3 _center;
	double              _radius;

	std::vector<Math::dvec3> _circlePoints;
	std::vector<int>         _indices;
	void createCircle( int LOD );

public:
	VCircle();
	VCircle( const Math::dvec3 &center, double radius );

	~VCircle();

	ObjectType type() const ;
	std::vector<Math::dvec3> snapPoints() const;
	Math::dvec3 center() const;

	void draw( OpenGL::Renderer *r ) const;
	void drawForSelection( OpenGL::Renderer *r ) const;
	bool intersects( const Math::box3 &b ) const;

	std::vector<Math::dvec3> getPoints() const;
	VObject* clone() const;
};

#endif