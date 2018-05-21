// MouseStatus.h
//
#ifndef S2QT_MOUSESTATUS_ONCE
#define S2QT_MOUSESTATUS_ONCE

#include "S2QtAPI.h"

#include <QPoint>

class QGraphicsSceneMouseEvent;
class QGraphicsSceneWheelEvent;

class QMouseEvent;
class QWheelEvent;


namespace s2Qt {

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
	};


private:
	template<typename T> Status getButton  ( T *e );
	template<typename T> Status getModifier( T *e );

	QPointF		_currPos;
	QPointF		_prevPos;
	double      _wheel;
	bool		_moving;
	bool		_doubleClick;
	int			_pressedButton;
	int			_releasedButton;
	int			_keyModifier;

public:
	MouseStatus();
	~MouseStatus(){}
	
	void reset();
	
	void update( QGraphicsSceneMouseEvent *e, bool updateButtons );
	void update( QGraphicsSceneWheelEvent *e, bool updateButtons );

	void update( QMouseEvent *e, bool updateButtons );
	void update( QWheelEvent *e, bool updateButtons );

	// -----------------------------------------------------------------------------------------------
	double       wheel()      const { return _wheel; }
	bool         isMoving()   const { return _moving; }
	QPointF      currPos()	  const { return _currPos; }
	QPointF      prevPos() 	  const { return _prevPos; }
	int          buttonDown() const { return _pressedButton;}
	int          buttonUp()   const { return _releasedButton;}
	int          modifier()   const { return _keyModifier; }
	unsigned int code()       const;
	// -----------------------------------------------------------------------------------------------
};

}
#endif
