// MouseStatus.cpp
//
#include "MouseStatus.h"

#include <QWidget>
#include <QGraphicsSceneMouseEvent>
#include <QMouseEvent>

#include <cmath>

using namespace s2::Qt;

// ------------------------------------------------------------------------------------------------
MouseStatus::MouseStatus()
{
	reset();
}

// ------------------------------------------------------------------------------------------------
MouseStatus::~MouseStatus()
{}

// ------------------------------------------------------------------------------------------------
MouseStatus::Status MouseStatus::getButton( QGraphicsSceneEvent *e ) const
{
	if( e->type() == QGraphicsSceneEvent::Wheel )
		return Status( ButtonNone );

	int btn               = ButtonNone;
	::Qt::MouseButton qtbtn = static_cast<QGraphicsSceneMouseEvent*>(e)->button();

	if( qtbtn & ::Qt::LeftButton )  btn |= ButtonLeft;
	if( qtbtn & ::Qt::RightButton ) btn |= ButtonRight;
	if( qtbtn & ::Qt::MidButton )   btn |= ButtonMiddle;

	return Status(btn);
}

// ------------------------------------------------------------------------------------------------
MouseStatus::Status MouseStatus::getButton( QEvent *e ) const
{
	if( e->type() == QEvent::Wheel )
		return Status( ButtonNone );

	int btn               = ButtonNone;
	::Qt::MouseButton qtbtn = static_cast<QMouseEvent*>(e)->button();

	if( qtbtn & ::Qt::LeftButton )  btn |= ButtonLeft;
	if( qtbtn & ::Qt::RightButton ) btn |= ButtonRight;
	if( qtbtn & ::Qt::MidButton )   btn |= ButtonMiddle;

	return Status(btn);
}

// -----------------------------------------------------------------------------------------------
MouseStatus::Status MouseStatus::getModifier( const ::Qt::KeyboardModifiers &kbMod ) const
{
	int mod = ModifierNone;

	if( kbMod & ::Qt::ShiftModifier )	mod |= ModifierShift;
	if( kbMod & ::Qt::ControlModifier ) mod |= ModifierCtrl;
	if( kbMod & ::Qt::AltModifier )	    mod |= ModifierAlt;
	if( kbMod & ::Qt::MetaModifier )	mod |= ModifierMeta;
	
	return Status(mod);
}
	
// -----------------------------------------------------------------------------------------------
void MouseStatus::reset()
{
	_pressedButton  = ButtonNone;
	_releasedButton = ButtonNone;
	_keyModifier    = ModifierNone;
	_moving         = false;
	_dragging       = false;
	_doubleClick    = false;
	_wheel          = 0.f;
	_clickPos       = QPointF(-1,-1);
}


// -----------------------------------------------------------------------------------------------
void MouseStatus::update( QWidget *w, QGraphicsSceneMouseEvent *e, bool updateButtons )
{
	_wheel          = 0.f;
	_prevPos        = _currPos;
	_currPos        = QPointF( e->scenePos().x(), e->widget()->height() - e->scenePos().y()-1 );
	_moving         = (e->type() == e->GraphicsSceneMouseMove);
	_releasedButton = ButtonNone;
	_keyModifier    = getModifier( e->modifiers() );

	// dragging check
	if( _moving && _clickPos.x()>=0.0 && _clickPos.y()>=0.0 
	&& (_clickPos-_prevPos).manhattanLength() > MovingThreshold )
		_dragging = true;

	if( updateButtons )
	{
		Status btn   = getButton(e);
		_doubleClick = false;

		if( e->type() == QEvent::GraphicsSceneMouseDoubleClick )
		{
			_dragging       = false;
			_doubleClick    = true;
			_pressedButton |= btn;
		}
		else if (e->type() == QEvent::GraphicsSceneMousePress )
		{
			_pressedButton |= btn;
			_clickPos       = e->scenePos();
			_dragging       = false;
		}
		else if (e->type() == QEvent::GraphicsSceneMouseRelease )
		{
			_pressedButton  &= ~btn;
			_releasedButton |=  btn;

			_clickPos = QPointF(-1,-1);
		}
	}

	// end of dragging
	if( _dragging && _moving && !_pressedButton )
		_dragging = false;

	
#if 0
	if( !toString().empty() )
		std::cout << toString() << std::endl;
#endif
	e->accept();
}

// -----------------------------------------------------------------------------------------------
void MouseStatus::update( QWidget *w, QGraphicsSceneWheelEvent *e, bool updateButtons )
{
	const double WHEEL_STEP = 120.;
	const double step = e->delta() / WHEEL_STEP;

    _wheel = Math::pow( 1.25, step );
    if( _wheel > 100 ) _wheel = 100;
    if( _wheel < .01 ) _wheel = .01;
	
	_keyModifier = getModifier( e->modifiers() );

	if( updateButtons )
	{
		Status btn = getButton(e);
			
		// needed ?
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


#if 0
	if( !toString().empty() )
		std::cout << toString() << std::endl;
#endif
	e->accept();
}

// -----------------------------------------------------------------------------------------------
void MouseStatus::update( QWidget *w, QMouseEvent *e, bool updateButtons )
{
	_wheel          = 0.f;
	_prevPos        = _currPos;
	_currPos        = QPointF( e->x(), w->height() - e->y() - 1 );
	_moving         = ( e->type() == e->MouseMove );
	_releasedButton = ButtonNone;
	_keyModifier    = getModifier( e->modifiers() );

	// dragging check
	if( _moving && _clickPos.x()>=0.0 && _clickPos.y()>=0.0 
	&& (_clickPos-_prevPos).manhattanLength() > MovingThreshold )
		_dragging = true;

	if( updateButtons )
	{
		Status btn   = getButton(e);
		_doubleClick = false;

		if( e->type() == QEvent::MouseButtonDblClick )
		{
			_dragging       = false;
			_doubleClick    = true;
			_pressedButton |= btn;
		}
		else if (e->type() == QEvent::MouseButtonPress )
		{
			_pressedButton |= btn;
			_clickPos       = e->pos();
			_dragging       = false;
		}
		else if (e->type() == QEvent::MouseButtonRelease )
		{
			_pressedButton  &= ~btn;
			_releasedButton |=  btn;

			_clickPos = QPointF(-1,-1);
		}
	}

	// end of dragging
	if( _dragging && _moving && !_pressedButton )
		_dragging = false;

	
#if 0
	if( !toString().empty() )
		std::cout << toString() << std::endl;
#endif
	e->accept();
}

// -----------------------------------------------------------------------------------------------
void MouseStatus::update( QWidget *w, QWheelEvent *e, bool updateButtons )
{
	const double WHEEL_STEP = 120.;
	const double step = e->delta() / WHEEL_STEP;

    _wheel = Math::pow( 1.25, step );
    if( _wheel > 100 ) _wheel = 100;
    if( _wheel < .01 ) _wheel = .01;
	
	_keyModifier = getModifier( e->modifiers() );

	if( updateButtons )
	{
		Status btn = getButton(e);
			
		// needed ?
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


#if 0
	if( !toString().empty() )
		std::cout << toString() << std::endl;
#endif
	e->accept();
}

// -----------------------------------------------------------------------------------------------
unsigned int MouseStatus::getCode() const
{
	unsigned int code = 0;

	if( _keyModifier & ModifierAlt   )            code |= ModifierAlt;
	if( _keyModifier & ModifierCtrl  )            code |= ModifierCtrl;
	if( _keyModifier & ModifierShift )            code |= ModifierShift;
	if( _keyModifier & ModifierMeta  )            code |= ModifierMeta;
												  
	if( _pressedButton & ButtonLeft   )           code |= ButtonLeft;
	if( _pressedButton & ButtonRight  )           code |= ButtonRight;
	if( _pressedButton & ButtonMiddle )           code |= ButtonMiddle;
	
	if( _releasedButton & ButtonLeft   )          code |= ButtonLeft|Released;
	if( _releasedButton & ButtonRight  )          code |= ButtonRight|Released;
	if( _releasedButton & ButtonMiddle )          code |= ButtonMiddle|Released;
	
	if( _pressedButton & ButtonAny   && _moving ) code |= Drag;
	if( _pressedButton == ButtonNone && _moving ) code |= Moving;

	if( _doubleClick              )               code |= DoubleClick;
	if( _wheel>0.f || _wheel<0.f  )			      code  = Wheel;

	return code;
}