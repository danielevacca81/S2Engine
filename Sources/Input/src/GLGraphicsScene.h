// GLGraphicsScene.h
// 
#ifndef GUITOOLS_GLGRAPHICSSCENE_H
#define GUITOOLS_GLGRAPHICSSCENE_H

#include "s2Qt_API.h"

#include "UserInteractionManager.h"

#include <QGraphicsScene>
#include <QGraphicsWidget>

namespace s2 {

namespace Qt {

class UserInterface;
class GLGraphicsView;
class GLGraphicsSceneUpdater;


class S2QT_API GLGraphicsScene : public QGraphicsScene
{
	Q_OBJECT

public:
	GLGraphicsScene( QWidget *parent  = Q_NULLPTR );
	virtual ~GLGraphicsScene();

	virtual void resizeScene( int w, int h ) = 0;
    virtual void renderScene() = 0;
	virtual void refreshScene() = 0;
	
	virtual void setUserInterface( UserInterface *ui );
	virtual void setInputPreferences( const UserInteractionManager::Preferences &p );
	virtual void setRefreshTime( unsigned int msec );


	unsigned int refreshTime() const;

protected:
	virtual void onMouseEnter();
	virtual void onMouseLeave();

	virtual void makeCurrent();
	virtual void drawBackground(QPainter *painter, const QRectF &rect);
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
	virtual void initializeOpenGL() = 0;
	void onResize( const QRectF &r );

protected:
	friend class GLSceneUpdateThread;
    friend class GLGraphicsView;

    GLGraphicsSceneUpdater *_sceneUpdater;
    QWidget                *_glWidget;
    UserInteractionManager  _uim;
	UserInterface           *_ui;
};


}}


#endif