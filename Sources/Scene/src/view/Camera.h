// Camera.h
// 
#ifndef RENDERENGINE_CAMERA_H
#define RENDERENGINE_CAMERA_H

#include "RenderPipeline_API.h"

#include "Math/Math.h"
#include "Math/Box.h"
#include "Math/Rectangle.h"

#include <vector>
#include <functional>

namespace RenderPipeline {

class RENDERPIPELINE_API Camera
{
public:
	Camera() = default;
	Camera( const Math::dvec3 &pos, const Math::dvec3 &target, const Math::dvec3 &up );

	void addConstraint( const std::function<Camera(Camera *)> & );

	void set( const Math::dvec3 &position, const Math::dmat4 &orientation );

	void lookAt( const Math::dvec3 &eyePos, const Math::dvec3 &centerPos, const Math::dvec3 &upVector );
	void lookAt( const Math::dvec3 &pos );
	void moveToLookAt( const Math::dvec3 &pos );

	void rotate( double degrees, const Math::dvec3 &axis, const Math::dvec3 &pivot );
	void rotate( double degrees, const Math::dvec3 &axis );

	void move( const Math::dvec3 &delta );
	void moveTarget( const Math::dvec3 &delta );

	Math::dvec3 position() const;
	Math::dvec3 target()   const;
	Math::dvec3 up()       const;

	Math::dvec3 front() const { return Math::normalize( _target - _position ); }
	Math::dvec3 side()  const { return Math::normalize( Math::cross( _target - _position, _up ) ); }

	Math::dmat4 matrix()         const; // world to camera matrix
	Math::dmat4 orientation()    const; // camera orientation in wc

	void fitSphere( const Math::dvec3 &center, double radius );
	void fitBoundingBox( const Math::dbox3 &box );
	void fitScreenRegion( const Math::irect &rectangle );

protected:
	void applyConstraints();

protected:
	std::vector< std::function<Camera( Camera * )> > _constraints;


	Math::dvec3 _position { 0.0, 0.0, 1.0 };     // worldspace coordinates
	Math::dvec3 _target   { 0.0, 0.0, 0.0 };     // worldspace coordinates
	Math::dvec3 _up       { 0.0, 1.0, 0.0 };     // normalized camera upvector
};


}

#endif