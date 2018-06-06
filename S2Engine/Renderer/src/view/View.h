// View.h
//
#ifndef VIEW_ONCE
#define VIEW_ONCE

#include "Renderer_API.h"

#include "Math/Math.h"

namespace s2 {

class RENDERER_API View
{
public:
	View();
	~View();

	void set( const Math::dvec3 &eye, const Math::dvec3 &target, const Math::dvec3 &up );
	void set( const Math::dmat4 &m );
	void setAspectRatio ( double ar );
	void setFieldOfViewY( double fovY );

	void setPerspectivePlanes( double nearPlane, double farPlane );
	void setOrthographicPlanes( double nearPlane, double farPlane );

	void setPerspective( double aspectRatio, double fovY, double nearPlane, double farPlane );
	void setOrthographic( double left, double right, double bottom, double top, double nearPlane, double farPlane );

	Math::dmat4 matrix() const { return _matrix; }

	Math::dvec3 eye()		                    const { return computeEyePosition(_matrix); }
	//Math::dvec3 target()	                    const;
	Math::dvec3 up()                            const { return Math::dvec3(_matrix[1]); }
	Math::dvec3 forward()	                    const { return Math::dvec3(_matrix[2]); }
	Math::dvec3 right()	                        const { return Math::dvec3(_matrix[0]); }
	 
	//Math::dvec3 forward()	                    const { return Math::normalize(_target-_eye); }
	//Math::dvec3 right()	                        const { return Math::normalize( Math::cross(forward(), up()) ); }

	double      aspectRatio()                   const { return _aspectRatio; }
	double      fieldOfViewY()                  const { return _fovY; }
	double      fieldOfViewX()                  const { return 2.0 * Math::atan( _aspectRatio * Math::tan( _fovY * 0.5 ) ); }

	double      perspectiveNearPlaneDistance()  const { return _perspectiveNearDistance; }
	double      perspectiveFarPlaneDistance()   const { return _perspectiveFarDistance;  }
		        
	double      orthographicNearPlaneDistance() const { return _orthographicNearDistance; }
	double      orthographicFarPlaneDistance()  const { return _orthographicFarDistance;  }
	double      orthographicDepth()             const { return Math::abs( _orthographicFarDistance - _orthographicNearDistance); }

	double      orthographicLeft()              const { return _orthographicLeft;   }
	double      orthographicRight()             const { return _orthographicRight;  }
	double      orthographicBottom()            const { return _orthographicBottom; }
	double      orthographicTop()               const { return _orthographicTop;    }

	//void        zoomToTarget( double targetRadius ); // move to camera

private:
	Math::dvec3 computeEyePosition( const Math::dmat4 &m ) const;

private:
	Math::dmat4 _matrix;    // world to view matrix
	
	double      _fovY;
	double      _aspectRatio;
	double      _perspectiveNearDistance;
	double      _perspectiveFarDistance;
	
	double      _orthographicNearDistance;
	double      _orthographicFarDistance;
	double      _orthographicLeft;
	double      _orthographicRight;
	double      _orthographicBottom;
	double      _orthographicTop;

};

}

#endif // VIEW_ONCE
