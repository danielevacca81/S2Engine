// VLine.h
//
#ifndef VLINE_ONCE
#define VLINE_ONCE

#include "VObject.h"

namespace s2 {

DECLARE_PTR(VLine)

class S2CAD_API VLine : public VObject
{
public:
	static VLinePtr New() { return std::make_shared<VLine>(); }
	static VLinePtr New( const Math::dvec3 &p0, const Math::dvec3 &p1 ) { return std::make_shared<VLine>(p0,p1); }

	VLine();
	VLine( const Math::dvec3 &p0, const Math::dvec3 &p1 );

	~VLine() {}

	ObjectType type() const ;
	std::vector<Math::dvec3> snapPoints() const;
	Math::dvec3 center() const ;


	void draw( OpenGL::Renderer *r ) const ;
	bool intersects( const Math::box3 &b ) const ;

	std::vector<Math::dvec3> points() const;
	VObjectPtr clone() const;

private:
	std::vector<Math::dvec3> _pts;
};

}

#endif