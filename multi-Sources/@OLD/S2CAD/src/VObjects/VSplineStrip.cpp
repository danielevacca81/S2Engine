// VPolyline.cpp
//
#include "VSplineStrip.h"

#include "OpenGL/Renderer.h"
#include "OpenGL/Selection.h"
#include "OpenGL/OpenGL.h"

#include "Math/Intersection.h"
#include "Math/Interpolation.h"

// ------------------------------------------------------------------------------------------------
VSplineStrip::VSplineStrip()
//: _borders(true)
{}

// ------------------------------------------------------------------------------------------------
VSplineStrip::VSplineStrip( const std::vector<Math::dvec3> &pts, double width, const int level )
//: _borders(true)
{
	_centerLine = pts;
	if( pts.size() == 1 )
		return;
	
	_width= width;

	std::vector<Math::dvec3> centerline;
	Math::Interpolation::CatmullRom( level, pts, centerline );


	//_leftBorderPoints.resize(np);
	//_rightBorderPoints.resize(np);
	size_t npmo = centerline.size()-1; // np minus one
	
	for( size_t i=0, j=0; i < centerline.size(); ++i )
	{		
		Math::dvec3 delta;
		
		if( i == 0 )        delta = centerline[i+1]-centerline[i];        // first point
		else if( i < npmo ) delta = (centerline[i+1]-centerline[i-1])*.5; // mid points
		else                delta = centerline[npmo]-centerline[npmo-1];  // last point

		const double m = Math::length( delta );
		
		if( m < 1.e-12 )
		{
			_points.push_back( centerline[i] );
			_indices.push_back(i);
			//_leftBorderPoints[i]= _rightBorderPoints[i]= centerline[i];
		}
		else
		{
			delta = .5 * delta * width / m;

			const Math::dvec3 lPoint = centerline[i] + Math::dvec3( -delta.y, +delta.x, 0. );
			const Math::dvec3 rPoint = centerline[i] + Math::dvec3( +delta.y, -delta.x, 0. );


			_points.push_back( lPoint );
			_points.push_back( rPoint );
			
			if( i==0 )
				_leftIndices.push_back(1);

			if( i > 0 )
			{
				_indices.push_back(j+0);
				_indices.push_back(j+1);
				_indices.push_back(j+2);

				_indices.push_back(j+2);
				_indices.push_back(j+1);
				_indices.push_back(j+3);

				_leftIndices.push_back(j+0);
				_rightIndices.push_back(j+1);

				j+=2;
			}

			_boundingBox.extend( lPoint );
			_boundingBox.extend( rPoint );
		}
	}
	
	_leftIndices.push_back(_leftIndices.back()+2);
	_rightIndices.push_back(_rightIndices.back()+2);
	_leftIndices.push_back(_rightIndices.back());
}

// ------------------------------------------------------------------------------------------------
VObject::ObjectType VSplineStrip::type() const		{ return _width <= 0.0 ? PolyLine : Shape; }

// ------------------------------------------------------------------------------------------------
std::vector<Math::dvec3> VSplineStrip::snapPoints() const
{
	return _points;
}

// ------------------------------------------------------------------------------------------------
std::vector<Math::vec4> VSplineStrip::colors() const { return _colors; }

// ------------------------------------------------------------------------------------------------
std::vector<Math::dvec3>  VSplineStrip::centerLine() const { return _centerLine; }

// ------------------------------------------------------------------------------------------------
Math::dvec3 VSplineStrip::center() const
{
	if( !_points.empty() )
		return _points[ _points.size()/2];
	
	return Math::dvec3();
}

// ------------------------------------------------------------------------------------------------
void VSplineStrip::draw( OpenGL::Renderer *r ) const
{
	if( _points.empty() )
		return;

	r->beginRendering();
	r->setVertex3DArray( _points );

	if( _width > 0 )
	{
		glColor4fv( color() );
		//if( _opaque ) glColor3fv( color() );
		//else          glColor4fv( color().transparent() );

		//glBegin(GL_QUAD_STRIP);
		//for( size_t i=0; i<_rightBorderPoints.size(); ++i )
		//{
		//	glVertex3dv( Math::value_ptr(_rightBorderPoints[i] - o) );
		//	glVertex3dv( Math::value_ptr(_leftBorderPoints[i] - o) );
		//}
		//glEnd();
		glDrawElements( GL_TRIANGLES,_indices.size(),GL_UNSIGNED_INT,&_indices[0] );
	}
	
	if( style().penSize() > 0.0 )
	{
		glColor4fv( color().darker() );
		glLineWidth( style().penSize() );

		glDrawElements( GL_LINE_STRIP,_rightIndices.size(),GL_UNSIGNED_INT,&_rightIndices[0] );
		glDrawElements( GL_LINE_STRIP,_leftIndices.size(), GL_UNSIGNED_INT,&_leftIndices[0]  );


		//glBegin(GL_LINE_STRIP);
		//for( size_t i=0; i<_rightBorderPoints.size(); ++i )
		//	glVertex3dv( Math::value_ptr(_rightBorderPoints[i] - o) );
		//glEnd();

		//glBegin(GL_LINE_STRIP);
		//for( size_t i=0; i<_leftBorderPoints.size(); ++i )
		//	glVertex3dv( Math::value_ptr(_leftBorderPoints[i] - o) );
		//glEnd();
	}
	r->endRendering();

}

// ------------------------------------------------------------------------------------------------
void VSplineStrip::drawForSelection( OpenGL::Renderer *r ) const
{
	OpenGL::Selection::Name name( _id );

	r->beginRendering();
	r->setVertex3DArray( _points );

	if( _width > 0 )
	{
		glColor4fv( color() );
		glDrawElements( GL_TRIANGLES,_indices.size(),GL_UNSIGNED_INT,&_indices[0] );
	}

	r->endRendering();

}

// ------------------------------------------------------------------------------------------------
bool VSplineStrip::intersects( const Math::box3 &b ) const 
{
	for( size_t i=0; i<_indices.size(); i+=3 )
	{
		const Math::dvec2 p0( _points[ _indices[i+0] ] );
		const Math::dvec2 p1( _points[ _indices[i+1] ] );
		const Math::dvec2 p2( _points[ _indices[i+2] ] );

		if( Math::triangleBox2DIntersection( p0, p1, p2, b ) )
			return true;
	}

	return false;
}

// ------------------------------------------------------------------------------------------------
std::vector<Math::dvec3> VSplineStrip::getPoints() const
{
	return _points;
}

// ------------------------------------------------------------------------------------------------
VObject* VSplineStrip::clone() const
{
	VSplineStrip* clone= new VSplineStrip;
	this->copyDataIn( clone );

	clone->_points       = _points;
	clone->_indices      = _indices;
	clone->_width        = _width;
	clone->_leftIndices  = _leftIndices;
	clone->_rightIndices = _rightIndices;

	return clone;
}