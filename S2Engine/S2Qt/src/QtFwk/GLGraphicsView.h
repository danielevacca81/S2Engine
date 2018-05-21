// GLGraphicsView.h
//
#ifndef S2QT_GLGRAPHICSVIEW_ONCE
#define S2QT_GLGRAPHICSVIEW_ONCE

#include "S2QtAPI.h"

#include <QGraphicsView>

class QResizeEvent;
class QGLWidget;

namespace s2Qt{

class S2QT_API GLGraphicsView : public QGraphicsView
{
private:
protected:
	void resizeEvent		( QResizeEvent *e );
	void leaveEvent			( QEvent *e);
	void enterEvent			( QEvent *e);

	static QGLWidget *sharedGLWidget;
public:
	static QGLWidget *getSharedGLWidget();

	GLGraphicsView( QWidget *parent, bool sharingContext  );
	GLGraphicsView( QWidget *parent, QGLWidget *customViewport );
	virtual ~GLGraphicsView() {}

	void makeCurrent();
};

}


#endif