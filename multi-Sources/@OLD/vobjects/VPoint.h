// VPoint.h
//
#ifndef VPOINT_ONCE
#define VPOINT_ONCE

#include "s2SceneGraph_API.h"

#include "VObject.h"

#include "math/Math.h"


namespace s2 {
namespace SceneGraph {

// ********************************************************
class VPoint;
typedef std::shared_ptr<VPoint> VPointPtr;
// ********************************************************

class S2SCENEGRAPH_API VPoint: public VObject
{
public:
	static VPointPtr New() { return std::make_shared<VPoint>(); }
	static VPointPtr New( const Math::dvec3 &p ) { return std::make_shared<VPoint>( p ); }

public:
	VPoint();
	VPoint( const Math::dvec3 &p );
	~VPoint() {}

	ObjectType               type() const { return Point; }

	void draw( const Renderer::SurfacePtr &surface, const Renderer::DrawingState &ds ) const override;
	bool intersects( const Math::box3 &b ) const override;

	std::vector<Math::dvec3> points() const override;
	VObjectPtr               clone() const override { return { }; } 
	void                     set( const VObjectPtr &o ) override {}

protected:
	VObjectBuffer toBuffer() const override;

private:
	Math::dvec3 _coords;
};


}}
#endif