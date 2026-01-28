// UserInterface.h
//
#ifndef S2QT_USERITERFACE_H
#define S2QT_USERITERFACE_H

#include "s2Qt_API.h"

#include <QGraphicsItem>
#include <QGraphicsLayout>
#include <QGraphicsWidget>


namespace s2 {
namespace Qt {

class GLGraphicsScene;

class S2QT_API UserInterface
{
public:
	UserInterface( QWidget *parent = Q_NULLPTR );
	~UserInterface();

    void setLayout( QGraphicsLayout *layout );


private:
	void resize( const QRectF &size );
	void adjustSize();

private:
	friend class GLGraphicsScene;

    QGraphicsWidget _mainWidget;
};

}}
#endif // !S2QT_USERITERFACE_H