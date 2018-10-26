// Camera.h
// 
#ifndef CAMERA_ONCE
#define CAMERA_ONCE

#include "s2SceneGraph_API.h"

#include "Math/Math.h"
#include "Math/Box.h"
#include "Math/Rectangle.h"

namespace s2 {
namespace SceneGraph {

class S2SCENEGRAPH_API Camera
{
public:
	Camera();
	virtual ~Camera();

	Math::dvec3 position()      const;
	Math::dvec3 target()        const;
	Math::dmat4 matrix()        const;

	Math::dvec3 upVector()      const;
	//	Math::dvec3 viewVector()    const;
	//	Math::dvec3 rightVector()   const;
	//	Math::dquat orientation()   const;

		//void setTarget(const Math::dvec3& target);
		//void setPosition(const Math::dvec3& pos);
	void set( const Math::dvec3 &pos, const Math::dvec3 &target, const Math::dvec3 &up );

	//void setOrientation(const Math::dquat& q);

	//void rotatePosition( double angle, const Math::dvec3 &axis, const Math::dvec3 &pivot );

	void fitSphere( const Math::dvec3 &center, double radius );
	void fitBoundingBox( const Math::box3 &box );
	void fitScreenRegion( const Math::Rectangle &rectangle );

protected:
	//Math::dquat _orientation;
	Math::dvec3 _position;
	Math::dvec3 _target;
	Math::dvec3 _up;
	//Math::dvec3 _pivot;
	double      _targetRadius;
};

}
}


#endif