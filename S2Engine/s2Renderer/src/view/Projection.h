// Projection.h
// 
#ifndef PROJECTION_ONCE
#define PROJECTION_ONCE

#include "Math/Math.h"

#include "Frustum.h"

namespace s2 {

class Projection
{
public:
	Projection();
	virtual ~Projection();

	void        setPerspective( double fovY, double aspect, double zNear, double zFar );
	void        setOrthographic( double left, double right, double bottom, double top, double zNear, double zFar );
	void        setScreenSpace( double left, double right, double bottom, double top );

	double      fov() const            { return _fov; }
	double      nearClip() const       { return _nearClip; }
	double      farClip() const        { return _farClip; }
	bool        isOrthographic() const { return _fov<=0.0; }
	bool        isPerspective()  const { return _fov>0.0;  }
	Math::dmat4 matrix() const         { return _matrix; }

protected:
	Math::dmat4 _matrix;

	double      _fov;
	double      _nearClip;
	double      _farClip;
};


}
#endif // VIEW_ONCE