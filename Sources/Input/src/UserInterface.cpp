// UserInterface.cpp
//
#include "UserInterface.h"

#include <QGraphicsGridLayout>

using namespace s2::Qt;

// -----------------------------------------------------------------------------------------------
UserInterface::UserInterface( QWidget *parent )
//: _mainWidget( new QGraphicsWidget )
{
	_mainWidget.setPos( 0, 0 );
}

// -----------------------------------------------------------------------------------------------
UserInterface::~UserInterface()
{}

// -----------------------------------------------------------------------------------------------
void UserInterface::setLayout( QGraphicsLayout *layout )
{
	_mainWidget.setLayout( layout );
}

// -----------------------------------------------------------------------------------------------
void UserInterface::resize( const QRectF &size )
{
	_mainWidget.setPreferredSize( size.size() );
}

// -----------------------------------------------------------------------------------------------
void UserInterface::adjustSize()
{
	_mainWidget.adjustSize();
}