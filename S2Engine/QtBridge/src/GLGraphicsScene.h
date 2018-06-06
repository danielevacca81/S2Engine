// GLGraphicsScene.h
// 
#ifndef GUITOOLS_GLGRAPHICSSCENE_H
#define GUITOOLS_GLGRAPHICSSCENE_H

#include "QtBridge_API.h"

#include "UserInteractionManager.h"

#include <QGraphicsScene>
#include <QGraphicsWidget>

namespace QtBridge{

class GLGraphicsView;
class GLGraphicsSceneUpdater;

class QTBRIDGE_API GLGraphicsScene : public QGraphicsScene
{
	Q_OBJECT

public:
	GLGraphicsScene( QWidget *parent  = Q_NULLPTR );
	virtual ~GLGraphicsScene();

    virtual void addItem( QGraphicsItem *item, QGraphicsLayout *layout );

	virtual void resizeScene( int w, int h ) = 0;
    virtual void renderScene() = 0;
	virtual void refreshScene() = 0;
	
	virtual void setInputPreferences( const UserInteractionManager::Preferences &p );
	virtual void setRefreshTime( unsigned int msec );

	//virtual void onMouseMoved();
	//virtual void onMousePressed();
	//virtual void onMouseReleased();
	//virtual void onMouseWheel();
	//virtual void onMouseDoubleClick();
	//virtual void onPinch();
	//virtual void onSwipe();
	virtual void onMouseEnter();
	virtual void onMouseLeave();

	unsigned int refreshTime() const;

protected:
	virtual void makeCurrent();
	virtual void drawBackground(QPainter *painter, const QRectF &rect);
    //virtual void focusInEvent( QFocusEvent *e );
    //virtual void focusOutEvent( QFocusEvent *e );
    virtual void mousePressEvent( QGraphicsSceneMouseEvent *e );
    virtual void mouseMoveEvent( QGraphicsSceneMouseEvent *e );
    virtual void mouseReleaseEvent( QGraphicsSceneMouseEvent *e );
    virtual void wheelEvent( QGraphicsSceneWheelEvent *e );
    virtual void mouseDoubleClickEvent( QGraphicsSceneMouseEvent *e );
    virtual void contextMenuEvent( QGraphicsSceneContextMenuEvent *e );
    virtual void keyPressEvent( QKeyEvent *e );
    virtual void keyReleaseEvent( QKeyEvent *e );
    virtual bool event( QEvent *e );
    virtual bool gestureEvent( QGestureEvent *event );

private slots:
	void onResize( const QRectF &r );
	virtual void initializeOpenGL();

protected:
	friend class GLSceneUpdateThread;
    friend class GLGraphicsView;

    GLGraphicsSceneUpdater *_sceneUpdater;
    QGraphicsWidget        *_centralWidget;
    QWidget                *_glWidget;
    UserInteractionManager  _uim;
};


}


#endif