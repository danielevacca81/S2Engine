// GraphicsItem.h
//
#ifndef QTBRIDGE_GRAPHICSITEM_ONCE
#define QTBRIDGE_GRAPHICSITEM_ONCE

#include "s2Qt_API.h"

#include <QGraphicsItem>
#include <QPropertyAnimation>
#include <QTimer>

namespace s2 {

namespace Qt {


class S2QT_API GraphicsItem : public QObject, public QGraphicsItem
{
	Q_OBJECT
	Q_PROPERTY(double opacity READ opacity WRITE setOpacity)
	Q_INTERFACES( QGraphicsItem )

protected:
	QPropertyAnimation fadeAnimation;

	QTimer timeout;

	virtual void hoverEnterEvent      (QGraphicsSceneHoverEvent *event);
	virtual void hoverLeaveEvent      (QGraphicsSceneHoverEvent *event);
	virtual void mousePressEvent      (QGraphicsSceneMouseEvent *event);
	virtual void mouseReleaseEvent    (QGraphicsSceneMouseEvent *event);
	virtual void wheelEvent		      (QGraphicsSceneWheelEvent *event);
	virtual void mouseMoveEvent       (QGraphicsSceneMouseEvent *event);
	virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);


public:
	GraphicsItem();
	virtual ~GraphicsItem();

	virtual void setAnchorPoint( int x, int y ) {}
	virtual void setStartOpacity( double value ) ;

protected slots:
	void onTimeout();
};

}}


#endif