// VPolyLine.h
//
#ifndef VPOLYLINE_ONCE
#define VPOLYLINE_ONCE

#include "Framework_Exports.h"

#include "VObject.h"

#include "Math/Math.h"

#include <vector>

namespace OpenGL {
	class Renderer;
}

// ********************************************************
class VPolyLine;
typedef std::shared_ptr<VPolyLine> VPolyLinePtr;
// ********************************************************

class FWK_API VPolyLine : public VObject
{
private:
	std::vector<Math::dvec3> _points;
	std::vector<int>         _indices;

public:
	static VPolyLinePtr New()                                      { return std::make_shared<VPolyLine>(); }
	static VPolyLinePtr New( const std::vector<Math::dvec3> &pts ) { return std::make_shared<VPolyLine>(pts); }

	VPolyLine();
	VPolyLine( const std::vector<Math::dvec3> &pts );

	~VPolyLine() {}

	ObjectType type() const ;
	std::vector<Math::dvec3> snapPoints() const;
	Math::dvec3 center() const ;


	void draw( OpenGL::Renderer *r ) const ;
	void drawForSelection( OpenGL::Renderer *r ) const ;
	bool intersects( const Math::box3 &b ) const ;

	virtual std::vector<Math::dvec3> getPoints() const;
	virtual VObject* clone() const;
};

#endif