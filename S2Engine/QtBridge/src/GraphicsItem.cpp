// GraphicsItem.cpp
//
#include "GraphicsItem.h"

using namespace QtBridge;

//----------------------------------------------------------------------------------------------
GraphicsItem::GraphicsItem()
{
    setFlag(QGraphicsItem::ItemIsMovable,    true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsFocusable,  true);
    
	setAcceptHoverEvents(true);
    
	fadeAnimation.setTargetObject(this);
	fadeAnimation.setPropertyName("opacity");
	fadeAnimation.setDuration(250);
	fadeAnimation.setStartValue(0.0);
	fadeAnimation.setEndValue(1.0);
	fadeAnimation.setEasingCurve(QEasingCurve::Linear);
	
	setPos(0, 0);

	timeout.setInterval(3000);
	connect( &timeout,SIGNAL(timeout()),SLOT(onTimeout()) );
}

//----------------------------------------------------------------------------------------------
GraphicsItem::~GraphicsItem()
{}

//-------------------------------------------------------------------------------------------------
void GraphicsItem::setStartOpacity( double value )
{
	fadeAnimation.stop();
	fadeAnimation.setStartValue( value );
	QGraphicsItem::setOpacity(value);
}


//-------------------------------------------------------------------------------------------------
void GraphicsItem::onTimeout()
{
	fadeAnimation.stop();
	fadeAnimation.setDirection(QPropertyAnimation::Backward);
	fadeAnimation.start();

	timeout.stop();
}

//-------------------------------------------------------------------------------------------------
void GraphicsItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
	fadeAnimation.stop();
	fadeAnimation.setDirection(QPropertyAnimation::Forward);
	fadeAnimation.start();
	QGraphicsItem::hoverEnterEvent(event);

	timeout.start();
}

//-------------------------------------------------------------------------------------------------
void GraphicsItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
	timeout.stop();
	fadeAnimation.stop();

	if( opacity() > fadeAnimation.startValue().toDouble() )
	{
		fadeAnimation.setDirection(QPropertyAnimation::Backward);
		fadeAnimation.start();
	}
	else
		setOpacity(fadeAnimation.startValue().toDouble() );

	QGraphicsItem::hoverLeaveEvent(event);
}

//-------------------------------------------------------------------------------------------------
void GraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	timeout.stop();
	
	if( opacity() < fadeAnimation.endValue().toDouble() && fadeAnimation.state() == QPropertyAnimation::Stopped )
	{
		//fadeAnimation.stop();
		fadeAnimation.setDirection(QPropertyAnimation::Forward);
		fadeAnimation.start();
	}
}

//-------------------------------------------------------------------------------------------------
void GraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	timeout.start();
}

//-------------------------------------------------------------------------------------------------
void GraphicsItem::wheelEvent(QGraphicsSceneWheelEvent *event)
{
	timeout.start();
}

//-------------------------------------------------------------------------------------------------
void GraphicsItem::mouseMoveEvent   (QGraphicsSceneMouseEvent *event)
{
	timeout.start();
}

//-------------------------------------------------------------------------------------------------
void GraphicsItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
	timeout.start();
}
