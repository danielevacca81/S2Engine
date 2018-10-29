// Manipulator.cpp
// 
#include "Manipulator.h"

using namespace OpenGL;

// ------------------------------------------------------------------------------------------------
Manipulator::Manipulator()
: _azimuth(0.0)
, _elevation(0.0)
, _size(1,1)
{}

// ------------------------------------------------------------------------------------------------
Manipulator::~Manipulator()
{}

// ------------------------------------------------------------------------------------------------
void Manipulator::update( const Math::ivec2 &deltaScreenPos )
{
	const double eps= 120./_size.y;

	_azimuth   += eps * deltaScreenPos.x;
	_elevation += eps * deltaScreenPos.y;

	if( _elevation < -90. ) _elevation= -90.;
	if( _elevation > +90. ) _elevation= +90.;

	if( _azimuth < -90. ) _azimuth= -90.;
	if( _azimuth > +90. ) _azimuth= +90.;
}

// ------------------------------------------------------------------------------------------------
void Manipulator::resize( int pixelWidth,int pixelHeight )
{
	_size = Math::ivec2( pixelWidth,pixelHeight );
}

// ------------------------------------------------------------------------------------------------
void Manipulator::setOrientation( const Math::dmat4 &orientation )
{}

// ------------------------------------------------------------------------------------------------
void Manipulator::reset()
{
	_azimuth   = 0.0;
	_elevation = 0.0;
}


// ------------------------------------------------------------------------------------------------
Math::dmat4 Manipulator::matrix() const
{
	Math::dmat4 el = Math::rotate( Math::dmat4(1), _elevation-90, Math::dvec3(1,0,0) );
	Math::dmat4 az = Math::rotate( Math::dmat4(1), _azimuth-90, Math::dvec3(0,0,1) );
	
	return az*el;
	//glRotatef( elevation-90, 1, 0, 0 );
	//glRotatef( azimuth-90, 0, 0, 1 );
}