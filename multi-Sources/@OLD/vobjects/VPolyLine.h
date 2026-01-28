// VPolyLine.h
//
#ifndef VPOLYLINE_ONCE
#define VPOLYLINE_ONCE

#include "s2SceneGraph_API.h"

#include "VObject.h"

#include "Math/Math.h"


namespace s2 {
namespace SceneGraph {

// ********************************************************
class VPolyLine;
typedef std::shared_ptr<VPolyLine> VPolyLinePtr;
// ********************************************************

class S2SCENEGRAPH_API VPolyLine : public VObject
{
public:
	static VPolyLinePtr New() { return std::make_shared<VPolyLine>(); }
	static VPolyLinePtr New( const Math::dvec3 &p0, const Math::dvec3 &p1 ) { return std::make_shared<VPolyLine>( p0,p1 ); }
	static VPolyLinePtr New( const std::vector<Math::dvec3> &points ) { return std::make_shared<VPolyLine>( points ); }

public:
	VPolyLine();
	VPolyLine( const Math::dvec3 &p0, const Math::dvec3 &p1 );
	VPolyLine( const std::vector<Math::dvec3> &points );
	~VPolyLine() {}

	ObjectType               type() const { return Polyline; }

	void draw( const Renderer::SurfacePtr &surface, const Renderer::DrawingState &ds ) const override;
	bool intersects( const Math::box3 &b ) const override;

	std::vector<Math::dvec3> points() const override;
	VObjectPtr               clone() const override { return { }; } 
	void                     set( const VObjectPtr &o ) override {}

protected:
	VObjectBuffer toBuffer() const override;

private:
	std::vector<Math::dvec3> _points;
};

}}
#endif