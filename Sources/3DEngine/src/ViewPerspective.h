// ViewPerspective.h
//
#ifndef VIEWPERSPECTIVE_ONCE
#define VIEWPERSPECTIVE_ONCE

#include "3DEngine_API.h"

#include "Math/Math.h"

namespace Scene {

class S2SCENE_API ViewPerspective
{
public:
	ViewPerspective();

	void setAspectRatio( double ar );
	void setFieldOfView( double fovY );
	void setFieldOfViewHorizontal( double fovX );
	void setPlanes( double nearPlane, double farPlane );
	void set( double aspectRatio, double fovY, double nearPlane, double farPlane );
	void setByFovHorizontal( double aspectRatio, double fovX, double nearPlane, double farPlane );

	Math::dmat4 matrix()                const;
	double      aspectRatio()           const;
	double      fieldOfView()           const;
	double      fieldOfViewHorizontal() const;
	double      nearPlane()             const;
	double      farPlane()              const;

private:
	Math::dmat4 _matrix;		// world to view matrix
	double      _fov;			// in degrees 
	double      _horizontalFov;	// in degrees
	double      _aspectRatio;
	double      _nearPlane;
	double      _farPlane;
};

}

#endif // VIEWPERSPECTIVE_ONCE
