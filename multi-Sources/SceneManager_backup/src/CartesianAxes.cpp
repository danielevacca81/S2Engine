// CartesianAxes.cpp
//
#include "CartesianAxes.h"

#include "Core/Intersection.h"

#include <iostream>

using namespace Scene;

// ------------------------------------------------------------------------------------------------
CartesianAxes::CartesianAxes()
: _gridXY( false )
, _gridXZ( false )
, _gridYZ( false )
{}


// ------------------------------------------------------------------------------------------------
void CartesianAxes::setXAxis( const Math::dvec3 &p0, const Math::dvec3 &p1, double minValue, double maxValue, bool minLabelVisible, bool maxLabelVisible/*, double tickDistance = 1.0 */ )
{
	_xAxis.setLine( p0, p1 );
	_xAxis.setValues( minValue, maxValue );
	//x.setTickDistance(tickDistance);
	_xAxis.setBoundaryVisible( minLabelVisible, maxLabelVisible );
}

// ------------------------------------------------------------------------------------------------
void CartesianAxes::setYAxis( const Math::dvec3 &p0, const Math::dvec3 &p1, double minValue, double maxValue, bool minLabelVisible, bool maxLabelVisible/*, double tickDistance = 1.0 */ )
{
	_yAxis.setLine( p0, p1 );
	_yAxis.setValues( minValue, maxValue );
	//y.setTickDistance(tickDistance);
	_yAxis.setBoundaryVisible( minLabelVisible, maxLabelVisible );
}

// ------------------------------------------------------------------------------------------------
void CartesianAxes::setZAxis( const Math::dvec3 &p0, const Math::dvec3 &p1, double minValue, double maxValue, bool minLabelVisible, bool maxLabelVisible/*, double tickDistance = 1.0 */ )
{
	_zAxis.setLine( p0, p1 );
	_zAxis.setValues( minValue, maxValue );
	//z.setTickDistance(tickDistance);
	_zAxis.setBoundaryVisible( minLabelVisible, maxLabelVisible );
}

// ------------------------------------------------------------------------------------------------
//void CartesianAxes::setFonts(Font *f, float fontSize)
//{
//    _fonts = f;
//    _fontSize = fontSize;
//
//    _xAxis.setFonts(f); _xAxis.setTextSize(fontSize);
//    _yAxis.setFonts(f); _yAxis.setTextSize(fontSize);
//    _zAxis.setFonts(f); _zAxis.setTextSize(fontSize);
//}

// ------------------------------------------------------------------------------------------------
void CartesianAxes::setVisible( bool on )
{
	_visible = on;
}

// ------------------------------------------------------------------------------------------------
void CartesianAxes::update( const Math::box3 &visibleArea, double pixelSize )
{
	if( !_visible )
		return;

	if( Math::lineBoxIntersection( _xAxis.minPoint(), _xAxis.maxPoint(), visibleArea )
		|| visibleArea.contains( _xAxis.minPoint() )
		|| visibleArea.contains( _xAxis.maxPoint() ) )
		_xAxis.update( pixelSize );

	if( Math::lineBoxIntersection( _yAxis.minPoint(), _yAxis.maxPoint(), visibleArea )
		|| visibleArea.contains( _yAxis.minPoint() )
		|| visibleArea.contains( _yAxis.maxPoint() ) )
		_yAxis.update( pixelSize );

	//if( lineBoxIntersection(z.minPoint(),z.maxPoint(),visibleArea )    z.update(pixelSize);
	//z.update();
}

// ------------------------------------------------------------------------------------------------
void CartesianAxes::update()
{
	_xAxis.update();
	_yAxis.update();
	//_zAxis.update( pixelSize );
}

// ------------------------------------------------------------------------------------------------
void CartesianAxes::draw( const Renderer::SurfacePtr &surface, const Renderer::DrawingState &ds ) const
{
	if( !_visible )
		return;

	_xAxis.draw( surface, ds );
	_yAxis.draw( surface, ds );
	//_zAxis.draw( renderer );


	//if( _gridXY || _gridXZ || _gridYZ )
	//{
	//	glPushAttrib( GL_LINE_BIT | GL_ENABLE_BIT );

	//	glEnable( GL_LINE_STIPPLE );
	//	glEnable( GL_BLEND );
	//	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	//	glColor4ub(0xff,0xff,0xff,0xff);
	//	glLineStipple(1,0x00ff);
	//	glLineWidth(1.2f);

	//	drawXYGrid();
	//	drawXZGrid();
	//	drawYZGrid();

	//	glPopAttrib();
	//}
}


// ------------------------------------------------------------------------------------------------
void CartesianAxes::drawXYGrid()
{
	if( !_gridXY )
		return;

	const std::vector<Math::dvec3> xP = _xAxis.getTickPositions();
	const Math::dvec3 xN              = _xAxis.getOrthoDir();

	const std::vector<Math::dvec3> yP = _yAxis.getTickPositions();
	const Math::dvec3 yN              = _yAxis.getOrthoDir();

	const Math::dvec3 xT              = xN * _yAxis.length();
	const Math::dvec3 yT              = yN * _xAxis.length();

	//// lines on x axis (skipping start and end point)
	//glBegin( GL_LINES );
	//for( size_t i=0; i<xP.size(); ++i )
	//{
	//		glVertex3dv( Math::value_ptr(xP[i]+xT) );
	//		glVertex3dv( Math::value_ptr(xP[i]) );
	//}
	//glEnd();		

	//// lines on y axis (skipping start and end point)
	//glBegin( GL_LINES );
	//for( size_t i=0; i<yP.size(); ++i )
	//{
	//		glVertex3dv( Math::value_ptr(yP[i]+yT) );
	//		glVertex3dv( Math::value_ptr(yP[i]) );
	//}
	//glEnd();
}

// ------------------------------------------------------------------------------------------------
void CartesianAxes::drawXZGrid()
{}

// ------------------------------------------------------------------------------------------------
void CartesianAxes::drawYZGrid()
{}