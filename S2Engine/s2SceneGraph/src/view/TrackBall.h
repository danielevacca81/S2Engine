// TrackBall.h
//
#ifndef TRACKBALL_ONCE
#define TRACKBALL_ONCE

#include "Math/Math.h"

#include "s2SceneGraph_API.h"

namespace s2 {
namespace SceneGraph {

class S2SCENEGRAPH_API TrackBall
{
public:
	enum UpdateMode { Start_Drag, End_Drag, Drag };

public:
	TrackBall();
	~TrackBall();

	void setRadius( double radius );
	void setOrientation( const Math::dmat4 &orientation );
	void update( UpdateMode mode, const Math::ivec2 &screenPos );
	void resize( int pixelWidth, int pixelHeight );
	void reset();

	Math::dmat4 matrix()     const;
	Math::dquat quaternion() const;
	double      radius()     const;

private:
	Math::dvec2 normalized( const Math::ivec2 &screenPos ) const;
	Math::dvec3 mapToSphere( const Math::ivec2 &screenPos ) const;
	void update( const Math::ivec2 &screenPos );


private:
	Math::dvec2 _size;
	Math::dvec3 _center;
	double      _radius;

	Math::dvec3 _startPoint;
	Math::dquat _prevQ;
	Math::dquat _currQ;

};

}
}	// End of namespace
#endif