// GraphicsScene.cpp
//
#include "GLGraphicsScene.h"

#include "GLWidget.h"
#include "GLGraphicsSceneUpdater.h"

#include <QDebug>
#include <QThread>
#include <QElapsedTimer>
#include <QGraphicsView>
#include <QPainter>
#include <QKeyEvent>
#include <QGestureEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneWheelEvent>
#include <QGraphicsSceneContextMenuEvent>

#include <iostream>

using namespace QtBridge;

// -----------------------------------------------------------------------------------------------
GLGraphicsScene::GLGraphicsScene( QWidget *parent )
//: _glWidget( new GLWidget( parent ) )
: _glWidget( new GLWidget( parent ) )
, _centralWidget( new QGraphicsWidget )
{
	_centralWidget->setPos( 0, 0 );
	
	_sceneUpdater = new GLGraphicsSceneUpdater( this, 16 );

	QGraphicsScene::addItem( _centralWidget );

	connect( this, &GLGraphicsScene::sceneRectChanged, this, &GLGraphicsScene::onResize );
	connect( static_cast<GLWidget*>( _glWidget ), &GLWidget::openGLInitialized, this, &GLGraphicsScene::initializeOpenGL );
}

// -----------------------------------------------------------------------------------------------
GLGraphicsScene::~GLGraphicsScene()
{
	delete _sceneUpdater; // needed?
}

// -----------------------------------------------------------------------------------------------
void GLGraphicsScene::initializeOpenGL()
{
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
	//if( p.gesturePanEnabled )   _glWidget->grabGesture( Qt::PanGesture   );
	if( p.gestureSwipeEnabled   ) _glWidget->grabGesture( Qt::SwipeGesture );
	if( p.gesturePinchEnabled   ) _glWidget->grabGesture( Qt::PinchGesture );
	if( p.gestureTapEnabled     ) _glWidget->grabGesture( Qt::TapGesture );
    if( p.gestureTapHoldEnabled ) _glWidget->grabGesture( Qt::TapAndHoldGesture );
}

// -----------------------------------------------------------------------------------------------
void GLGraphicsScene::addItem( QGraphicsItem *item, QGraphicsLayout *layout )
{
	if( layout )
		_centralWidget->setLayout( layout );
	else
		QGraphicsScene::addItem( item );
}

// -----------------------------------------------------------------------------------------------
void GLGraphicsScene::drawBackground( QPainter *painter, const QRectF &rect )
{
	_centralWidget->adjustSize(); // @todo: adjust size in a resize event not in a paint event

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

	_centralWidget->setPreferredSize( rect.size() ); // @todo: handle resize here avoiding recursive calls
}

// -----------------------------------------------------------------------------------------------
//void GLGraphicsScene::onMouseMoved() {}
//// -----------------------------------------------------------------------------------------------
//void GLGraphicsScene::onMousePressed() {}
//// -----------------------------------------------------------------------------------------------
//void GLGraphicsScene::onMouseReleased() {}
//// -----------------------------------------------------------------------------------------------
//void GLGraphicsScene::onMouseDoubleClick() {}
//// -----------------------------------------------------------------------------------------------
//void GLGraphicsScene::onMouseWheel() {}
//// -----------------------------------------------------------------------------------------------
//void GLGraphicsScene::onPinch() {}
//// -----------------------------------------------------------------------------------------------
//void GLGraphicsScene::onSwipe() {}
// -----------------------------------------------------------------------------------------------
void GLGraphicsScene::onMouseEnter() {}
// -----------------------------------------------------------------------------------------------
void GLGraphicsScene::onMouseLeave() {}

// -----------------------------------------------------------------------------------------------
//void GLGraphicsScene::focusInEvent( QFocusEvent *e )
//{ 
//	QGraphicsScene::focusInEvent( e ); 
//	
//	makeCurrent();
//}

// -----------------------------------------------------------------------------------------------
//void GLGraphicsScene::focusOutEvent( QFocusEvent *e )
//{ 
//	QGraphicsScene::focusOutEvent( e ); 
//}

// -----------------------------------------------------------------------------------------------
bool GLGraphicsScene::event( QEvent *e )
{
	//makeCurrent();


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
	//makeCurrent();
	
	// Propagate event to graphics items
	QGraphicsScene::mousePressEvent( e );

	if( e->isAccepted() )
		return;

	//trackMousePosition( e->scenePos() );

	_uim.updateMouse( e, true );
}

// -----------------------------------------------------------------------------------------------
void GLGraphicsScene::mouseMoveEvent( QGraphicsSceneMouseEvent *e )
{
	//makeCurrent();
	//auto listView = views();
	//bool track = listView.at( 0 )->hasMouseTracking();
	
	// Propagate event to graphics items
	QGraphicsScene::mouseMoveEvent( e );

	if( e->isAccepted() )
		return;

	//trackMousePosition( e->scenePos() );

	_uim.updateMouse( e, false );

	//onMouseMoved();
}

// -----------------------------------------------------------------------------------------------
void GLGraphicsScene::mouseReleaseEvent( QGraphicsSceneMouseEvent *e )
{
	//makeCurrent();
	
	// Propagate event to graphics items
	QGraphicsScene::mouseReleaseEvent( e );

	if( e->isAccepted() )
		return;

	//trackMousePosition( e->scenePos() );

	_uim.updateMouse( e, true );

	//onMouseReleased();
}

// -----------------------------------------------------------------------------------------------
void GLGraphicsScene::wheelEvent( QGraphicsSceneWheelEvent *e )
{
	//makeCurrent();
	
	QGraphicsScene::wheelEvent( e );
	
	if( e->isAccepted() )
		return;

	//trackMousePosition( e->scenePos() );

	_uim.updateMouse( e, true );
	//qDebug() << e;

	//onMouseWheel();
}

// -----------------------------------------------------------------------------------------------
bool GLGraphicsScene::gestureEvent( QGestureEvent *event )
{
	//makeCurrent();
	_uim.updateGesture( event );
	return true;
}

// -----------------------------------------------------------------------------------------------
void GLGraphicsScene::mouseDoubleClickEvent( QGraphicsSceneMouseEvent *e )
{
	//makeCurrent();
	
	QGraphicsScene::mouseDoubleClickEvent( e );

	if( e->isAccepted() )
		return;

	//trackMousePosition( e->scenePos() );

	_uim.updateMouse( e, true );

	//onMouseDoubleClick();
}

// -----------------------------------------------------------------------------------------------
void GLGraphicsScene::contextMenuEvent( QGraphicsSceneContextMenuEvent *e ) {}

// -----------------------------------------------------------------------------------------------
void GLGraphicsScene::keyPressEvent( QKeyEvent *e )
{
	//makeCurrent();
	
	QGraphicsScene::keyPressEvent( e );
	if( e->isAccepted() )
		return;

	_uim.updateKeyboard( e, true );
}

// -----------------------------------------------------------------------------------------------
void GLGraphicsScene::keyReleaseEvent( QKeyEvent *e )
{
	//makeCurrent();
	
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