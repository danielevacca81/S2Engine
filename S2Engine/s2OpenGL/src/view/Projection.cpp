// Projection.cpp
// 
#include "Projection.h"

using namespace OpenGL;

// ------------------------------------------------------------------------------------------------
Projection::Projection()
: _farClip(1.0)
, _fov(0.0)
, _nearClip(-1.0)
{}

// ------------------------------------------------------------------------------------------------
Projection::~Projection()
{}

// ------------------------------------------------------------------------------------------------
void Projection::setPerspective( double fovY, double aspect, double zNear, double zFar )
{
	const double tangent = Math::tan( Math::radians(fovY*0.5) );
	const double height  = zNear * tangent;
	const double width   = height * aspect;

	_matrix   = Math::frustum( -width,width,-height,height, zNear, zFar );
	_fov      = fovY;
	_nearClip = zNear;
	_farClip  = zFar;
}

// ------------------------------------------------------------------------------------------------
void Projection::setOrthographic( double left, double right, double bottom, double top, double zNear, double zFar )
{
	_matrix   = Math::ortho( left,right, bottom,top, zNear,zFar );
	_fov      =  0.0;
	_nearClip = zNear;
	_farClip  = zFar;
}