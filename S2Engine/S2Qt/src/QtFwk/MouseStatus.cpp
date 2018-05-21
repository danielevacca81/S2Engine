// MouseStatus.cpp
//
#include "MouseStatus.h"

#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>

using namespace s2Qt;

// -----------------------------------------------------------------------------------------------
template <typename T>
MouseStatus::Status MouseStatus::getButton( T *e )
{
	int btn               = ButtonNone;
	Qt::MouseButton qtbtn = Qt::NoButton;

	if( e->type() != QEvent::GraphicsSceneWheel )
		qtbtn = ((QGraphicsSceneMouseEvent*)e)->button();

	if (qtbtn & Qt::LeftButton)  btn |= ButtonLeft;
	if (qtbtn & Qt::RightButton) btn |= ButtonRight;
	if (qtbtn & Qt::MidButton)   btn |= ButtonMiddle;

	return Status(btn);
}

// -----------------------------------------------------------------------------------------------
template <typename T>
MouseStatus::Status MouseStatus::getModifier( T *e )
{
	int mod          = ModifierNone;

	Qt::KeyboardModifiers modifiers = e->modifiers();

	if (modifiers & Qt::ShiftModifier)	 mod |= ModifierShift;
	if (modifiers & Qt::ControlModifier) mod |= ModifierCtrl;
	if (modifiers & Qt::AltModifier)	 mod |= ModifierAlt;
	if (modifiers & Qt::MetaModifier)	 mod |= ModifierMeta;
	
	return Status(mod);
}

// ------------------------------------------------------------------------------------------------
MouseStatus::MouseStatus()
{
	reset();
}

	
// -----------------------------------------------------------------------------------------------
void MouseStatus::reset()
{
	_pressedButton  = ButtonNone;
	_releasedButton = ButtonNone;
	_keyModifier    = ModifierNone;
	_moving         = false;
	_doubleClick    = false;
	_wheel          = 0.f;
}


// -----------------------------------------------------------------------------------------------
void MouseStatus::update( QGraphicsSceneMouseEvent *e, bool updateButtons )
{
	_wheel          = 0.f;
	_prevPos        = _currPos;
	_currPos        = e->scenePos();
	_moving         = (e->type() == e->GraphicsSceneMouseMove);
	_releasedButton = ButtonNone;
	_keyModifier    = getModifier(e);

	if( updateButtons )
	{
		Status btn  = getButton(e);
		_doubleClick = false;

		if (e->type() == QEvent::GraphicsSceneMouseDoubleClick )
		{
			_doubleClick    = true;
			_pressedButton |= btn;
		}
		else if (e->type() == QEvent::GraphicsSceneMousePress )
		{
			_pressedButton |=   btn;
		}
		else if (e->type() == QEvent::GraphicsSceneMouseRelease )
		{
			_pressedButton &= ~(btn);		
			_releasedButton |= btn;
		}
	}
	
	e->accept();
}

// -----------------------------------------------------------------------------------------------
void MouseStatus::update( QGraphicsSceneWheelEvent *e, bool updateButtons )
{
	const double WHEEL_STEP = 120.;
	const double step = e->delta() / WHEEL_STEP;

	_wheel = pow(1.25, step);
	if( _wheel > 100 ) _wheel = 100;
	if( _wheel < .01 ) _wheel = .01;
	
	_keyModifier = getModifier(e);

	if( updateButtons )
	{
		Status btn = getButton(e);
			
		if (e->type() == QEvent::GraphicsSceneMousePress )
		{
			_pressedButton |=   btn;
		}
		else if (e->type() == QEvent::GraphicsSceneMouseRelease )
		{
			_pressedButton &= ~(btn);		
			_releasedButton |= btn;
		}
	}

	e->accept();
}

// -----------------------------------------------------------------------------------------------
void MouseStatus::update( QMouseEvent *e, bool updateButtons )
{
	_wheel          = 0.f;
	_prevPos        = _currPos;
	_currPos        = e->posF();
	_moving         = (e->type() == e->MouseMove);
	_releasedButton = ButtonNone;
	_keyModifier    = getModifier(e);

	if( updateButtons )
	{
		Status btn  = getButton(e);
		_doubleClick = false;

		if (e->type() == QEvent::MouseButtonDblClick )
		{
			_doubleClick    = true;
			_pressedButton |= btn;
		}
		else if (e->type() == QEvent::MouseButtonPress )
		{
			_pressedButton |=   btn;
		}
		else if (e->type() == QEvent::MouseButtonRelease )
		{
			_pressedButton &= ~(btn);		
			_releasedButton |= btn;
		}
	}

	e->accept();
}

// -----------------------------------------------------------------------------------------------
void MouseStatus::update( QWheelEvent *e, bool updateButtons )
{
	const double WHEEL_STEP = 120.;
	const double step = e->delta() / WHEEL_STEP;

	_wheel = pow(1.25, step);
	if( _wheel > 100 ) _wheel = 100;
	if( _wheel < .01 ) _wheel = .01;

	_keyModifier = getModifier(e);

	if( updateButtons )
	{
		Status btn = getButton(e);

		if (e->type() == QEvent::MouseButtonPress )
		{
			_pressedButton |=   btn;
		}
		else if (e->type() == QEvent::MouseButtonRelease )
		{
			_pressedButton &= ~(btn);		
			_releasedButton |= btn;
		}
	}

	e->accept();
}

// -----------------------------------------------------------------------------------------------
unsigned int MouseStatus::code() const
{
	unsigned int c = 0;

	if( _keyModifier & ModifierAlt   )           c |= ModifierAlt;
	if( _keyModifier & ModifierCtrl  )           c |= ModifierCtrl;
	if( _keyModifier & ModifierShift )           c |= ModifierShift;
	if( _keyModifier & ModifierMeta  )           c |= ModifierMeta;
											 
	if( _pressedButton & ButtonLeft   )          c |= ButtonLeft;
	if( _pressedButton & ButtonRight  )          c |= ButtonRight;
	if( _pressedButton & ButtonMiddle )          c |= ButtonMiddle;
	
	if( _pressedButton & ButtonAny  && _moving ) c |= Drag;
	if( _pressedButton & ButtonNone && _moving ) c |= Moving;

	if( _wheel>0.f || _wheel<0.f )			     c |= Wheel;
	if( _doubleClick             )               c |= DoubleClick;

	return c;
}