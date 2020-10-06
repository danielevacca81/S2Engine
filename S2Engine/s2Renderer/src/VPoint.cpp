// VPoint.cpp
//
#include "VPoint.h"

#include "s2Renderer/Surface.h"

#include <iostream>


using namespace Scene;

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
VObject::VObjectBuffer VPoint::toBuffer() const
{
	VObjectBuffer b;
	b.vertices = { _coords };

	return std::move( b );
}

// ------------------------------------------------------------------------------------------------
void VPoint::draw( const Renderer::SurfacePtr &surface, const Renderer::DrawingState &ds ) const
{
	// inefficient draw
	Renderer::PrimitiveBufferPtr buf = Renderer::PrimitiveBuffer::New();

	auto obj = toBuffer();
	buf->setVertices( { obj.vertices.begin(),obj.vertices.end() } );

	surface->draw( Renderer::PrimitiveType::Points, buf, ds );
}

// ------------------------------------------------------------------------------------------------
//void VPoint::drawForSelection( OpenGL::Renderer *r ) const
//{
//	OpenGL::Selection::Name name( _id );
//
//	glPointSize(style().penSize());
//
//	r->beginRendering();
//	r->setVertex3DArray( _coords );
//
//	glDrawArrays( GL_POINTS, 0, 1 );
//
//	r->endRendering();
//}

// ------------------------------------------------------------------------------------------------
bool VPoint::intersects( const Math::box3 &b ) const
{
	return b.contains( _coords );
}

// ------------------------------------------------------------------------------------------------
std::vector<Math::dvec3> VPoint::vertices() const
{
	return { _coords };
}

// ------------------------------------------------------------------------------------------------
//VObject* VPoint::clone() const
//{
//	VPoint* clone= new VPoint;
//	this->copyDataIn( clone );
//
//	clone->_coords=	_coords;
//
//	return clone;
//}