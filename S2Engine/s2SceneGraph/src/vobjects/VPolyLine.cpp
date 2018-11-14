// VPolyLine.cpp
//
#include "VPolyLine.h"

#include "renderer/Surface.h"

#include "math/Intersection.h"

#include <iostream>

using namespace s2;
using namespace s2::SceneGraph;

// ------------------------------------------------------------------------------------------------
VPolyLine::VPolyLine()
{}

// ------------------------------------------------------------------------------------------------
VPolyLine::VPolyLine( const Math::dvec3 &p0, const Math::dvec3 &p1 )
: VPolyLine( std::vector<Math::dvec3> { p0,p1 } )
{}

// ------------------------------------------------------------------------------------------------
VPolyLine::VPolyLine( const std::vector<Math::dvec3> &points )
{
	for( auto &p : points )
	{
		_points.push_back( p );

		_boundingBox.extend( p );
	}
}

// ------------------------------------------------------------------------------------------------
VObject::VObjectBuffer VPolyLine::toBuffer() const
{
	VObjectBuffer b;
	b.vertices = _points;

	return std::move( b );
}

// ------------------------------------------------------------------------------------------------
void VPolyLine::draw( const Renderer::SurfacePtr &surface, const Renderer::DrawingState &ds ) const
{
	// inefficient draw
	Renderer::PrimitiveBufferPtr buf = Renderer::PrimitiveBuffer::New();

	auto obj = toBuffer();
	buf->setVertices( { obj.vertices.begin(),obj.vertices.end() } );

	surface->draw( Renderer::Primitive::LineStrip, buf, ds );
}

// ------------------------------------------------------------------------------------------------
//void VPolyLine::drawForSelection( OpenGL::Renderer *r ) const
//{
//	OpenGL::Selection::Name name( _id );
//
//	glLineWidth( 2 );
//	
//	r->beginRendering();
//	r->setVertex3DArray( _pts );
//
//	glDrawArrays( GL_LINES,0, 2 );
//
//	r->endRendering();
//}

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
std::vector<Math::dvec3> VPolyLine::points() const
{
	return _points;
}

//// ------------------------------------------------------------------------------------------------
//VObject* VLine::clone() const
//{
//	VLine* clone= new VLine;
//	this->copyDataIn( clone );
//
//	clone->_pts =_pts;
//
//	return clone;
//}