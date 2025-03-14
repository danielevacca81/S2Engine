// Viewer.cpp
//
#include "Viewer.h"

#include "Math/Plane.h"
#include "Math/Ray.h"
#include "Math/Math.h"

#include <glm/ext/matrix_transform.hpp>

using namespace RenderPipeline;


// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
void Viewer::setCameraDirectMatrix( const Math::dmat4 &matrix )
{
	_camera.set( matrix[3], matrix );
	applyConstraints();

#ifdef _DEBUG
	// todo controlla che checkMatrix == matrix
	auto checkMatrix = Math::inverse( _camera.matrix() );
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
void Viewer::setLocalSphericalCoordinate( const Math::dvec3 &origin,
										  const double theta, const double phi, const double rho )
{
	const Math::dmat4 pivotTrans = Math::translate( Math::dmat4( 1.0 ), origin );
	const Math::dmat4 startOrient = Math::dmat4( Math::dvec4{0,1,0,0}, Math::dvec4{0,0,1,0}, 
												 Math::dvec4{1,0,0,0}, Math::dvec4{0,0,0,1} ); 
	const Math::dmat4 thetaRot   = Math::rotate( Math::dmat4( 1.0 ), Math::radians(theta), {0.,1.,0.} );
	const Math::dmat4 phiRot     = Math::rotate( Math::dmat4( 1.0 ), Math::radians(-phi),  {1.,0.,0.} );
	const Math::dmat4 rhoTrans   = Math::translate( Math::dmat4( 1.0 ), Math::dvec3(0,0,rho) );

	setCameraDirectMatrix( pivotTrans * startOrient * thetaRot * phiRot * rhoTrans );
}

// ------------------------------------------------------------------------------------------------
Math::dvec2 Viewer::viewportSize() const
{
	return Math::dvec2( _viewport.width(), _viewport.height() );
}

// ------------------------------------------------------------------------------------------------
/**
	states if two ViewerPerspective instances are equivalent.
	They cannot be identical member by member
*/
bool Viewer::equals( const Viewer& o ) const
{
	return  _viewport.width()  == o._viewport.width()  &&
			_viewport.height() == o._viewport.height() &&

			_camera.position() == o._camera.position() &&	
			_camera.target()   == o._camera.target()   &&	  // todo in verita' basterebbe che i target siano allineati
			_camera.up()       == o._camera.up()       ;
}

// ------------------------------------------------------------------------------------------------
void Viewer::applyConstraints()
{
	if( _applyConstraintsInProgress )
		return;

	_applyConstraintsInProgress = true;

	for( auto &c : _constraints )
		c( this );

	_applyConstraintsInProgress = false;
}


// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------
View ViewerOrthographic::view() const
{
	return View( viewport(), Math::dmat4(1.0), camera().matrix(), _ortho.matrix() );
}

// ------------------------------------------------------------------------------------------------
/**
	states if two ViewerPerspective instances are equivalent.
	They cannot be identical member by member
*/
bool ViewerOrthographic::equals( const ViewerOrthographic& o ) const
{
	return  Viewer::equals( o ) &&
		_ortho.nearPlane()   == o._ortho.nearPlane()   &&
		_ortho.farPlane()    == o._ortho.farPlane()    &&
		_ortho.leftPlane()   == o._ortho.leftPlane()   &&
		_ortho.rightPlane()  == o._ortho.rightPlane()  &&
		_ortho.bottomPlane() == o._ortho.bottomPlane() &&
		_ortho.topPlane()    == o._ortho.topPlane()    ;
}


// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------
void Viewer2D::setZoomFactor( double factor )
{
	_ortho.set( _viewport.width(), _viewport.height(), -50.0, 50.0, factor );
	applyConstraints();
}

// ------------------------------------------------------------------------------------------------
bool Viewer2D::fitToBox( const Math::dbox3 &b ) // b is in world space
{
	const auto w = viewportSize().x;
	const auto h = viewportSize().y;

	if( w <= 0 || h <= 0 || b.sizeX() <= 0.0 || b.sizeY() <= 0.0 )
		return false;

	auto rotBB(b);
	rotBB.rotate( _camera.orientation() );


	const double aspect = w / (double) h;
	const double bAspect = rotBB.sizeX() / rotBB.sizeY();

	const double newW = ( aspect > bAspect ) ? aspect * rotBB.sizeY() : rotBB.sizeX();

	/*auto cam = camera();
	cam.moveToLookAt( b.center() );
	setCamera( cam );*/
	Math::dvec3 center = rotBB.center(); center.z = 0.0;
	//_camera.lookAt( center + Math::dvec3(0.0, 0.0, 1.0),  center, Math::dvec3(0.0, 1.0, 0.0) );
	_camera.moveToLookAt( center ); // set new camera position without changing its orientation

	setZoomFactor( 2.0 / newW );
	return true;
}

// ------------------------------------------------------------------------------------------------
bool Viewer2D::fillByBox( const Math::dbox3 &b )
{
	const auto w = viewportSize().x;
	const auto h = viewportSize().y;

	if( w <= 0 || h <= 0 || b.sizeX() <= 0.0 || b.sizeY() <= 0.0 )
		return false;

	const double aspect = w / (double) h;
	const double bAspect = b.sizeX() / b.sizeY();

	const double newW = ( aspect < bAspect ) ? aspect * b.sizeY() : b.sizeX();

	/*auto cam = camera();
	cam.moveToLookAt( b.center() );
	setCamera( cam );*/
	Math::dvec3 center = b.center(); center.z = 0.0;
	_camera.lookAt( Math::dvec3( center.x, center.y, _camera.position().z ), center, Math::dvec3(0.0, 1.0, 0.0) );

	setZoomFactor( 2.0 / newW );
	return true;
}

// ------------------------------------------------------------------------------------------------
Math::dbox3 Viewer2D::viewBox() const
{
	Math::dbox3 retval( Math::dvec3( _ortho.leftPlane(), _ortho.bottomPlane(), _ortho.nearPlane() ),
					    Math::dvec3( _ortho.rightPlane(), _ortho.topPlane(), _ortho.farPlane() ) );
	
	retval.translate( _camera.position() );
	return retval;
}

// ------------------------------------------------------------------------------------------------
Math::dbox3 Viewer2D::rotatedViewBox() const
{
	Math::dbox3 retval( Math::dvec3( _ortho.leftPlane(), _ortho.bottomPlane(), _ortho.nearPlane() ),
					   Math::dvec3( _ortho.rightPlane(), _ortho.topPlane(), _ortho.farPlane() ) );
	
	retval = retval.rotate( _camera.orientation() );
	retval.translate( _camera.position() );
	return retval;
}

// ------------------------------------------------------------------------------------------------
Math::dbox3 Viewer2D::viewAABB() const
{
	Math::dbox3 localBox( Math::dvec3( _ortho.leftPlane(), _ortho.bottomPlane(), _ortho.nearPlane() ),
					      Math::dvec3( _ortho.rightPlane(), _ortho.topPlane(), _ortho.farPlane() ) );

	// localBox = localBox.rotated( _camera.orientation() ); // considerare l'orientazione della camera ?
	
	Math::dmat4 cameraMarix = cameraDirectMatrix();

	Math::dbox3 retval;
	for( int i= 0; i< 8; ++i )
	{
		const Math::dvec4 p = {localBox.corner( i ), 1.0 };
		const Math::dvec3 globalP = cameraMarix * p;
		if( i==0 )
			retval = { globalP, globalP };
		else
			retval.extend( globalP );
	}

	return retval;
}

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------
void ViewerPerspective::setViewportAndFov( const int width, const int height, const FovId fovId, const double fov )
{
	_viewport = Math::irect( 0, 0 , width, height );

	auto theFovId = fovId;
	if( fovId == FovId::Min )
		theFovId = width < height ? FovId::Horizontal : FovId::Vertical;
	else if( fovId == FovId::Max )
		theFovId = width > height ? FovId::Horizontal : FovId::Vertical;

	const double ar = double(width) / height;

	if( theFovId == FovId::Horizontal )
		_perspective.setByFovHorizontal( ar, fov, _perspective.nearPlane(), _perspective.farPlane() );
	else
		_perspective.set( ar, fov, _perspective.nearPlane(), _perspective.farPlane() );

	applyConstraints();
}

// ------------------------------------------------------------------------------------------------
View ViewerPerspective::view() const
{
	View retval;

	retval.setViewport( viewport() );
	retval.setViewMatrix( camera().matrix() );
	retval.setProjectionMatrix( _perspective.matrix() );

	return retval;
}

// ------------------------------------------------------------------------------------------------
double ViewerPerspective::focalLengthPixel() const
{
	return (_viewport.height()*0.5) / Math::tan( Math::radians(_perspective.fieldOfView()*0.5) );
}

// ------------------------------------------------------------------------------------------------
/**
	states if two ViewerPerspective instances are equivalent.
	They cannot be identical member by member
*/
bool ViewerPerspective::equals( const ViewerPerspective& o ) const
{
	return  Viewer::equals( o ) &&
		_perspective.aspectRatio() == o._perspective.aspectRatio() &&
		_perspective.fieldOfView() == o._perspective.fieldOfView() &&		
		_perspective.nearPlane()   == o._perspective.nearPlane()   &&
		_perspective.farPlane()    == o._perspective.farPlane()     ;
}


// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------
void Viewer3D::setZoomFactor( double factor )
{
	Math::dvec3 depth0WorldPoint;
	bool depth0WorldPointOk = depth0PlaneAtScreenPos( viewportLocalCenter(), depth0WorldPoint );  
	if( depth0WorldPointOk )
	{
		const double worldPointDist = Math::distance( depth0WorldPoint, _camera.position() );
		const double currZoomFactor = ( 2.0 * focalLengthPixel()) / (_viewport.width() * worldPointDist);

		const double newWorldPointDist = (currZoomFactor * worldPointDist) / factor;

		_camera.set( depth0WorldPoint - newWorldPointDist*_camera.front(), _camera.orientation() );
	}
	else
	{
		const double dist = Math::abs( _camera.position().z );
		const double currZoomFactor = ( 2.0 * focalLengthPixel()) / (_viewport.width() * dist);

		double newDist = (currZoomFactor * dist) / factor;
		newDist = dist - newDist;

		_camera.set( _camera.position() + newDist*_camera.front(), _camera.orientation() );
	}

	applyConstraints();
}

// ------------------------------------------------------------------------------------------------
void Viewer3D::setCameraTopDownAdaptToBox( const Math::dbox3 &b, const BoxAdaptationPolicy &adaptPolicy )
{
	const auto trueBox = trueBoxByAdaptationPolicy( b, adaptPolicy );

	const double halfBoxSizeY = trueBox.sizeY() * 0.5;
	const double halfFovYDeg  = _perspective.fieldOfView() * 0.5;

	const double cameraH = halfBoxSizeY / Math::tan( Math::radians(halfFovYDeg) );

	const Math::dvec3 target = trueBox.center();
	Math::dvec3 cameraPos( target.x, target.y, /*target.z +*/ cameraH );

	_camera = Camera( cameraPos, target, {0., 1., 0.} );

	applyConstraints();
}

// -------------------------------------------------------------------------------------
double Viewer3D::zoomFactor() const
{
	Math::dvec3 depth0WorldPoint;
	const bool depth0WorldPointOk = depth0PlaneAtScreenPos( viewportLocalCenter(), depth0WorldPoint ); 
	if( depth0WorldPointOk )
	{
		const auto worldPointDist = Math::distance( depth0WorldPoint, _camera.position() );
		return ( 2.0 * focalLengthPixel()) / (_viewport.width() * worldPointDist);
	}
	else
	{
		const auto dist = Math::abs( _camera.position().z );
		return ( 2.0 * focalLengthPixel()) / (_viewport.width() * dist);
	}
}

// -------------------------------------------------------------------------------------
double Viewer3D::zoomDist( const double zoomFactor ) const
{
	return ( 2.0 * focalLengthPixel()) / (_viewport.width() * zoomFactor);
}

// -------------------------------------------------------------------------------------
bool Viewer3D::depth0PlaneAtScreenPos( const Math::ivec2 &screenCoord, Math::dvec3 &seaPlaneWorldPoint ) const
{
	const Math::dvec2 pixelIsAreaCoord( screenCoord.x + 0.5, screenCoord.y + 0.5 );
	return depth0PlaneAtScreenPos( pixelIsAreaCoord, seaPlaneWorldPoint );  
}

// ------------------------------------------------------------------------------------------------
bool Viewer3D::depth0PlaneAtScreenPos( const Math::dvec2 &screenCoord, Math::dvec3 &depth0WorldPoint ) const
{
	const auto theView = view();
	const auto point3D = Math::unProject( Math::dvec3( screenCoord, 0.0 ),
										  theView.viewMatrix(),
										  theView.projectionMatrix(),
										  Math::dvec4( _viewport.left(), _viewport.bottom(), _viewport.width(), _viewport.height() ) );

	static const Math::dplane depth0Plane( Math::dvec3(0,0,1), 0.0 );
	const auto cameraPos = _camera.position();
	const Math::dray raySight( cameraPos, point3D-cameraPos );

	double t;
	if( !raySight.intersects( depth0Plane, t ) )
		return false;

	depth0WorldPoint = raySight.pointAt( t );
	return true;
}

// ------------------------------------------------------------------------------------------------
/**
	\see setLocalSphericalCoordinate
*/
bool Viewer3D::depth0LocalSphericalCoordinate( Math::dvec3 &depth0Origin, 
											   double &theta, double &phi, double &rho ) const
{
	if( !depth0PlaneAtScreenPos( viewportLocalCenter(), depth0Origin )  )
		return false;

	rho = Math::length( _camera.position() - depth0Origin );	

	Math::dvec3 cDir = Math::normalize( -_camera.front() );	 // todo da rivedere e migliorare
	if( cDir.y != 0.0 || cDir.x != 0.0 )
		theta = Math::degrees( std::atan2( cDir.y, cDir.x ) );
	else
	{
		Math::dvec3 side = _camera.side();
		theta = Math::degrees( std::atan2( side.y, side.x ) ) - 90.0;
	}
	theta = Math::normalizeAngleDeg( theta );

	phi = Math::degrees( std::atan2( cDir.z, std::hypot(cDir.x,cDir.y) ) );
	return true;
}

// ------------------------------------------------------------------------------------------------
inline Math::dbox3 Viewer3D::trueBoxByAdaptationPolicy( const Math::dbox3 &b,
													   const BoxAdaptationPolicy adaptPolicy ) const
{
	const Math::dvec3 bSizes ( b.sizeX(),    b.sizeY(),    0.0 );
	const Math::dvec3 bCenter( b.center().x, b.center().y, 0.0 );

	const double wAspect = _viewport.width()  / double( _viewport.height() );
	const double bAspect = bSizes.x / bSizes.y;

	Math::dvec3 sizes = bSizes;
	if( (wAspect > bAspect) == (adaptPolicy == BoxAdaptationPolicy::Fill) )
		sizes.y = sizes.x / wAspect;
	else
		sizes.x = sizes.y * wAspect;	

	Math::dbox3 retval( -sizes * 0.5, sizes * 0.5 );
	return retval.translate( bCenter );
}

				     