// Camera.cpp
//
#include "Camera.h"

using namespace s2::Scene;

// ------------------------------------------------------------------------------------------------
void Camera::set( const Math::dvec3& eye, const Math::dvec3& target, const Math::dvec3& up )
{
	_position = eye;
	_target = target;
	_up = Math::normalize( up );
}

// ------------------------------------------------------------------------------------------------
void Camera::set( const Math::dmat4& matrix )
{
	// decompose the matrix orientation
	const Math::dvec4& side = matrix[0];
	const Math::dvec4& up = matrix[1];
	const Math::dvec4& front = matrix[2];

	const double viewDist = 1.0;  // distance from the camera to the target

	_position = matrix[3];
	_up       = Math::dvec3( up );
	_target   = _position - Math::dvec3( front ) * viewDist;

#ifdef _DEBUG
	// todo controlla che checkMatrix == matrix
	auto checkMatrix = Math::inverse( Math::lookAt( _position, _target, _up ) );
	int gg = 0;
#endif // _DEBUG
}

// ------------------------------------------------------------------------------------------------
/**
	the spherical coordinates are with respect to the following reference system:
	axis are aligned to the global reference system
	'origin' is the origin of the reference system
	angles are defined as follow

		  z
		  |
		  |    /|
		  |   / |
		  |  /  |
		  | /p  |
		o |/_)__|______y
		 / \ )  |
		/___\)  |
	   /  t  \  |
	  /       \ |
	 /         \|
	x
*/
void Camera::set( const Math::dvec3& origin, const double theta, const double phi, const double rho )
{
	const Math::dmat4 pivotTrans  = Math::translate( Math::dmat4( 1.0 ), origin );
	const Math::dmat4 startOrient = Math::dmat4( Math::dvec4 { 0,1,0,0 }, Math::dvec4 { 0,0,1,0 },
												 Math::dvec4 { 1,0,0,0 }, Math::dvec4 { 0,0,0,1 } );
	const Math::dmat4 thetaRot = Math::rotate( Math::dmat4( 1.0 ), Math::radians( theta ), { 0.,1.,0. } );
	const Math::dmat4 phiRot   = Math::rotate( Math::dmat4( 1.0 ), Math::radians( -phi ), { 1.,0.,0. } );
	const Math::dmat4 rhoTrans = Math::translate( Math::dmat4( 1.0 ), Math::dvec3( 0, 0, rho ) );

	// set camera direct matrix
	set( pivotTrans * startOrient * thetaRot * phiRot * rhoTrans );
}

// ------------------------------------------------------------------------------------------------
void Camera::setProjectionTransform( const Math::ProjectionTransform& t )
{
	_projection = t;
}


// ------------------------------------------------------------------------------------------------
void Camera::setNearFarPlanes( double nearPlane, double farPlane )
{
	_projection =
		_projection.isOrthographic() 
		? Math::ProjectionTransform::createOrthographic(
			_projection.leftPlane(), _projection.rightPlane(),
			_projection.bottomPlane(), _projection.topPlane(),
			nearPlane, farPlane )
		: Math::ProjectionTransform::createPerspective(
			_projection.aspectRatio(), _projection.fieldOfView(),
			nearPlane, farPlane )
		;
}

// ------------------------------------------------------------------------------------------------
void Camera::setViewport( const Math::irect& viewportRect )
{
	_viewport = viewportRect;
}


// ------------------------------------------------------------------------------------------------
/**
	states if two ViewerPerspective instances are equivalent.
	They cannot be identical member by member
*/
bool Camera::operator==( const Camera& o ) const
{
	return  _viewport.width() == o._viewport.width() &&
		_viewport.height() == o._viewport.height() &&

		_position == o._position &&
		_target == o._target &&
		_up == o._up;
}

// ------------------------------------------------------------------------------------------------
Math::dmat4 Camera::worldToCameraMatrix() const { return Math::lookAt( _position, _target, _up ); }
Math::dmat4 Camera::cameraToWorldMatrix() const { return Math::inverse( worldToCameraMatrix() ); }
Math::dmat4 Camera::orientationMatrix() const
{
	Math::dmat4 m = worldToCameraMatrix();

	m[3][0] = 0.0;
	m[3][1] = 0.0;
	m[3][2] = 0.0;

	return Math::transpose( m ); // quick inverse of lookAt matrix without translation
}

// ------------------------------------------------------------------------------------------------
Math::dvec3 Camera::dirSide()         const { return Math::normalize( Math::cross( _target - _position, _up ) ); }
Math::dvec3 Camera::dirFront()        const { return Math::normalize( _target - _position ); }
Math::dvec3 Camera::position()        const { return _position; }
Math::dvec3 Camera::target()          const { return _target; }
Math::dvec3 Camera::up()              const { return _up; }
Math::irect const& Camera::viewport() const { return _viewport; }

// ------------------------------------------------------------------------------------------------
Math::dray  Camera::rayAt( const Math::dvec2& screenCoord ) const
{
	const Math::dvec3 tn( screenCoord, 0.0 );
	const Math::dvec3 tf( screenCoord, 0.0 );

	const Math::ivec4 vp( _viewport.left(), _viewport.bottom(), _viewport.width(), _viewport.height() );

	const Math::dvec3 n = Math::unProject( tn, worldToCameraMatrix(), _projection.matrix(), vp );
	const Math::dvec3 f = Math::unProject( tf, worldToCameraMatrix(), _projection.matrix(), vp );

	return Math::dray( _position, Math::normalize(f-n) );
}

// ------------------------------------------------------------------------------------------------
Math::dvec3 Camera::worldPoint( const Math::dvec2& screenCoord ) const
{
	return Math::unProject( Math::dvec3( screenCoord, 0.0 ),
							worldToCameraMatrix(),
							_projection.matrix(),

							Math::vec4( _viewport.left(),
							_viewport.bottom(),
							_viewport.width(),
							_viewport.height() )
	);
}

// ------------------------------------------------------------------------------------------------
Math::ivec2 Camera::screenPoint( const Math::dvec3& worldCoord ) const
{
	return Math::project( worldCoord,
						  worldToCameraMatrix(),
						  _projection.matrix(),

						  Math::vec4( _viewport.left(),
						  _viewport.bottom(),
						  _viewport.width(),
						  _viewport.height() )
	);
}

// ------------------------------------------------------------------------------------------------
double Camera::worldSize( int pixels )       const
{
	Math::dvec3 scale;
	Math::dquat rotation;
	Math::dvec3 translation;
	Math::dvec3 skew;
	Math::dvec4 perspective;

	Math::decompose( _projection.matrix(), scale, rotation, translation, skew, perspective ); // @todo_dv: need to decompose modelviewpersp?

	const double horizontalScaleFactor = std::abs( scale.x ) * 0.5; // const double horizontalScaleFactor = _projectionMatrix[0][0] * 0.5; 

	/*
	const auto invProjection = Math::inverse( _projectionMatrix );
	const Math::dvec4 rightLessLeft = invProjection * Math::dvec4( 1.0, 0.0, 0.0, 1.0 ) - invProjection * Math::dvec4( -1.0, 0.0, 0.0, 1.0 );
	const double horizontalScaleFactor = 1 / rightLessLeft.x;
	*/

	return pixels / ( horizontalScaleFactor * _viewport.width() );
}

// ------------------------------------------------------------------------------------------------
int Camera::pixelSize( double worldSize ) const
{
	// project a point at the world origin
	const Math::dvec3 zero = Math::project( Math::dvec3( 0.0 ),
											worldToCameraMatrix(),
											_projection.matrix(),

											Math::vec4( _viewport.left(),
											_viewport.bottom(),
											_viewport.width(),
											_viewport.height() )
	);

	// project a point at the worldSize distance along the x-axis
	const Math::dvec3 out = Math::project( Math::dvec3( worldSize, 0.0, 0.0 ),
										   worldToCameraMatrix(),
										   _projection.matrix(),

										   Math::vec4( _viewport.left(),
										   _viewport.bottom(),
										   _viewport.width(),
										   _viewport.height() )
	);

	// check if the output is valid
	if( Math::any( Math::isnan( out - zero ) ) ||
		Math::any( Math::isinf( out - zero ) ) )
		return 0;

	// return the pixel length of the vector from the world origin to the projected point
	return (int) Math::length( out - zero );
}