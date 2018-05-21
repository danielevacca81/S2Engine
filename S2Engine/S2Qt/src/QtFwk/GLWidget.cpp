// GLWidget.cpp
//
#include "GLWidget.h"

#include "UserInteractionManager.h"
#include "MouseStatus.h"

#include <QMouseEvent>

using namespace s2Qt;

// ------------------------------------------------------------------------------------------------
GLWidget::GLWidget( const QGLFormat &fmt, QWidget *parent)
: QGLWidget(fmt, parent)
{
	_uim      = new UserInteractionManager;
	_zoomMode = ZoomOnMouse;
}

// ------------------------------------------------------------------------------------------------
GLWidget::~GLWidget()
{}

// ------------------------------------------------------------------------------------------------
QSize GLWidget::minimumSizeHint() const { return QSize(50, 50);  }
QSize GLWidget::sizeHint()        const { return QSize(400, 400);}

// ------------------------------------------------------------------------------------------------
void GLWidget::initializeGL()
{}

// ------------------------------------------------------------------------------------------------
void GLWidget::paintGL()
{}

// ------------------------------------------------------------------------------------------------
void GLWidget::resizeGL(int width, int height)
{}

// ------------------------------------------------------------------------------------------------
void GLWidget::mousePressEvent( QMouseEvent *e )
{
	QGLWidget::mousePressEvent( e );

	if( e->isAccepted() )
		return;

	trackMousePosition( e->posF() );

	_uim->updateMouse(e,true);
	onMousePressed();
}

// ------------------------------------------------------------------------------------------------
void GLWidget::mouseMoveEvent( QMouseEvent *e )
{
	QGLWidget::mouseMoveEvent( e );

	if( e->isAccepted() )
		return;

	trackMousePosition( e->posF() );

	_uim->updateMouse(e,false);
	onMouseMoved();
}

// ------------------------------------------------------------------------------------------------
void GLWidget::mouseReleaseEvent( QMouseEvent *e )
{
	QGLWidget::mouseReleaseEvent( e );

	if( e->isAccepted() )
		return;

	trackMousePosition( e->posF() );

	_uim->updateMouse(e,true);
	onMousePressed();
}

// ------------------------------------------------------------------------------------------------
void GLWidget::wheelEvent( QWheelEvent *e )
{
	QGLWidget::wheelEvent( e );
	if( e->isAccepted() )
		return;

	trackMousePosition( e->pos() );

	_uim->updateMouse(e,true);

	onMouseWheel();
}

// ------------------------------------------------------------------------------------------------
void GLWidget::mouseDoubleClickEvent( QMouseEvent *e )
{
	QGLWidget::mouseDoubleClickEvent( e );

	if( e->isAccepted() )
		return;

	trackMousePosition( e->posF() );

	_uim->updateMouse(e,true);

	onMouseDoubleClick();
}

// -----------------------------------------------------------------------------------------------
void GLWidget::trackMousePosition( const QPointF &p )
{
	const bool flipY = true;

	if( flipY ) _mousePosition = QPointF( p.x(),height()-p.y()-1 );
	else        _mousePosition = p;
}

// ================================================================================================
//                                              QT Events
// ================================================================================================
void GLWidget::onMouseMoved(){}
void GLWidget::onMousePressed(){}
void GLWidget::onMouseReleased(){}
void GLWidget::onMouseWheel(){}
void GLWidget::onMouseDoubleClick(){}
// ================================================================================================
//                                              QT Events
// ================================================================================================
