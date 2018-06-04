// TrackBall.h
//
#ifndef TRACKBALL_ONCE
#define TRACKBALL_ONCE

#include "Math/Math.h"

#include "Renderer_API.h"

namespace s2 {

class RENDERER_API TrackBall
{
private:
	Math::dvec2 _size;
	Math::dvec3 _center;
	double      _radius;

	Math::dvec3 _startPoint;
	Math::dquat _prevQ;
	Math::dquat _currQ;
	
	Math::dvec2 normalized( const Math::ivec2 &screenPos, bool flipY ) const;
	Math::dvec3 mapToSphere( const Math::ivec2 &screenPos, bool flipY ) const;
	void update( const Math::ivec2 &screenPos, bool flipY );


public:
	enum UpdateMode { Start_Drag, End_Drag, Drag };

	TrackBall();
	~TrackBall();

	void setRadius( double radius );
	void setOrientation( const Math::dmat4 &orientation );
	void update( UpdateMode mode,const Math::ivec2 &screenPos, bool flipY = true );
	void resize( int pixelWidth,int pixelHeight );
	void reset();

	Math::dmat4 matrix()     const;
	Math::dquat quaternion() const;
	double      radius()     const;
};

}	// End of namespace
#endif