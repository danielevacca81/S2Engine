// Camera.cpp
//
#include "Camera.h"

using namespace s2::Scene;

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
// void Camera::set( const Math::dvec3& origin, const double theta, const double phi, const double rho )
// {
// 	const Math::dmat4 pivotTrans  = Math::translate( Math::dmat4( 1.0 ), origin );
// 	const Math::dmat4 startOrient = Math::dmat4( Math::dvec4 { 0,1,0,0 }, Math::dvec4 { 0,0,1,0 },
// 												 Math::dvec4 { 1,0,0,0 }, Math::dvec4 { 0,0,0,1 } );
// 	const Math::dmat4 thetaRot = Math::rotate( Math::dmat4( 1.0 ), Math::radians( theta ), { 0.,1.,0. } );
// 	const Math::dmat4 phiRot   = Math::rotate( Math::dmat4( 1.0 ), Math::radians( -phi ), { 1.,0.,0. } );
// 	const Math::dmat4 rhoTrans = Math::translate( Math::dmat4( 1.0 ), Math::dvec3( 0, 0, rho ) );

// 	// set camera direct matrix
// 	set( pivotTrans * startOrient * thetaRot * phiRot * rhoTrans );
// }