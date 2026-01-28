// VPolyline.cpp
//
#include "VPolygon.h"

#include "OpenGL/Renderer.h"
#include "OpenGL/Selection.h"
#include "OpenGL/Tessellation.h"
#include "OpenGL/OpenGL.h"

#include "Geometry/Intersection.h"

using namespace s2;
using namespace s2::Renderer;

// ------------------------------------------------------------------------------------------------
VPolygon::VPolygon()
{}

// ------------------------------------------------------------------------------------------------
VPolygon::VPolygon( const std::vector<Math::dvec3> &pts )
{
	_points = pts;
	OpenGL::tesselate( _points, _indices );
	_border = _points;

	for( size_t i=0; i<_points.size(); ++i )
		_boundingBox.extend( _points[i] );
}

// ------------------------------------------------------------------------------------------------
VObject::ObjectType VPolygon::type() const		{ return Shape; }

// ------------------------------------------------------------------------------------------------
std::vector<Math::dvec3> VPolygon::snapPoints() const
{
	return _points;
}

// ------------------------------------------------------------------------------------------------
Math::dvec3 VPolygon::center() const
{
	return _boundingBox.center();
}

// ------------------------------------------------------------------------------------------------
void VPolygon::draw( OpenGL::Renderer *r ) const
{
	glColor4fv( color() );

	r->beginRendering();
	r->setVertex3DArray( _points );

	glDrawElements( GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, &_indices[0] );

	if( style().penSize() > 0.0 )
	{
		glColor4fv( color().darker() );
		glLineWidth( style().penSize() );

		r->setVertex3DArray( _border );
		glDrawArrays( GL_LINE_LOOP,0, _border.size() );
	}

	r->endRendering();
}

// ------------------------------------------------------------------------------------------------
void VPolygon::drawForSelection( OpenGL::Renderer *r ) const
{
	OpenGL::Selection::Name name( _id );

	r->beginRendering();
	r->setVertex3DArray( _points );

	glDrawElements( GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, &_indices[0] );

	r->endRendering();
}

// ------------------------------------------------------------------------------------------------
bool VPolygon::intersects( const Math::box3 &b ) const 
{
	// test p0-p1 against box edges
	bool intersect = false;
	size_t nb= _border.size();

	for( size_t i=0; i<_indices.size(); i+=3 )
	{
		const Math::dvec2 t0( _points[ _indices[i] ].x, _points[ _indices[i] ].y );
		const Math::dvec2 t1( _points[ _indices[i+1] ].x, _points[ _indices[i+1] ].y );
		const Math::dvec2 t2( _points[ _indices[i+2] ].x, _points[ _indices[i+2] ].y );

		intersect = Math::triangleBox2DIntersection( t0, t1, t2, b );
		if( intersect )
			break;
	}

	return intersect;
}

// ------------------------------------------------------------------------------------------------
std::vector<Math::dvec3> VPolygon::getPoints() const
{
	return _border;
}

// ------------------------------------------------------------------------------------------------
VObject* VPolygon::clone() const
{
	VPolygon* clone= new VPolygon;
	this->copyDataIn( clone );

	clone->_border=	_border;
	clone->_points=	_points;

	return clone;
}