// VLine.cpp
//
#include "VLine.h"

#include "OpenGL/Renderer.h"
#include "OpenGL/Selection.h"
#include "OpenGL/OpenGL.h"

#include "Geometry/Intersection.h"

// ------------------------------------------------------------------------------------------------
VLine::VLine()
{}

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
	glColor3fv( color() );
		
	glLineWidth( style().penSize() );

	r->beginRendering();
	r->setVertex3DArray( _pts );

	glDrawArrays( GL_LINES,0, 2 );

	r->endRendering();
}

// ------------------------------------------------------------------------------------------------
void VLine::drawForSelection( OpenGL::Renderer *r ) const
{
	OpenGL::Selection::Name name( _id );

	glLineWidth( 2 );
	
	r->beginRendering();
	r->setVertex3DArray( _pts );

	glDrawArrays( GL_LINES,0, 2 );

	r->endRendering();
}

// ------------------------------------------------------------------------------------------------
bool VLine::intersects( const Math::box3 &b ) const 
{
	return lineBoxIntersection( _pts[0], _pts[1], b );
}

// ------------------------------------------------------------------------------------------------
std::vector<Math::dvec3> VLine::getPoints() const
{
	return _pts;
}

// ------------------------------------------------------------------------------------------------
VObject* VLine::clone() const
{
	VLine* clone= new VLine;
	this->copyDataIn( clone );

	clone->_pts =_pts;

	return clone;
}