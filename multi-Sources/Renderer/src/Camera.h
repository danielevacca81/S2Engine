// Camera.h
//
#ifndef GRAPHICS_CAMERA_H
#define GRAPHICS_CAMERA_H

#include "Renderer_API.h"

#include "Math/Math.h"
#include "Math/ProjectionTransform.h"
#include "Math/Rectangle.h"
#include "Math/Ray.h"

namespace Graphics {

class RENDERER_API Camera
{
public:
	Camera() = default;
	virtual ~Camera() = default;

	void set( const Math::dvec3& eye, const Math::dvec3& target, const Math::dvec3& up );
	void set( const Math::dmat4& cameraMatrix );
	void set( const Math::dvec3& origin, double theta, double phi, double rho );

	void setViewport( const Math::irect& viewportRect );
	void setProjectionTransform( const Math::ProjectionTransform& t );
	void setNearFarPlanes( double nearPlane, double farPlane );

	const Math::irect& viewport() const;
	
	Math::dmat4 worldToCameraMatrix() const; // aka view matrix
	Math::dmat4	cameraToWorldMatrix() const; // inverse of worldToCameraMatrix
	Math::dmat4 orientationMatrix() const;   // returns the orientation matrix of the camera, i.e. the rotation part of the worldToCameraMatrix without translation
	Math::dmat4 projectionMatrix() const { return _projection.matrix(); }
	
	Math::dvec3 dirSide()  const;
	Math::dvec3 dirFront() const;
	Math::dvec3 position() const;
	Math::dvec3 target()   const;
	Math::dvec3 up()       const;

	
	Math::dray  rayAt( const Math::dvec2& screenCoord ) const; // returns a ray in worldspace starting at the camera position and pointing through the screen coordinate
	Math::dvec3 worldPoint( const Math::dvec2& screenCoord ) const; // returns the worldspace point at the given screen coordinate
	Math::ivec2 screenPoint( const Math::dvec3& worldCoord ) const; // returns the screen coordinate of the given worldspace point
	//Math::dvec3 normalizedDeviceCoordinates( const Math::ivec2& screenCoord ) const;
	double      worldSize( int pixels )       const;
	int         pixelSize( double worldSize ) const;


	bool operator==( const Camera& o ) const;
	bool operator!=( const Camera& o ) const { return !( *this == o ); }


protected:
	Math::irect _viewport;

	Math::ProjectionTransform _projection;

	Math::dvec3 _position { 0.0, 0.0, 1.0 };     // worldspace coordinates
	Math::dvec3 _target   { 0.0, 0.0, 0.0 };     // worldspace coordinates
	Math::dvec3 _up       { 0.0, 1.0, 0.0 };     // normalized camera upvector
};

}

#endif // !GRAPHICS_CAMERA_H