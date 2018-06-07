// GLGraphicsView.cpp
//
#include "GLGraphicsView.h"

#include "GLGraphicsScene.h"
#include "GLGraphicsSceneUpdater.h"

#include <QLayout>
#include <QThread>
#include <QResizeEvent>

#include <iostream>

using namespace s2Qt;

// ------------------------------------------------------------------------------------------------
GLGraphicsView::GLGraphicsView( QWidget *parent, QFrame::Shape shape )
: QGraphicsView( parent )
{
	setViewportUpdateMode( QGraphicsView::FullViewportUpdate );
	setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
	setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
	setMouseTracking( true );
	setFrameShape( shape );
}

// ------------------------------------------------------------------------------------------------
GLGraphicsView::GLGraphicsView( GLGraphicsScene *scene, QWidget *parent, QFrame::Shape shape )
: QGraphicsView( parent )
{
	setViewportUpdateMode( QGraphicsView::FullViewportUpdate );
	setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
	setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
	setMouseTracking( true );
	setFrameShape( shape );

	setScene( scene );
}

// ------------------------------------------------------------------------------------------------
void GLGraphicsView::setScene( GLGraphicsScene *scene )
{
	QGraphicsView::setScene( scene );

	setViewport( scene->_glWidget );
	scene->setSceneRect( rect() );
}

// ------------------------------------------------------------------------------------------------
void GLGraphicsView::showEvent( QShowEvent *e )
{
	// spontaneous showevents are sent from the underlying windows system (OS)
	// we need to handle only internal showevents

	// @todo: add here first resize event handling
	if( !e->spontaneous() )
		// start the updater as soon as the view is shown
		static_cast<GLGraphicsScene*>( scene() )->_sceneUpdater->start();

	QGraphicsView::showEvent( e );
}

// ------------------------------------------------------------------------------------------------
void GLGraphicsView::resizeEvent( QResizeEvent *event )
{
	if( scene() && isVisible() )
		scene()->setSceneRect( QRectF( QPointF( 0, 0 ), event->size() ) );

	QGraphicsView::resizeEvent( event );
	event->accept();
}

// ------------------------------------------------------------------------------------------------
void GLGraphicsView::leaveEvent( QEvent *e )
{
	QGraphicsView::leaveEvent( e );

	if( scene() )
		scene()->clearFocus();
}

// ------------------------------------------------------------------------------------------------
void GLGraphicsView::enterEvent( QEvent *e )
{
	QGraphicsView::enterEvent( e );

	if( scene() )
		scene()->setFocus( Qt::OtherFocusReason );
}