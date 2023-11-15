// VSplineStrip.h
//
#ifndef VSPLINESTRIP_ONCE
#define VSPLINESTRIP_ONCE

#include "s2SceneGraph_API.h"

#include "VObject.h"

#include "Math/Math.h"

#include <vector>

namespace s2 {
namespace SceneGraph {


class S2SCENEGRAPH_API VSplineStrip : public VObject
{
private:
	//bool _borders;
	std::vector<Math::dvec3>  _points;
	std::vector<unsigned int> _indices;
	std::vector<unsigned int> _rightIndices;
	std::vector<unsigned int> _leftIndices;
	double _width;
	//std::vector<Math::dvec3> _leftBorderPoints;
	//std::vector<Math::dvec3> _rightBorderPoints;

public:
	VSplineStrip();
	VSplineStrip( const std::vector<Math::dvec3> &pts, double width, const int level= 8 );

	~VSplineStrip() {}

	ObjectType type() const ;
	std::vector<Math::dvec3> snapPoints() const;
	Math::dvec3 center() const ;

	//void enableBorders( bool on );
	//bool bordersEnabled() const;

	void draw( OpenGL::Renderer *r ) const ;
	void drawForSelection( OpenGL::Renderer *r ) const ;
	bool intersects( const Math::box3 &b ) const ;

	virtual std::vector<Math::dvec3> getPoints() const;
	virtual VObject* clone() const;
};

}}
#endif