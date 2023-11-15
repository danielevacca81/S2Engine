// VSplineStrip.h
//
#ifndef VSPLINESTRIP_ONCE
#define VSPLINESTRIP_ONCE

#include "Framework_Exports.h"

#include "VObject.h"

#include "Math/Math.h"

#include <vector>

namespace OpenGL {
	class Renderer;
}

// ********************************************************
class VSplineStrip;
typedef std::shared_ptr<VSplineStrip> VSplineStripPtr;
// ********************************************************

class FWK_API VSplineStrip : public VObject
{
private:
	std::vector<Math::vec4>    _colors;
	std::vector<Math::dvec3>   _points;
	std::vector<Math::dvec3>   _centerLine;
	std::vector<unsigned int>  _indices;
	std::vector<unsigned int>  _rightIndices;
	std::vector<unsigned int>  _leftIndices;
	double _width;

public:
	static VSplineStripPtr New()                                                                        { return std::make_shared<VSplineStrip>(); }
	static VSplineStripPtr New( const std::vector<Math::dvec3> &pts, double width, const int level= 8 ) { return std::make_shared<VSplineStrip>(pts,width,level); }

	VSplineStrip();
	VSplineStrip( const std::vector<Math::dvec3> &pts, double width, const int level= 8 );

	~VSplineStrip() {}

	void setColors( const std::vector<Math::vec4> &colors) { _colors = colors; }

	ObjectType                type()       const;
	std::vector<Math::dvec3>  snapPoints() const;
	std::vector<Math::dvec3>  centerLine() const;
	std::vector<Math::vec4>   colors()     const;
	Math::dvec3               center()     const;

	void draw( OpenGL::Renderer *r ) const ;
	void drawForSelection( OpenGL::Renderer *r ) const ;
	bool intersects( const Math::box3 &b ) const ;

	virtual std::vector<Math::dvec3> getPoints() const;
	virtual VObject* clone() const;
};

#endif