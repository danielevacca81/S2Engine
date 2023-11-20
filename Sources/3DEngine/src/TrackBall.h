// TrackBall.h
//
#ifndef TRACKBALL_ONCE
#define TRACKBALL_ONCE

#include "Core/Math.h"

#include "s2Scene_API.h"

namespace Scene {

class S2SCENE_API TrackBall
{
public:
	enum class UpdateEvent { DragBegin, DragEnd, DragUpdate };

public:
	TrackBall();

	void setRadius( double radius );
	void setOrientation( const Math::dmat4 &orientation );
	void update( UpdateEvent mode, const Math::ivec2 &screenPos );
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

}	// End of namespace
#endif