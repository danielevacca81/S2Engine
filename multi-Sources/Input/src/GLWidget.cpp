// GLRenderWidget.cpp
//
#include "GLWidget.h"

using namespace s2::Qt;

// ------------------------------------------------------------------------------------------------
GLWidget::GLWidget( QWidget *parent )
: QOpenGLWidget( parent )
, _animating( false )
, _uim( this )
{
	//connect( this, &QObject::destroyed, this, &GLWidget::destroyGL );
	
	//connect( &_updateTimer, &QTimer::timeout, this, QOverload<>::of( &QOpenGLWidget::updateAnimations ) );
	connect( &_updateTimer, &QTimer::timeout, this, &GLWidget::updateAnimations );
	enableAnimation(10);
}

// ------------------------------------------------------------------------------------------------
void GLWidget::enableAnimation( unsigned int refreshTime )
{
	if( !_animating )
	{
		_updateTimer.setInterval( refreshTime );
		_updateTimer.start();
		_animating = true;
	}
	else
	{
		if( _updateTimer.interval() != refreshTime )
			_updateTimer.setInterval( refreshTime );
	}
}

// ------------------------------------------------------------------------------------------------
void GLWidget::disableAnimation()
{
	if( !_animating )
		return;
	
	_updateTimer.stop();
	_animating = false;
}

// ------------------------------------------------------------------------------------------------
bool GLWidget::isAnimationEnabled() const
{
	return _animating;
}

// ------------------------------------------------------------------------------------------------
void GLWidget::initializeGL()
{}

// ------------------------------------------------------------------------------------------------
void GLWidget::updateAnimations()
{}

// -----------------------------------------------------------------------------------------------
void GLWidget::mousePressEvent( QMouseEvent *e )
{
	QOpenGLWidget::mousePressEvent( e );
	if( e->isAccepted() )
		return;

	_uim.updateMouse( e, true );
}

// -----------------------------------------------------------------------------------------------
void GLWidget::mouseMoveEvent( QMouseEvent *e )
{
	QOpenGLWidget::mouseMoveEvent( e );
	if( e->isAccepted() )
		return;

	_uim.updateMouse( e, false );
}

// -----------------------------------------------------------------------------------------------
void GLWidget::mouseReleaseEvent( QMouseEvent *e )
{
	QOpenGLWidget::mouseReleaseEvent( e );
	if( e->isAccepted() )
		return;

	_uim.updateMouse( e, true );
}

// -----------------------------------------------------------------------------------------------
void GLWidget::wheelEvent( QWheelEvent *e )
{
	QOpenGLWidget::wheelEvent( e );
	if( e->isAccepted() )
		return;

	_uim.updateMouse( e, true );
}

// -----------------------------------------------------------------------------------------------
void GLWidget::mouseDoubleClickEvent( QMouseEvent *e )
{
	QOpenGLWidget::mouseDoubleClickEvent( e );
	if( e->isAccepted() )
		return;

	_uim.updateMouse( e, true );
}

// -----------------------------------------------------------------------------------------------
void GLWidget::contextMenuEvent( QContextMenuEvent *e )
{}

// -----------------------------------------------------------------------------------------------
void GLWidget::keyPressEvent( QKeyEvent *e )
{
	QOpenGLWidget::keyPressEvent( e );
	if( e->isAccepted() )
		return;

	_uim.updateKeyboard( e, true );
}

// -----------------------------------------------------------------------------------------------
void GLWidget::keyReleaseEvent( QKeyEvent *e )
{
	QOpenGLWidget::keyReleaseEvent( e );
	if( e->isAccepted() )
		return;

	_uim.updateKeyboard( e, false );
}