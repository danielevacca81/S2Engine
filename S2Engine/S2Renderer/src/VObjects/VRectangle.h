// VRectangle.h
//
#ifndef VRECTANGLE_ONCE
#define VRECTANGLE_ONCE

#include "Framework_Exports.h"

#include "VObject.h"

#include "Math/Math.h"
#include "Math/Box.h"

namespace OpenGL {
	class Renderer;
}


class FWK_API VRectangle : public VObject
{
private:
	//Math::box3 _rect;
	std::vector<Math::dvec3> _points;

public:
	VRectangle();
	VRectangle( const Math::dvec3 &bottomLeft, const Math::dvec3 &topRight );
	VRectangle( const Math::box3 &rect );

	~VRectangle() {}

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