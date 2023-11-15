// VRectangle.cpp
//
#include "VRectangle.h"

#include "OpenGL/Renderer.h"
#include "OpenGL/Selection.h"
#include "OpenGL/OpenGL.h"

#include <iostream>

#include "Utils/MemCheck.h"

// ------------------------------------------------------------------------------------------------
VRectangle::VRectangle()
{}

// ------------------------------------------------------------------------------------------------
VRectangle::VRectangle( const Math::dvec3 &bottomLeft, const Math::dvec3 &topRight )
//: _rect(bottomLeft,topRight)
{
	_points.push_back( Math::dvec3(bottomLeft.x,bottomLeft.y,0) );
	_points.push_back( Math::dvec3(topRight.x,  bottomLeft.y,0) );
	_points.push_back( Math::dvec3(topRight.x,  topRight.y,0) );
	_points.push_back( Math::dvec3(bottomLeft.x,topRight.y,0) );

	_boundingBox = Math::box3(bottomLeft,topRight);
}

// ------------------------------------------------------------------------------------------------
VRectangle::VRectangle( const Math::box3 &rect )
//: _rect(rect)
{
	_points.push_back( Math::dvec3(rect.minPoint().x,rect.minPoint().y,0) );
	_points.push_back( Math::dvec3(rect.maxPoint().x,rect.minPoint().y,0) );
	_points.push_back( Math::dvec3(rect.maxPoint().x,rect.maxPoint().y,0) );
	_points.push_back( Math::dvec3(rect.minPoint().x,rect.maxPoint().y,0) );

	_boundingBox = Math::box3(rect);
}

// ------------------------------------------------------------------------------------------------
VObject::ObjectType VRectangle::type() const		{ return Shape; }

// ------------------------------------------------------------------------------------------------
std::vector<Math::dvec3> VRectangle::snapPoints() const
{
	std::vector<Math::dvec3> pts(_points);
	pts.push_back( _boundingBox.center() );

	return pts;
}

// ------------------------------------------------------------------------------------------------
Math::dvec3 VRectangle::center() const
{
	return _boundingBox.center();
}

// ------------------------------------------------------------------------------------------------
void VRectangle::draw( OpenGL::Renderer *r ) const
{
	if( _points.empty() )
		return;

	// Draw inner
	const float alpha             = style().baseColor().a();
	const OpenGL::Color fillColor = color();

	glColor4f( fillColor.r(),fillColor.g(),fillColor.b(), alpha );
	
	r->beginRendering();
	r->setVertex3DArray( _points );

	glDrawArrays( GL_QUADS, 0, _points.size() );

	if( style().penSize() > 0.0 )
	{
		const OpenGL::Color borderColor = color().darker();
		glLineWidth( style().penSize() );
		glColor4f( borderColor.r(),borderColor.g(),borderColor.b(), alpha );

		glDrawArrays( GL_LINE_LOOP, 0, _points.size() );
	}

	r->endRendering();


	//const Math::dvec3 o = r->origin();

	//glPushAttrib(GL_COLOR_BUFFER_BIT|GL_CURRENT_BIT|GL_LINE_BIT);

	//const Math::box3 box = _rect.translated(-o);

	//// fill
	////if( _opaque ) glColor3fv( color() );
	////else          glColor4fv( color().transparent() );

	//glColor4fv( color() );
	//
	//glBegin( GL_TRIANGLE_STRIP );
	//	glVertex3d( box.minPoint().x, box.maxPoint().y, box.minPoint().z );
	//	glVertex3d( box.minPoint().x, box.minPoint().y, box.minPoint().z );
	//	glVertex3d( box.maxPoint().x, box.maxPoint().y, box.minPoint().z );
	//	glVertex3d( box.maxPoint().x, box.minPoint().y, box.minPoint().z );
	//glEnd();

	//// borders
	//if( style().penSize() > 0.0 )
	//{
	//	glLineWidth( style().penSize() );
	//	glColor4fv( color().darker() );
	//	glBegin( GL_LINE_LOOP);
	//		glVertex3d( box.minPoint().x, box.maxPoint().y, box.minPoint().z );
	//		glVertex3d( box.minPoint().x, box.minPoint().y, box.minPoint().z );
	//		glVertex3d( box.maxPoint().x, box.minPoint().y, box.minPoint().z );
	//		glVertex3d( box.maxPoint().x, box.maxPoint().y, box.minPoint().z );
	//	glEnd();
	//}

	////glPopAttrib();
}

// ------------------------------------------------------------------------------------------------
void VRectangle::drawForSelection( OpenGL::Renderer *r ) const
{
	OpenGL::Selection::Name name( _id );

	r->beginRendering();
	r->setVertex3DArray( _points );

	glDrawArrays( GL_QUADS, 0, _points.size() );

	r->endRendering();

}

// ------------------------------------------------------------------------------------------------
bool VRectangle::intersects( const Math::box3 &b ) const
{
	return _boundingBox.overlaps( b );
}

// ------------------------------------------------------------------------------------------------
std::vector<Math::dvec3> VRectangle::getPoints() const
{
	//std::vector<Math::dvec3> vp(2);
	//vp[0]= _boundingBox.minPoint();
	//vp[1]= _boundingBox.maxPoint();
	//return vp;
	return _points;
}

// ------------------------------------------------------------------------------------------------
VObject* VRectangle::clone() const
{
	VRectangle* clone= new VRectangle;
	this->copyDataIn( clone );

	clone->_points = _points;
	//TEST clone->_rect.translate( Math::dvec3( 1., .5, 0.) );

	return clone;
}