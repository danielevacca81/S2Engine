// VPolyline.cpp
//
#include "VPolyLine.h"

#include "OpenGL/Renderer.h"
#include "OpenGL/Selection.h"
#include "OpenGL/OpenGL.h"

#include "Math/Intersection.h"

// ------------------------------------------------------------------------------------------------
VPolyLine::VPolyLine()
{}

// ------------------------------------------------------------------------------------------------
VPolyLine::VPolyLine( const std::vector<Math::dvec3> &pts )
{
	_points = pts;

	for( size_t i=0; i<pts.size(); ++i )
		_indices.push_back(i);

	_boundingBox.reset();

	for( size_t i=0; i<_points.size(); ++i )
		_boundingBox.extend( _points[i] );
}

// ------------------------------------------------------------------------------------------------
VObject::ObjectType VPolyLine::type() const		{ return PolyLine; }

// ------------------------------------------------------------------------------------------------
std::vector<Math::dvec3> VPolyLine::snapPoints() const
{
	std::vector<Math::dvec3> pts;
	for( size_t i=0; i<_points.size(); ++i )
		pts.push_back( _points[i] );

	return pts;
}

// ------------------------------------------------------------------------------------------------
Math::dvec3 VPolyLine::center() const
{
	if( _points.empty() )
		return Math::dvec3();
	
	return _points[ _points.size()/2];
}

// ------------------------------------------------------------------------------------------------
void VPolyLine::draw( OpenGL::Renderer *r ) const
{
	if( _points.empty() )
		return;

	glPushAttrib(GL_DEPTH_BUFFER_BIT);
		glDepthMask( GL_FALSE );

		r->beginRendering();
		r->setVertex3DArray( _points );

		glColor4fv( color() );
		glLineWidth( style().penSize() );
		glDrawElements( GL_LINE_STRIP, _indices.size(), GL_UNSIGNED_INT, &_indices[0] );

		r->endRendering();

	glPopAttrib();	
}

// ------------------------------------------------------------------------------------------------
void VPolyLine::drawForSelection( OpenGL::Renderer *r ) const
{
	OpenGL::Selection::Name name( _id );

	r->beginRendering();
	r->setVertex3DArray( _points );

	glColor4fv( color() );
	glLineWidth( style().penSize() );
	glDrawElements( GL_LINE_STRIP, _indices.size(), GL_UNSIGNED_INT, &_indices[0] );

	r->endRendering();
}

// ------------------------------------------------------------------------------------------------
bool VPolyLine::intersects( const Math::box3 &b ) const 
{
	// test p0-p1 against box edges
	bool intersect = false;

	for( size_t i=0; i<_points.size()-1; ++i )
	{
		const Math::dvec2 p0( _points[i+0].x,_points[i+0].y );
		const Math::dvec2 p1( _points[i+1].x,_points[i+1].y );

		intersect |= Math::lineIntersection( p0,p1, Math::dvec2(b.minPoint().x,b.minPoint().y), Math::dvec2(b.minPoint().x,b.maxPoint().y) ); // left
		intersect |= Math::lineIntersection( p0,p1, Math::dvec2(b.minPoint().x,b.maxPoint().y), Math::dvec2(b.maxPoint().x,b.maxPoint().y) ); // top
		intersect |= Math::lineIntersection( p0,p1, Math::dvec2(b.maxPoint().x,b.maxPoint().y), Math::dvec2(b.maxPoint().x,b.minPoint().y) ); // right
		intersect |= Math::lineIntersection( p0,p1, Math::dvec2(b.maxPoint().x,b.minPoint().y), Math::dvec2(b.minPoint().x,b.minPoint().y) ); // bottom

		if( intersect )
			break;
	}

	return intersect;
}

// ------------------------------------------------------------------------------------------------
std::vector<Math::dvec3> VPolyLine::getPoints() const
{
	return _points;
}

// ------------------------------------------------------------------------------------------------
VObject* VPolyLine::clone() const
{
	VPolyLine* clone= new VPolyLine;
	this->copyDataIn( clone );

	clone->_points=	_points;

	return clone;
}