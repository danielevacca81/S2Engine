// GLGraphicsScene.h
// 
#ifndef S2QT_GLGRAPHICSSCENE_ONCE
#define S2QT_GLGRAPHICSSCENE_ONCE

#include "S2QtAPI.h"

#include "Settings.h"

#include <QGraphicsScene>

class QGraphicsSceneMouseEvent;
class QGraphicsSceneWheelEvent;
class QGraphicsSceneContextMenuEvent;

namespace s2Qt {

class UserInteractionManager;

class S2QT_API GLGraphicsScene : public QGraphicsScene
{
	Q_OBJECT

protected:
	UserInteractionManager *_uim;
	QWidget				   *_view;

	ZoomMode           _zoomMode;
	bool               _focused;
	QPointF            _mousePosition;

	void trackMousePosition( const QPointF &mousePosition );

	virtual void focusInEvent          ( QFocusEvent *e);
	virtual void focusOutEvent         ( QFocusEvent *e);
	virtual void mousePressEvent       ( QGraphicsSceneMouseEvent *e );
	virtual void mouseMoveEvent        ( QGraphicsSceneMouseEvent *e );
	virtual void mouseReleaseEvent     ( QGraphicsSceneMouseEvent *e );
	virtual void wheelEvent            ( QGraphicsSceneWheelEvent *e );
	virtual void mouseDoubleClickEvent ( QGraphicsSceneMouseEvent *e );
	virtual void contextMenuEvent      ( QGraphicsSceneContextMenuEvent *e);
	virtual void keyPressEvent		   ( QKeyEvent *e );
	virtual void keyReleaseEvent	   ( QKeyEvent *e );

public:
	GLGraphicsScene( QWidget *parent );
	virtual ~GLGraphicsScene();

	virtual void postInitialization() {}
	virtual void drawBackground( QPainter *painter, const QRectF &rect ) = 0;
	virtual void makeCurrent();
	virtual void updateDrawables();
	virtual void setZoomMode( ZoomMode zm );

	virtual ZoomMode zoomMode() const;
	virtual bool     hasFocus() const;

	
	
// ================================================================================================
//                                              QT Events
// ================================================================================================
	virtual void onMouseMoved();
	virtual void onMousePressed();
	virtual void onMouseReleased();
	virtual void onMouseWheel();
	virtual void onMouseDoubleClick();
	virtual void resizeScene( int w, int h );

private slots:
	void onResize( const QRectF &r );
// ================================================================================================
//                                              QT Events
// ================================================================================================
};


}


#endif