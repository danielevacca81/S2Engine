// VLine.cpp
//
#include "VLine.h"

#include "Renderer/Renderer.h"

#include "Geometry/Intersection.h"

using namespace s2;

// ------------------------------------------------------------------------------------------------
VLine::VLine()
{
	_pts.reserve(2);
	_pts.resize(2);
}

// ------------------------------------------------------------------------------------------------
VLine::VLine( const Math::dvec3 &p0, const Math::dvec3 &p1 )
{
	_pts.push_back( p0 );
	_pts.push_back( p1 );
	
	_boundingBox.extend( p0 );
	_boundingBox.extend( p1 );
}

// ------------------------------------------------------------------------------------------------
VObject::ObjectType VLine::type() const		{ return Line; }

// ------------------------------------------------------------------------------------------------
std::vector<Math::dvec3> VLine::snapPoints() const
{
	return _pts;
}

// ------------------------------------------------------------------------------------------------
Math::dvec3 VLine::center() const
{
	return _boundingBox.center();
}

// ------------------------------------------------------------------------------------------------
void VLine::draw( OpenGL::Renderer *r ) const
{
	if( _pts.empty() )
		return;

	//glColor3fv( color() );
	//	
	//glLineWidth( style().penSize() );

	//r->beginRendering();
	//r->setVertex3DArray( _pts );

	//glDrawArrays( GL_LINES,0, 2 );

	//r->endRendering();
}

// ------------------------------------------------------------------------------------------------
bool VLine::intersects( const Math::box3 &b ) const 
{
	return Math::lineBoxIntersection( _pts[0], _pts[1], b );
}

// ------------------------------------------------------------------------------------------------
std::vector<Math::dvec3> VLine::points() const
{
	return _pts;
}

// ------------------------------------------------------------------------------------------------
VObjectPtr VLine::clone() const
{
	return New( _pts[0], _pts[1] );
}