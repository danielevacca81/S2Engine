// GraphicsItem.h
//
#ifndef S2QT_GRAPHICSITEM_ONCE
#define S2QT_GRAPHICSITEM_ONCE

#include "S2QtAPI.h"

#include <QGraphicsItem>
#include <QPropertyAnimation>
#include <QTimer>

namespace s2Qt {

class S2QT_API GraphicsItem : public QObject, public QGraphicsItem
{
	Q_OBJECT
	Q_PROPERTY(double opacity READ opacity WRITE setOpacity)
	Q_INTERFACES( QGraphicsItem )

protected:
	QPropertyAnimation fadeAnimation;

	QTimer timeout;

	virtual void hoverEnterEvent  (QGraphicsSceneHoverEvent *event);
	virtual void hoverLeaveEvent  (QGraphicsSceneHoverEvent *event);
	virtual void mousePressEvent  (QGraphicsSceneMouseEvent *event);
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	virtual void wheelEvent		  (QGraphicsSceneWheelEvent *event);
	virtual void mouseMoveEvent   (QGraphicsSceneMouseEvent *event);
	virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);


public:
	GraphicsItem();
	virtual ~GraphicsItem();

	virtual void setAnchorPoint( int x, int y ) {}
	virtual void setStartOpacity( double value ) ;

protected slots:
	void onTimeout();
};

}


#endif