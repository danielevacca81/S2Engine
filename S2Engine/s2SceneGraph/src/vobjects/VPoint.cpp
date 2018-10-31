// VPoint.cpp
//
#include "VPoint.h"

#include "renderer/Surface.h"

#include <iostream>

using namespace s2;
using namespace s2::SceneGraph;

// ------------------------------------------------------------------------------------------------
VPoint::VPoint()
{}

// ------------------------------------------------------------------------------------------------
VPoint::VPoint( const Math::dvec3 &p )
{
	_coords = p;

	_boundingBox.extend( _coords );
	_boundingBox.scale(1.2);
}

// ------------------------------------------------------------------------------------------------
VObject::ObjectType VPoint::type() const		{ return Point; }

// ------------------------------------------------------------------------------------------------
void VPoint::draw( const Renderer::SurfacePtr &surface, const Renderer::DrawingState &ds ) const
{
	//const float borderSize = style().penSize() + style().borderSize();
	//glPointSize(borderSize + (isHilighted() * 3.f) );
	//glColor4fv( style().borderColor() );

	//r->beginRendering();
	//r->setVertex3DArray( _coords );
	//// background as border
	//glDrawArrays( GL_POINTS, 0, 1 );

	//glColor4fv( color() );
	//glPointSize(style().penSize() + (isHilighted() * 3.f) );

	//glDrawArrays( GL_POINTS, 0, 1 );

	//r->endRendering();
}

// ------------------------------------------------------------------------------------------------
void VPoint::drawForSelection( OpenGL::Renderer *r ) const
{
	OpenGL::Selection::Name name( _id );

	glPointSize(style().penSize());

	r->beginRendering();
	r->setVertex3DArray( _coords );

	glDrawArrays( GL_POINTS, 0, 1 );

	r->endRendering();
}

// ------------------------------------------------------------------------------------------------
bool VPoint::intersects( const Math::box3 &b ) const
{
	return b.contains( _coords[0] );
}

// ------------------------------------------------------------------------------------------------
std::vector<Math::dvec3> VPoint::getPoints() const
{
	return _coords;
}

// ------------------------------------------------------------------------------------------------
VObject* VPoint::clone() const
{
	VPoint* clone= new VPoint;
	this->copyDataIn( clone );

	clone->_coords=	_coords;

	return clone;
}