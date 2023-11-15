// Camera.h
//
#ifndef CAMERA_ONCE
#define CAMERA_ONCE

#include "CoreAPI.h"

#include "Math/Math.h"

namespace OpenGL{

class S2CORE_API Camera
{
public:
	Math::dvec3 _eye;		// worldspace coordinates
	Math::dvec3 _center;	// worldspace coordinates
	Math::dvec3 _up;		// normalized camera vector
	
public:
	Camera();
	~Camera();

	void set( const Math::dvec3 &position, const Math::dmat4 &orientation );
	void lookAt( const Math::dvec3 &eyePos, const Math::dvec3 &centerPos, const Math::dvec3 &upVector );
	void lookAt( const Math::dvec3 &pos );

	void rotate( double degrees, const Math::dvec3 &axis, const Math::dvec3 &pivot );
	void rotate( double degrees, const Math::dvec3 &axis );

	Math::dvec3 eye()		const {return _eye;}
	Math::dvec3 center()	const {return _center;}
	
	Math::dvec3 up()	    const {return _up;}
	Math::dvec3 front()	    const {return Math::normalize(_center-_eye);}
	Math::dvec3 side()	    const {return Math::normalize(Math::cross(_center-_eye,_up) );}

	Math::dmat4 matrix()         const;
	Math::dmat4 orientation()    const;
};

}

#endif // CAMERA_ONCE
