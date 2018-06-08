// VPolygon.h
//
#ifndef VPOLYGON_ONCE
#define VPOLYGON_ONCE

#include "s2Renderer_API.h"

#include "VObject.h"

#include "Math/Math.h"

#include <vector>

namespace s2 {
namespace Renderer {


class S2RENDERER_API VPolygon : public VObject
{
private:
	std::vector<Math::dvec3> _points;
	std::vector<Math::dvec3> _border;
	std::vector<int> _indices;
public:
	VPolygon();
	VPolygon( const std::vector<Math::dvec3> &pts );

	~VPolygon() {}

	ObjectType type() const ;
	std::vector<Math::dvec3> snapPoints() const;
	Math::dvec3 center() const ;


	void draw( OpenGL::Renderer *r ) const ;
	void drawForSelection( OpenGL::Renderer *r ) const ;
	bool intersects( const Math::box3 &b ) const ;

	virtual std::vector<Math::dvec3> getPoints() const;
	virtual VObject* clone() const;
};

}}
#endif