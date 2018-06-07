// View.cpp
// 
#include "View.h"

using namespace s2;

// ------------------------------------------------------------------------------------------------
View::View()
//: _eye                      (0.0, 0.0, 1.0)
//, _target                   (0.0, 0.0, 0.0)
//, _up                       (0.0, 1.0, 0.0)
: _fovY                     ( 45.0 )
, _aspectRatio              ( 1.0  )
, _perspectiveNearDistance  ( 0.01 )
, _perspectiveFarDistance   ( 64   )
, _orthographicNearDistance ( 0.0  )
, _orthographicFarDistance  ( 1.0  )
, _orthographicLeft         ( 0.0  )
, _orthographicRight        ( 1.0  )
, _orthographicBottom       ( 0.0  )
, _orthographicTop          ( 1.0  )
{}

// ------------------------------------------------------------------------------------------------
View::~View()
{}

// ------------------------------------------------------------------------------------------------
//void View::transform( const Math::dmat4 &m )
//{
//	_up     = Math::dvec3( m * Math::dvec4(_up,1.0)     );
//	_eye    = Math::dvec3( m * Math::dvec4(_eye,1.0)    );
//	_target = Math::dvec3( m * Math::dvec4(_target,1.0) );
//
//	////const Math::dvec3 viewDir = Math::dvec3( m[2][0], m[2][1], m[2][2] );
//	//const Math::dvec3 viewDir = Math::dvec3( m[2] );
//
//	//_up     = Math::dvec3( m[1] );
//	//_eye    = computeEyePosition( m );
//	//_target = _eye + viewDir;
//}

// ------------------------------------------------------------------------------------------------
void View::set( const Math::dmat4 &m )
{
	//Math::dvec4 r = m[0]; // right
	//Math::dvec4 u = m[1]; // up
	//Math::dvec4 f = m[2]; // forward

	//_eye    = computeEyePosition(m);
	//_up     = Math::dvec3(u);
	//_target = _eye+Math::dvec3(f);

	_matrix = m;
}


// ------------------------------------------------------------------------------------------------
void View::set( const Math::dvec3 &eye, const Math::dvec3 &target, const Math::dvec3 &up )	
{ 
	_matrix = Math::lookAt( eye,target,up );

	//_eye    = eye;
	//_target = target;
	//_up     = up;
}

// ------------------------------------------------------------------------------------------------
void View::setPerspective( double aspectRatio, double fovY, double nearPlane, double farPlane )
{
	_aspectRatio             = aspectRatio;
	_fovY                    = fovY;
	_perspectiveNearDistance = nearPlane;
	_perspectiveFarDistance  = farPlane;
}

void View::setAspectRatio( double aspectRatio ) { _aspectRatio = aspectRatio; }
void View::setFieldOfViewY( double fovY )       { _fovY        = fovY; }
void View::setPerspectivePlanes( double nearPlane, double farPlane )
{
	_perspectiveNearDistance = nearPlane;
	_perspectiveFarDistance  = farPlane;
}

// ------------------------------------------------------------------------------------------------
void View::setOrthographic( double left, double right, double bottom, double top, double nearPlane, double farPlane ) 
{ 
	_orthographicLeft         = left;
	_orthographicRight        = right;
	_orthographicBottom       = bottom;
	_orthographicTop          = top;
	_orthographicNearDistance = nearPlane;
	_orthographicFarDistance  = farPlane;
}

// ------------------------------------------------------------------------------------------------
//double View::pixelSize( const Math::dvec3 &p ) const
//{
//}

// ------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------
//void View::zoomToTarget( double targetRadius )
//{
//	Math::dvec3 toEye     = Math::normalize(_eye - _target);
//	const double s        = Math::sin( Math::min( fieldOfViewX(), fieldOfViewY() ) * 0.5);
//	const double distance = (targetRadius / s);
//	_eye                  = _target + (distance * toEye);	
//}

// ------------------------------------------------------------------------------------------------
//void View::zoomIn( double zf )  { _zoomFactor *= zf; }
//void View::zoomOut( double zf ) {_zoomFactor *= zf;  }

// ------------------------------------------------------------------------------------------------
Math::dvec3 View::computeEyePosition( const Math::dmat4 &m ) const
{
	// Get the 3 basis vector planes at the camera origin and transform them into model space.
	//  
	// NOTE: Planes have to be transformed by the inverse transpose of a matrix
	//       Nice reference here: http://www.opengl.org/discussion_boards/showthread.php/159564-Clever-way-to-transform-plane-by-matrix
	//
	//       So for a transform to model space we need to do:
	//            inverse(transpose(inverse(MV)))
	//       This equals : transpose(MV) - see Lemma 5 in http://mathrefresher.blogspot.com.au/2007/06/transpose-of-matrix.html
	//
	// As each plane is simply (1,0,0,0), (0,1,0,0), (0,0,1,0) we can pull the data directly from the transpose matrix.
	//  
	Math::dmat4 mT = Math::transpose(m);

	// Get plane normals 
	Math::dvec3 n1(mT[0]);
	Math::dvec3 n2(mT[1]);
	Math::dvec3 n3(mT[2]);

	// Get plane distances
	const double d1(mT[0].w);
	const double d2(mT[1].w);
	const double d3(mT[2].w);

	// Get the intersection of these 3 planes
	// http://paulbourke.net/geometry/3planes/
	Math::dvec3 n2n3 = Math::cross(n2, n3);
	Math::dvec3 n3n1 = Math::cross(n3, n1);
	Math::dvec3 n1n2 = Math::cross(n1, n2);

	const Math::dvec3 top = (n2n3 * d1) + (n3n1 * d2) + (n1n2 * d3);
	double denom = Math::dot(n1, n2n3);

	return top / -denom;
#if 0
	// Get the intersection of these 3 planes 
	// (using math from RealTime Collision Detection by Christer Ericson)
	Math::dvec3 n2n3   = Math::cross(n2, n3);
	const double denom = Math::dot(n1, n3);

	Math::dvec3 top = (n2n3 * d1) + Math::cross(n1, (d3*n2) - (d2*n3));
	return top / -denom;
#endif
}