// ProjectionTransform.cpp
// 
#include "ProjectionTransform.h"


namespace glm
{

// ------------------------------------------------------------------------------------------------
ProjectionTransform ProjectionTransform::createPerspective( double aspectRatio, double fovY, double nearPlane, double farPlane )
{
	ProjectionTransform pt;		
	{
		pt._matrix       = Math::perspective( Math::radians( fovY ), aspectRatio, nearPlane, farPlane );
		pt._aspectRatio  = aspectRatio;
		pt._fov          = fovY;
		pt._fovHorizonal = Math::degrees( 2.0 * Math::atan( Math::tan( Math::radians( fovY ) * 0.5 ) * aspectRatio ) );
		pt._nearPlane    = nearPlane;
		pt._farPlane     = farPlane;
	};
	return pt;
}

// ------------------------------------------------------------------------------------------------
ProjectionTransform ProjectionTransform::createPerspectiveByFovHorizontal( double aspectRatio, double fovX, double nearPlane, double farPlane )
{
	ProjectionTransform pt;
	{
		pt._matrix       = Math::perspective( Math::radians( fovX ), aspectRatio, nearPlane, farPlane );
		pt._aspectRatio  = aspectRatio;
		pt._fov          = Math::degrees( 2.0 * Math::atan( Math::tan( Math::radians( fovX ) * 0.5 ) / aspectRatio ) );
		pt._fovHorizonal = fovX;
		pt._nearPlane    = nearPlane;
		pt._farPlane     = farPlane;
	};
	return pt;

}

// ------------------------------------------------------------------------------------------------
ProjectionTransform ProjectionTransform::createOrthographic( double left, double right, double bottom, double top, double nearPlane, double farPlane )
{
	ProjectionTransform pt;
	{
		pt._matrix       = Math::ortho( left, right, bottom, top, nearPlane, farPlane );
		pt._aspectRatio  = ( right - left ) / ( top - bottom );
		pt._fov          = 90.0; // orthographic projection does not have a field of view, but we set it to 90 degrees.
		pt._fovHorizonal = 90.0; // orthographic projection does not have a field of view, but we set it to 90 degrees.
		pt._leftPlane    = left;
		pt._rightPlane   = right;
		pt._bottomPlane  = bottom;
		pt._topPlane     = top;
		pt._nearPlane    = nearPlane;
		pt._farPlane     = farPlane;
	};
	return pt;
}

// ------------------------------------------------------------------------------------------------
ProjectionTransform ProjectionTransform::createOrthographic( int w, int h, double nearPlane, double farPlane, double zoomFactor )
{
	if( zoomFactor == 0.0 )
		zoomFactor = 1.0; // default zoom factor

	const double left   = -0.5 * w / zoomFactor;
	const double right  = 0.5 * w / zoomFactor;
	const double bottom = -0.5 * h / zoomFactor;
	const double top    = 0.5 * h / zoomFactor;

	ProjectionTransform pt;
	{
		// @todo: some terms can be computed from the matrix itself (i.e. fov, aspect).
		pt._matrix       = Math::ortho( left, right, bottom, top, nearPlane, farPlane );
		pt._aspectRatio  = ( right - left ) / ( top - bottom );
		pt._fov          = 90.0; // orthographic projection does not have a field of view, but we set it to 90 degrees.
		pt._fovHorizonal = 90.0; // orthographic projection does not have a field of view, but we set it to 90 degrees.
		pt._leftPlane    = left;
		pt._rightPlane   = right;
		pt._bottomPlane  = bottom;
		pt._topPlane     = top;
		pt._nearPlane    = nearPlane;
		pt._farPlane     = farPlane;
	};
	return pt;
}

// ------------------------------------------------------------------------------------------------
ProjectionTransform ProjectionTransform::createScreenSpaceOrthographic( int w, int h, double nearPlane /*= -50.0*/, double farPlane /*= 50.0*/ )
{
	return createOrthographic( 0.0, (double) w, 0.0, (double) h, nearPlane, farPlane );
}

// ------------------------------------------------------------------------------------------------
double      ProjectionTransform::aspectRatio()           const { return _aspectRatio;}
double      ProjectionTransform::fieldOfView()           const { return _fov; }
double      ProjectionTransform::fieldOfViewHorizontal() const { return _fovHorizonal; }
double      ProjectionTransform::nearPlane()             const { return _nearPlane; }
double      ProjectionTransform::farPlane()              const { return _farPlane; }
double      ProjectionTransform::leftPlane()             const { return _leftPlane; }
double      ProjectionTransform::rightPlane()            const { return _rightPlane; }
double      ProjectionTransform::bottomPlane()           const { return _bottomPlane; }
double      ProjectionTransform::topPlane()              const { return _topPlane; }
Math::dmat4 ProjectionTransform::matrix()                const { return _matrix; }

// ------------------------------------------------------------------------------------------------
bool ProjectionTransform::isOrthographic() const 
{
	return Math::all( Math::equal( _matrix[3], Math::dvec4( 0.0, 0.0, 0.0, 1.0 ) ) );
}

// ------------------------------------------------------------------------------------------------
bool ProjectionTransform::isPerspective() const 
{
	return !isOrthographic();
}

}