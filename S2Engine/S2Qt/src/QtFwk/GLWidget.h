// GLWidget.h
// 
#ifndef S2QT_GLWIDGET_ONCE
#define S2QT_GLWIDGET_ONCE

#include "S2QtAPI.h"

#include "Settings.h"

#include <QGLWidget>

namespace s2Qt {

class UserInteractionManager;

class S2QT_API GLWidget : public QGLWidget
{
	Q_OBJECT

protected:
	void initializeGL();
	void paintGL();
	void resizeGL(int width, int height);
	
	UserInteractionManager *_uim;

	ZoomMode           _zoomMode;
	bool               _focused;
	QPointF            _mousePosition;

	void trackMousePosition( const QPointF &mousePosition );

	//virtual void focusInEvent          ( QFocusEvent *e);
	//virtual void focusOutEvent         ( QFocusEvent *e);
	
	void mousePressEvent       ( QMouseEvent *e );
	void mouseMoveEvent        ( QMouseEvent *e );
	void mouseReleaseEvent     ( QMouseEvent *e );
	void wheelEvent            ( QWheelEvent *e );
	void mouseDoubleClickEvent ( QMouseEvent *e );
	
	//virtual void contextMenuEvent      ( QGraphicsSceneContextMenuEvent *e);
	
	//virtual void keyPressEvent	   ( QKeyEvent *e );
	//virtual void keyReleaseEvent	   ( QKeyEvent *e );

public:
	GLWidget( const QGLFormat &fmt, QWidget *parent = 0 );
	virtual ~GLWidget();

	QSize minimumSizeHint() const;
	QSize sizeHint()        const;	
	
	// ================================================================================================
	//                                              QT Events
	// ================================================================================================
	virtual void onMouseMoved();
	virtual void onMousePressed();
	virtual void onMouseReleased();
	virtual void onMouseWheel();
	virtual void onMouseDoubleClick();
	// ================================================================================================
	//                                              QT Events
	// ================================================================================================
};


}


#endif