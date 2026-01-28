// GestureStatus.h
//
#ifndef QTBRIDGE_GESTURESTATUS_H
#define QTBRIDGE_GESTURESTATUS_H

#include "s2Qt_API.h"

#include "math/Math.h"

#include <QPoint>
#include <QGestureEvent>

namespace s2 {

namespace Qt {

class S2QT_API GestureStatus
{
public:
	//
	enum Gesture
	{
		None       = 0xFFFF,
		Pan        = 0x0010, // Pan Gesture.
		Pinch      = 0x0020, // Pinch Gesture.
		Swipe      = 0x0040, // Swipe Gesture.
		Tap        = 0x0080,
		TapAndHold = 0x00F0
	};

	enum Direction
	{
		NoDirection = 0x0000,
		Left        = 0x0001,
		Right       = 0x0002,
		Up          = 0x0003,
		Down        = 0x0004
	};

public:
	GestureStatus();
	~GestureStatus() {}

	void update( QGestureEvent *e );

	// -----------------------------------------------------------------------------------------------
	double    scaleFactor() const { return _scaleFactor; }
	double    angle()       const { return _angleDeg; }
	//QPointF   center()      const { return _center;   }
	Math::ivec2  center()   const { return Math::ivec2( _center.x(), _center.y() ); }
	Math::ivec2  deltaPan() const { return Math::ivec2( _delta.x(), _delta.y() ); }
	Math::ivec2  prevCenter()   const { return Math::ivec2( _prevCenter.x(), _prevCenter.y() ); }
	Direction direction()   const;

	unsigned int getCode() const { return _gestureCode; }

private:
	double  _scaleFactor;
	double  _angleDeg;
	QPointF _center;
	QPointF _delta;
	QPointF _prevCenter;

	unsigned int _gestureCode;
};

}
}

#endif


