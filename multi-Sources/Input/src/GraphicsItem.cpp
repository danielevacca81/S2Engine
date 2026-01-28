// GraphicsItem.cpp
//
#include "GraphicsItem.h"

using namespace s2::Qt;

//----------------------------------------------------------------------------------------------
GraphicsItem::GraphicsItem()
{
    setFlag(QGraphicsItem::ItemIsMovable,    true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsFocusable,  true);
    
	setAcceptHoverEvents(true);
    
	_fadeAnimation.setTargetObject(this);
	_fadeAnimation.setPropertyName("opacity");
	_fadeAnimation.setDuration(250);
	_fadeAnimation.setStartValue(0.0);
	_fadeAnimation.setEndValue(1.0);
	_fadeAnimation.setEasingCurve(QEasingCurve::Linear);
	
	setPos(0, 0);

	_timeout.setInterval(3000);
	connect( &_timeout, &QTimer::timeout, this, &GraphicsItem::onTimeout );
}

//----------------------------------------------------------------------------------------------
GraphicsItem::~GraphicsItem()
{}

//-------------------------------------------------------------------------------------------------
void GraphicsItem::setStartOpacity( double value )
{
	_fadeAnimation.stop();
	_fadeAnimation.setStartValue( value );
	QGraphicsItem::setOpacity(value);
}


//-------------------------------------------------------------------------------------------------
void GraphicsItem::onTimeout()
{
	_fadeAnimation.stop();
	_fadeAnimation.setDirection(QPropertyAnimation::Backward);
	_fadeAnimation.start();

	_timeout.stop();
}

//-------------------------------------------------------------------------------------------------
void GraphicsItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
	_fadeAnimation.stop();
	_fadeAnimation.setDirection(QPropertyAnimation::Forward);
	_fadeAnimation.start();
	QGraphicsItem::hoverEnterEvent(event);

	_timeout.start();
}

//-------------------------------------------------------------------------------------------------
void GraphicsItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
	_timeout.stop();
	_fadeAnimation.stop();

	if( opacity() > _fadeAnimation.startValue().toDouble() )
	{
		_fadeAnimation.setDirection(QPropertyAnimation::Backward);
		_fadeAnimation.start();
	}
	else
		setOpacity(_fadeAnimation.startValue().toDouble() );

	QGraphicsItem::hoverLeaveEvent(event);
}

//-------------------------------------------------------------------------------------------------
void GraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	_timeout.stop();
	
	if( opacity() < _fadeAnimation.endValue().toDouble() && _fadeAnimation.state() == QPropertyAnimation::Stopped )
	{
		//_fadeAnimation.stop();
		_fadeAnimation.setDirection(QPropertyAnimation::Forward);
		_fadeAnimation.start();
	}
}

//-------------------------------------------------------------------------------------------------
void GraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	_timeout.start();
}

//-------------------------------------------------------------------------------------------------
void GraphicsItem::wheelEvent(QGraphicsSceneWheelEvent *event)
{
	_timeout.start();
}

//-------------------------------------------------------------------------------------------------
void GraphicsItem::mouseMoveEvent   (QGraphicsSceneMouseEvent *event)
{
	_timeout.start();
}

//-------------------------------------------------------------------------------------------------
void GraphicsItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
	_timeout.start();
}
