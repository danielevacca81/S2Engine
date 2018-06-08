// VCircle.cpp
//
#include "VCircle.h"

#include "OpenGL/Renderer.h"
#include "OpenGL/Selection.h"
#include "OpenGL/OpenGL.h"

#include "Geometry/Geometry.h"

#include <iostream>

using namespace s2;
using namespace s2::Renderer;

// ------------------------------------------------------------------------------------------------
VCircle::VCircle()
{}

// ------------------------------------------------------------------------------------------------
VCircle::VCircle( const Math::dvec3 &center, double radius )
: _center(center),
  _radius(radius)
{
	createCircle(32);

	_boundingBox = Math::box3( Math::dvec3(_center)-radius,Math::dvec3(_center)+radius );
}

// ------------------------------------------------------------------------------------------------
VCircle::~VCircle()
{}

// ------------------------------------------------------------------------------------------------
void VCircle::createCircle( int  LOD )
{
	_circlePoints = Geometry::circle(_center,_radius,LOD);

	for( size_t i=0; i<_circlePoints.size(); ++i )
		_indices.push_back(i);
}

// ------------------------------------------------------------------------------------------------
VObject::ObjectType VCircle::type() const		{ return Shape; }

// ------------------------------------------------------------------------------------------------
std::vector<Math::dvec3> VCircle::snapPoints() const
{
	std::vector<Math::dvec3> pts;
	pts.push_back( _center );
	pts.push_back( _center + Math::dvec3( _radius, 0, 0) );
	pts.push_back( _center + Math::dvec3( 0, _radius, 0) );
	pts.push_back( _center + Math::dvec3( -_radius, 0, 0) );
	pts.push_back( _center + Math::dvec3( 0, -_radius, 0) );

	return pts;
}

// ------------------------------------------------------------------------------------------------
Math::dvec3 VCircle::center() const
{
	return _center;
}

// ------------------------------------------------------------------------------------------------
void VCircle::draw( OpenGL::Renderer *r ) const
{
	// Draw inner
	const float alpha             = style().baseColor().a();
	const OpenGL::Color fillColor = color();

	glColor4f( fillColor.r(),fillColor.g(),fillColor.b(), alpha );

	r->beginRendering();
	r->setVertex3DArray( _circlePoints );

	glDrawElements(GL_TRIANGLE_FAN,_indices.size(),GL_UNSIGNED_INT,&_indices[0]);

	if( style().penSize() > 0.0 )
	{
		const OpenGL::Color borderColor = color().darker();
		glLineWidth( style().penSize() );
		glColor4f( borderColor.r(),borderColor.g(),borderColor.b(), alpha );

		glDrawElements(GL_LINE_LOOP,_indices.size()-2,GL_UNSIGNED_INT,&_indices[1]); // skip center and last point
	}
		
	r->endRendering();

}

// ------------------------------------------------------------------------------------------------
void VCircle::drawForSelection( OpenGL::Renderer *r ) const
{
	OpenGL::Selection::Name name( _id );

	r->beginRendering();
	r->setVertex3DArray( _circlePoints );

	glDrawElements(GL_TRIANGLE_FAN,_indices.size(),GL_UNSIGNED_INT,&_indices[0]);

	r->endRendering();

}

// ------------------------------------------------------------------------------------------------
bool VCircle::intersects( const Math::box3 &b ) const
{
	Math::dvec3 dist;
	dist.x = Math::abs( _center.x-b.center().x );
	dist.y = Math::abs( _center.y-b.center().y );

	const double rw = b.sizes().x*0.5;
	const double rh = b.sizes().y*0.5;

	if( dist.x > rw + _radius )	return false;
    if( dist.y > rh + _radius ) return false;

    if( dist.x <= rw )	return true;
    if( dist.y <= rh )	return true;

    double cornerDistance_sq = (dist.x - rw)*(dist.x - rw) + (dist.y - rh)*(dist.y - rh);

    return (cornerDistance_sq <= (_radius*_radius));
}

// ------------------------------------------------------------------------------------------------
std::vector<Math::dvec3> VCircle::getPoints() const
{
	std::vector<Math::dvec3> vp(2);
	vp[0]= _center;
	vp[1]= _center + Math::dvec3(_radius, 0, 0);
	return vp;
}

// ------------------------------------------------------------------------------------------------
VObject* VCircle::clone() const
{
	VCircle* clone= new VCircle;
	this->copyDataIn( clone );

	clone->_center=	_center;
	clone->_radius=	_radius;

	return clone;
}