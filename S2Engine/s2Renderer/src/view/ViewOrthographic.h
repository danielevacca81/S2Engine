// ViewOrthographic.h
//
#ifndef VIEWORTHOGRAPHIC_ONCE
#define VIEWORTHOGRAPHIC_ONCE

#include "s2Renderer_API.h"

#include "Math/Math.h"

namespace s2 {
namespace Renderer {

class S2RENDERER_API ViewOrthographic
{
public:
	ViewOrthographic();
	~ViewOrthographic();

	void set( double left, double right, double bottom, double top, double nearPlane, double farPlane );

	Math::dmat4 matrix()      const;
	double      nearPlane()   const;
	double      farPlane()    const;
	double      leftPlane()   const;
	double      rightPlane()  const;
	double      bottomPlane() const;
	double      topPlane()    const;

private:
	Math::dmat4 _matrix;    // world to view matrix
	double      _nearPlane;
	double      _farPlane;
	double      _leftPlane;
	double      _rightPlane;
	double      _bottomPlane;
	double      _topPlane;
};

}
}

#endif // VIEWORTHOGRAPHIC_ONCE
