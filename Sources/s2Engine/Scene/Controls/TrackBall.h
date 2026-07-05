// TrackBall.h
//
#ifndef GRAPHICS_TRACKBALL_H
#define GRAPHICS_TRACKBALL_H

#include "Math/Math.h"

#include "s2Engine_API.h"

namespace s2 {
namespace Scene {

class S2ENGINE_API TrackBall
{
public:
	enum class DragEvent { Begin, End, Update };

public:
	//TrackBall();

	void setRadius( double radius );
	void setOrientation( const Math::dmat4 &orientation );
	void setCenter( const Math::ivec2& screenPos );
	void update( const DragEvent &e, const Math::ivec2 &screenPos );
	void resize( int pixelWidth, int pixelHeight );
	//void reset();

	Math::dmat4 matrix()     const;
	Math::dquat quaternion() const;
	double      radius()     const;

private:
	void enterDrag( const Math::ivec2& screenPos );
	void exitDrag();
	Math::dvec2 normalized( const Math::ivec2 &screenPos ) const;
	Math::dvec3 mapToSphere( const Math::ivec2 &screenPos ) const;
	void update( const Math::ivec2 &screenPos );


private:
	Math::dvec2 _size;
	Math::ivec2 _center;
	double      _radius { 1.0 };

	Math::dvec3 _startPoint;
	Math::dquat _prevQ { 1.0, 0.0, 0.0, 0.0 };
	Math::dquat _currQ { 1.0, 0.0, 0.0, 0.0 };
	bool 	 _dragging { false }; // true if the trackball is currently being dragged

};

}
}
#endif // !SCENE_TRACKBALL_H