// ViewPerspective.cpp
// 
#include "ViewPerspective.h"


using namespace RenderPipeline;

// ------------------------------------------------------------------------------------------------
ViewPerspective::ViewPerspective()
: _fov( 45.0 )
, _horizontalFov( 45.0 )
, _aspectRatio( 1.0 )
, _nearPlane( 0.1 )
, _farPlane( 1000000.0 )
{}


// ------------------------------------------------------------------------------------------------
void ViewPerspective::set( double aspectRatio, double fovY, double nearPlane, double farPlane )
{
	_fov           = fovY;
	_aspectRatio   = aspectRatio;
	_nearPlane     = nearPlane;
	_farPlane      = farPlane;
	_horizontalFov = Math::degrees( 2.0 * Math::atan( Math::tan( Math::radians( _fov ) * 0.5 ) * _aspectRatio ) );

	_matrix = Math::perspective( Math::radians( _fov ), _aspectRatio, nearPlane, farPlane );
}

// ------------------------------------------------------------------------------------------------
void ViewPerspective::setByFovHorizontal( double aspectRatio, double fovX, double nearPlane, double farPlane )
{
	_aspectRatio   = aspectRatio;
	_nearPlane     = nearPlane;
	_farPlane      = farPlane;
	_horizontalFov = fovX;
	_fov           = Math::degrees( 2.0 * Math::atan( Math::tan( Math::radians( fovX ) * 0.5 ) / _aspectRatio ) ); 

	_matrix = Math::perspective( Math::radians( _fov ), _aspectRatio, nearPlane, farPlane );
}

// ------------------------------------------------------------------------------------------------
void ViewPerspective::setAspectRatio( double aspectRatio )           { set( aspectRatio, _fov, _nearPlane, _farPlane ); }
void ViewPerspective::setFieldOfView( double fov )                   { set( _aspectRatio, fov, _nearPlane, _farPlane ); }
void ViewPerspective::setFieldOfViewHorizontal( double fov )         { setByFovHorizontal( _aspectRatio, fov, _nearPlane, _farPlane ); }
void ViewPerspective::setPlanes( double nearPlane, double farPlane ) { set( _aspectRatio, _fov, nearPlane, farPlane ); }

// ------------------------------------------------------------------------------------------------
Math::dmat4 ViewPerspective::matrix()                const { return _matrix; }
double      ViewPerspective::aspectRatio()           const { return _aspectRatio; }
double      ViewPerspective::fieldOfView()           const { return _fov; }
double      ViewPerspective::fieldOfViewHorizontal() const { return _horizontalFov; }
double      ViewPerspective::nearPlane()             const { return _nearPlane; }
double      ViewPerspective::farPlane()              const { return _farPlane; }

// ------------------------------------------------------------------------------------------------
//Math::dvec3 View::computeEyePosition( const Math::dmat4 &m ) const
//{
//	// Get the 3 basis vector planes at the camera origin and transform them into model space.
//	//  
//	// NOTE: Planes have to be transformed by the inverse transpose of a matrix
//	//       Nice reference here: http://www.opengl.org/discussion_boards/showthread.php/159564-Clever-way-to-transform-plane-by-matrix
//	//
//	//       So for a transform to model space we need to do:
//	//            inverse(transpose(inverse(MV)))
//	//       This equals : transpose(MV) - see Lemma 5 in http://mathrefresher.blogspot.com.au/2007/06/transpose-of-matrix.html
//	//
//	// As each plane is simply (1,0,0,0), (0,1,0,0), (0,0,1,0) we can pull the data directly from the transpose matrix.
//	//  
//	Math::dmat4 mT = Math::transpose(m);
//
//	// Get plane normals 
//	Math::dvec3 n1(mT[0]);
//	Math::dvec3 n2(mT[1]);
//	Math::dvec3 n3(mT[2]);
//
//	// Get plane distances
//	const double d1(mT[0].w);
//	const double d2(mT[1].w);
//	const double d3(mT[2].w);
//
//	// Get the intersection of these 3 planes
//	// http://paulbourke.net/geometry/3planes/
//	Math::dvec3 n2n3 = Math::cross(n2, n3);
//	Math::dvec3 n3n1 = Math::cross(n3, n1);
//	Math::dvec3 n1n2 = Math::cross(n1, n2);
//
//	const Math::dvec3 top = (n2n3 * d1) + (n3n1 * d2) + (n1n2 * d3);
//	double denom = Math::dot(n1, n2n3);
//
//	return top / -denom;
//#if 0
//	// Get the intersection of these 3 planes 
//	// (using math from RealTime Collision Detection by Christer Ericson)
//	Math::dvec3 n2n3   = Math::cross(n2, n3);
//	const double denom = Math::dot(n1, n3);
//
//	Math::dvec3 top = (n2n3 * d1) + Math::cross(n1, (d3*n2) - (d2*n3));
//	return top / -denom;
//#endif
//}