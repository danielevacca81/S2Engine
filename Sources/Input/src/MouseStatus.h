// MouseStatus.h
//
#ifndef QTBRIDGE_MOUSESTATUS_H
#define QTBRIDGE_MOUSESTATUS_H

#include "s2Qt_API.h"

#include <QPoint>

#include "math/Math.h"


// to be removed...
class QGraphicsSceneEvent;
class QGraphicsSceneMouseEvent;
class QGraphicsSceneWheelEvent;
// ....


class QWidget;
class QEvent;
class QMouseEvent;
class QWheelEvent;

namespace s2 {
namespace Qt {


/*
	"Alt",         MouseStatus::ModifierAlt
	"Ctrl",        MouseStatus::ModifierCtrl
	"Shift",       MouseStatus::ModifierShift
	"LeftButton",  MouseStatus::ButtonLeft
	"RightButton", MouseStatus::ButtonRight
	"MidButton",   MouseStatus::ButtonMiddle
	"DoubleClick", MouseStatus::DoubleClick
	"Moving",      MouseStatus::Moving
	"Drag",        MouseStatus::Drag
	"Wheel",       MouseStatus::Wheel
	"Released",    MouseStatus::Released
*/

class S2QT_API MouseStatus
{
public:
	//
	enum Status
	{
		ButtonNone    = 0x0000, // No button or key pressed.
		ButtonLeft    = 0x0001, // Left mouse button pressed.
		ButtonMiddle  = 0x0002, // Middle mouse button pressed.
		ButtonRight   = 0x0004, // Right mouse button pressed.
		Wheel         = 0x0008, // Mouse wheel activated.
		DoubleClick   = 0x0010, // Double click
		ButtonAny     = 0x00FF, // Mask to get buttons only

		ModifierNone  = 0x0000, // No modifier
		ModifierShift = 0x0100, // Shift key pressed
		ModifierCtrl  = 0x0200, // Ctrl key pressed
		ModifierAlt   = 0x0400, // Alt key pressed
		ModifierMeta  = 0x0800, // Meta key pressed
		ModifierAny   = 0x0F00, // Mask to get modifiers only

		Moving        = 0x1000, // Moving
		Drag          = 0x2000, // Drag
		Pressed       = 0x4000,
		Released      = 0x8000,
		Clicked       = 0xA000
	};

public:
	MouseStatus();
	~MouseStatus();

	double       wheel()            const { return _wheel; }
	bool         isMoving()         const { return _moving; }
	bool         isDragging()       const { return _dragging; }
	Math::ivec2  currentPosition()  const { return Math::ivec2( _currPos.x(), _currPos.y() ); }
	Math::ivec2  previousPosition() const { return Math::ivec2( _prevPos.x(), _prevPos.y() ); }
	int          buttonDown()       const { return _pressedButton; }
	int          buttonUp()         const { return _releasedButton; }
	int          modifiers()        const { return _keyModifier; }


private:
	Status getButton( QGraphicsSceneEvent *e ) const;
	Status getButton( QEvent *e ) const;
	Status getModifier( const ::Qt::KeyboardModifiers &kbMod ) const;

	void reset();

	void update( QWidget *w, QGraphicsSceneMouseEvent *e, bool updateButtons );
	void update( QWidget *w, QGraphicsSceneWheelEvent *e, bool updateButtons );

	void update( QWidget *w, QMouseEvent *e, bool updateButtons );
	void update( QWidget *w, QWheelEvent *e, bool updateButtons );

	unsigned int getCode()          const;

private:
	static const int MovingThreshold = 8;

	QPointF	_currPos;
	QPointF	_prevPos;
	QPointF _clickPos;

	double  _wheel;
	bool	_moving;
	bool    _dragging;
	bool	_doubleClick;
	int		_pressedButton;
	int		_releasedButton;
	int		_keyModifier;

	friend class UserInteractionManager;
};

}
}

#endif
