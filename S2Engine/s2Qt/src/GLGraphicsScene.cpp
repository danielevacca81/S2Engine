// GraphicsScene.cpp
//
#include "GLGraphicsScene.h"

#include "GLWidget.h"
#include "GLGraphicsSceneUpdater.h"
#include "UserInterface.h"

#include <QPainter>
#include <QKeyEvent>
#include <QGestureEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneWheelEvent>
#include <QGraphicsSceneContextMenuEvent>

#include <iostream>

using namespace s2::Qt;

// -----------------------------------------------------------------------------------------------
GLGraphicsScene::GLGraphicsScene( QWidget *parent )
: _glWidget( new GLWidget( parent ) )
, _ui( nullptr )
, _uim( parent )
{	
	_sceneUpdater = new GLGraphicsSceneUpdater( this, 16 );

	//_centralWidget->setPos( 0, 0 );
	//QGraphicsScene::addItem( _centralWidget );

	connect( this, &GLGraphicsScene::sceneRectChanged, this, &GLGraphicsScene::onResize );
	//connect( static_cast<GLWidget*>( _glWidget ), &GLWidget::openGLInitialized, this, &GLGraphicsScene::initializeOpenGL );
}

// -----------------------------------------------------------------------------------------------
GLGraphicsScene::~GLGraphicsScene()
{
	//delete _sceneUpdater; // needed?
}

// -----------------------------------------------------------------------------------------------
void GLGraphicsScene::setRefreshTime( unsigned int msec )
{
	_sceneUpdater->setRefreshTime( msec );
}

// -----------------------------------------------------------------------------------------------
unsigned int GLGraphicsScene::refreshTime() const
{
	return _sceneUpdater->refreshTime();
}

// -----------------------------------------------------------------------------------------------
void GLGraphicsScene::setInputPreferences( const UserInteractionManager::Preferences &p )
{
	_uim.setPreferences( p );

	// no need for the pan gesture if we handle the pan component in pinch
	//if( p.gesturePanEnabled )   _glWidget->grabGesture( ::Qt::PanGesture   );
	if( p.gestureSwipeEnabled   ) _glWidget->grabGesture( ::Qt::SwipeGesture );
	if( p.gesturePinchEnabled   ) _glWidget->grabGesture( ::Qt::PinchGesture );
	if( p.gestureTapEnabled     ) _glWidget->grabGesture( ::Qt::TapGesture );
    if( p.gestureTapHoldEnabled ) _glWidget->grabGesture( ::Qt::TapAndHoldGesture );
}

// -----------------------------------------------------------------------------------------------
void GLGraphicsScene::setUserInterface( UserInterface *ui )
{
	if( _ui )
		QGraphicsScene::removeItem( &_ui->_mainWidget );
	
	QGraphicsScene::addItem( &ui->_mainWidget );
	_ui = ui;
}


// -----------------------------------------------------------------------------------------------
void GLGraphicsScene::drawBackground( QPainter *painter, const QRectF &rect )
{
	if( _ui )
		_ui->adjustSize(); // @todo: adjust size in a resize event not in a paint event

	makeCurrent();
	painter->beginNativePainting();
	renderScene();
	painter->endNativePainting();
}

// -----------------------------------------------------------------------------------------------
void GLGraphicsScene::onResize( const QRectF & rect )
{
	makeCurrent();
	resizeScene( rect.width(), rect.height() );

	if( _ui )
		_ui->resize( rect );
}

// -----------------------------------------------------------------------------------------------
void GLGraphicsScene::onMouseEnter() {}
// -----------------------------------------------------------------------------------------------
void GLGraphicsScene::onMouseLeave() {}

// -----------------------------------------------------------------------------------------------
bool GLGraphicsScene::event( QEvent *e )
{
	if( e->type() == QEvent::Enter )
		onMouseEnter();

	if( e->type() == QEvent::Leave )
		onMouseLeave();

	if( e->type() == QEvent::Gesture )
		return gestureEvent( static_cast<QGestureEvent*>( e ) );
	
#ifdef VERBOSE_DEBUG
	qDebug() << e;
#endif

	return QGraphicsScene::event( e );
}

// -----------------------------------------------------------------------------------------------
void GLGraphicsScene::mousePressEvent( QGraphicsSceneMouseEvent *e )
{
	// Propagate event to graphics items
	QGraphicsScene::mousePressEvent( e );

	if( e->isAccepted() )
		return;

	_uim.updateMouse( e, true );
}

// -----------------------------------------------------------------------------------------------
void GLGraphicsScene::mouseMoveEvent( QGraphicsSceneMouseEvent *e )
{
	// Propagate event to graphics items
	QGraphicsScene::mouseMoveEvent( e );

	if( e->isAccepted() )
		return;

	_uim.updateMouse( e, false );
}

// -----------------------------------------------------------------------------------------------
void GLGraphicsScene::mouseReleaseEvent( QGraphicsSceneMouseEvent *e )
{
	// Propagate event to graphics items
	QGraphicsScene::mouseReleaseEvent( e );

	if( e->isAccepted() )
		return;

	_uim.updateMouse( e, true );
}

// -----------------------------------------------------------------------------------------------
void GLGraphicsScene::wheelEvent( QGraphicsSceneWheelEvent *e )
{
	QGraphicsScene::wheelEvent( e );
	
	if( e->isAccepted() )
		return;

	_uim.updateMouse( e, true );
}

// -----------------------------------------------------------------------------------------------
bool GLGraphicsScene::gestureEvent( QGestureEvent *event )
{
	_uim.updateGesture( event );
	return true;
}

// -----------------------------------------------------------------------------------------------
void GLGraphicsScene::mouseDoubleClickEvent( QGraphicsSceneMouseEvent *e )
{
	QGraphicsScene::mouseDoubleClickEvent( e );

	if( e->isAccepted() )
		return;
	_uim.updateMouse( e, true );
}

// -----------------------------------------------------------------------------------------------
void GLGraphicsScene::contextMenuEvent( QGraphicsSceneContextMenuEvent *e ) 
{}

// -----------------------------------------------------------------------------------------------
void GLGraphicsScene::keyPressEvent( QKeyEvent *e )
{
	QGraphicsScene::keyPressEvent( e );

	if( e->isAccepted() )
		return;

	_uim.updateKeyboard( e, true );
}

// -----------------------------------------------------------------------------------------------
void GLGraphicsScene::keyReleaseEvent( QKeyEvent *e )
{
	QGraphicsScene::keyReleaseEvent( e );
	if( e->isAccepted() )
		return;

	_uim.updateKeyboard( e, false );
}

// -----------------------------------------------------------------------------------------------
void GLGraphicsScene::makeCurrent()
{
	static_cast<QOpenGLWidget*>( _glWidget )->makeCurrent();
}