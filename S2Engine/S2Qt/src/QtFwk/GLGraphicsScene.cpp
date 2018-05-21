// GraphicsScene.cpp
//
#include "GLGraphicsScene.h"
#include "GLGraphicsView.h"

#include "UserInteractionManager.h"
#include "MouseStatus.h"

//#include "OpenGL/OpenGL.h"
//#include "OpenGL/Renderer.h"

#include <QKeyEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneWheelEvent>
#include <QGraphicsSceneContextMenuEvent>

#include "Win32/MemCheck.h"

using namespace s2Qt;

// -----------------------------------------------------------------------------------------------
GLGraphicsScene::GLGraphicsScene( QWidget *parent )
: QGraphicsScene(parent)
, _view(parent)
{
	makeCurrent();

	//OpenGL::glInitExtensions();
	//renderer  = new OpenGL::Renderer;
	
	_uim      = new UserInteractionManager;
	_zoomMode = ZoomOnMouse;

	setSceneRect( parent->rect() );

	connect( this,SIGNAL(sceneRectChanged(const QRectF&)),SLOT(onResize(const QRectF&)) );
}

// -----------------------------------------------------------------------------------------------
GLGraphicsScene::~GLGraphicsScene()
{
	//delete _renderer;
	delete _uim;
}

// -----------------------------------------------------------------------------------------------
void GLGraphicsScene::setZoomMode( ZoomMode zm )
{
	_zoomMode = zm;
}

// -----------------------------------------------------------------------------------------------
ZoomMode GLGraphicsScene::zoomMode() const { return _zoomMode; }
bool GLGraphicsScene::hasFocus()     const { return _focused;  }

// -----------------------------------------------------------------------------------------------
void GLGraphicsScene::updateDrawables()
{}

// -----------------------------------------------------------------------------------------------
void GLGraphicsScene::onResize( const QRectF & rect )
{
	makeCurrent();

	//renderer->setViewport( 0,0,rect.width(), rect.height() );	

	resizeScene( rect.width(), rect.height() );
	updateDrawables();
}

// -----------------------------------------------------------------------------------------------
void GLGraphicsScene::resizeScene( int w, int h )
{}

// -----------------------------------------------------------------------------------------------
void GLGraphicsScene::trackMousePosition( const QPointF &p )
{
	const bool flipY = true;

	if( flipY ) _mousePosition = QPointF( p.x(),height()-p.y()-1 );
	else        _mousePosition = p;
}

// -----------------------------------------------------------------------------------------------
void GLGraphicsScene::onMouseMoved(){}
// -----------------------------------------------------------------------------------------------
void GLGraphicsScene::onMousePressed(){}
// -----------------------------------------------------------------------------------------------
void GLGraphicsScene::onMouseReleased(){}
// -----------------------------------------------------------------------------------------------
void GLGraphicsScene::onMouseDoubleClick(){}
// -----------------------------------------------------------------------------------------------
void GLGraphicsScene::onMouseWheel(){}
// -----------------------------------------------------------------------------------------------
void GLGraphicsScene::focusInEvent( QFocusEvent *e)  { QGraphicsScene::focusInEvent(e); }
// -----------------------------------------------------------------------------------------------
void GLGraphicsScene::focusOutEvent( QFocusEvent *e) { QGraphicsScene::focusOutEvent(e); }

// -----------------------------------------------------------------------------------------------
void GLGraphicsScene::mousePressEvent       ( QGraphicsSceneMouseEvent *e )
{
	// Propagate event to graphics items
	QGraphicsScene::mousePressEvent( e );

	if( e->isAccepted() )
		return;

	trackMousePosition( e->scenePos() );
	
	_uim->updateMouse(e,true);
	onMousePressed();
}

// -----------------------------------------------------------------------------------------------
void GLGraphicsScene::mouseMoveEvent        ( QGraphicsSceneMouseEvent *e )
{
	// Propagate event to graphics items
	QGraphicsScene::mouseMoveEvent( e );

	if( e->isAccepted() )
		return;

	trackMousePosition( e->scenePos() );

	_uim->updateMouse(e,false);

	onMouseMoved();
}

// -----------------------------------------------------------------------------------------------
void GLGraphicsScene::mouseReleaseEvent     ( QGraphicsSceneMouseEvent *e )
{
	// Propagate event to graphics items
	QGraphicsScene::mouseReleaseEvent( e );

	if( e->isAccepted() )
		return;

	trackMousePosition( e->scenePos() );

	_uim->updateMouse(e,true);

	onMouseReleased();
}

// -----------------------------------------------------------------------------------------------
void GLGraphicsScene::wheelEvent            ( QGraphicsSceneWheelEvent *e )
{
	QGraphicsScene::wheelEvent( e );
	if( e->isAccepted() )
		return;

	trackMousePosition( e->scenePos() );

	_uim->updateMouse(e,true);

	onMouseWheel();
}

// -----------------------------------------------------------------------------------------------
void GLGraphicsScene::mouseDoubleClickEvent ( QGraphicsSceneMouseEvent *e )
{
	QGraphicsScene::mouseDoubleClickEvent( e );

	if( e->isAccepted() )
		return;

	trackMousePosition( e->scenePos() );

	_uim->updateMouse(e,true);

	onMouseDoubleClick();
}

// -----------------------------------------------------------------------------------------------
void GLGraphicsScene::contextMenuEvent( QGraphicsSceneContextMenuEvent *e )
{}

// -----------------------------------------------------------------------------------------------
void GLGraphicsScene::keyPressEvent ( QKeyEvent *e )
{
	QGraphicsScene::keyPressEvent(e);
	if( e->isAccepted() )
		return;

	_uim->updateKeyboard(e,true);
}

// -----------------------------------------------------------------------------------------------
void GLGraphicsScene::keyReleaseEvent ( QKeyEvent *e )
{
	QGraphicsScene::keyReleaseEvent(e);
	if( e->isAccepted() )
		return;

	_uim->updateKeyboard(e,false);
}

// -----------------------------------------------------------------------------------------------
void GLGraphicsScene::makeCurrent()
{
	static_cast<GLGraphicsView*>(_view)->makeCurrent();
}