// GLGraphicsView.cpp
//
#include "GLGraphicsView.h"

#include <QGLWidget>
#include <QLayout>
#include <QResizeEvent>

#include "Win32/MemCheck.h"

using namespace s2Qt;

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
QGLWidget *GLGraphicsView::sharedGLWidget = NULL;

QGLWidget *GLGraphicsView::getSharedGLWidget()
{
	if( !sharedGLWidget )
	{
		QGLFormat f = QGLFormat::defaultFormat();
		f.setSampleBuffers(true);
		f.setAlpha(true);
		QGLFormat::setDefaultFormat(f);

		sharedGLWidget = new QGLWidget(f);
		
		sharedGLWidget->makeCurrent();
	}
	

	return sharedGLWidget;
}
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------
GLGraphicsView::GLGraphicsView( QWidget *parent, bool sharingContext )
: QGraphicsView( parent )
{
	QGLWidget *glWidget	 = new QGLWidget( this, sharingContext ? getSharedGLWidget():0 );
	
	setViewport(glWidget);
	setViewportUpdateMode( QGraphicsView::FullViewportUpdate );
	setCacheMode(QGraphicsView::CacheNone);
	setMouseTracking(true);

	makeCurrent();
}

// ------------------------------------------------------------------------------------------------
GLGraphicsView::GLGraphicsView( QWidget *parent, QGLWidget *customViewport )
: QGraphicsView( parent )
{
	setViewport(customViewport);
	setViewportUpdateMode( QGraphicsView::FullViewportUpdate );
	setCacheMode(QGraphicsView::CacheNone);
	setMouseTracking(true);

	makeCurrent();
}

// ------------------------------------------------------------------------------------------------
void GLGraphicsView::resizeEvent(QResizeEvent *event)
{
	if( scene() && isVisible() )
		scene()->setSceneRect( QRectF( QPointF(0, 0), event->size()) );
	
	QGraphicsView::resizeEvent(event);
	event->accept();
}

// ------------------------------------------------------------------------------------------------
void GLGraphicsView::leaveEvent			( QEvent *e)
{
	QGraphicsView::leaveEvent(e);

	if( scene() )
		scene()->clearFocus();
}

// ------------------------------------------------------------------------------------------------
void GLGraphicsView::enterEvent			( QEvent *e)
{
	QGraphicsView::enterEvent(e);

	if( scene() )
		scene()->setFocus(Qt::OtherFocusReason);
}

// ------------------------------------------------------------------------------------------------
void GLGraphicsView::makeCurrent()
{
	if( viewport() )
		((QGLWidget *)viewport())->makeCurrent();
}
