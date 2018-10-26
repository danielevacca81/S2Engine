// VLine.h
//
#ifndef VLINE_ONCE
#define VLINE_ONCE

#include "s2SceneGraph_API.h"

#include "VObject.h"

#include "Math/Math.h"


namespace s2 {
namespace SceneGraph {


class S2SCENEGRAPH_API VLine : public VObject
{
private:
	std::vector<Math::dvec3> _pts;

public:
	VLine();
	VLine( const Math::dvec3 &p0, const Math::dvec3 &p1 );

	~VLine() {}

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