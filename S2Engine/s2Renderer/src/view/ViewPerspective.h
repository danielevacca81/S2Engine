// ViewPerspective.h
//
#ifndef VIEWPERSPECTIVE_ONCE
#define VIEWPERSPECTIVE_ONCE

#include "s2Renderer_API.h"

#include "Math/Math.h"

namespace s2 {
namespace Renderer {

class S2RENDERER_API ViewPerspective
{
public:
	ViewPerspective();
	~ViewPerspective();

	void setAspectRatio( double ar );
	void setFieldOfViewY( double fovY );
	void setPlanes( double nearPlane, double farPlane );
	void set( double aspectRatio, double fovY, double nearPlane, double farPlane );

	Math::dmat4 matrix()       const;
	double      aspectRatio()  const;
	double      fieldOfViewY() const;
	double      fieldOfViewX() const;
	double      nearPlane()    const;
	double      farPlane()     const;

private:
	Math::dmat4 _matrix;    // world to view matrix
	double      _fovY;
	double      _aspectRatio;
	double      _nearPlane;
	double      _farPlane;
};

}
}

#endif // VIEWPERSPECTIVE_ONCE
