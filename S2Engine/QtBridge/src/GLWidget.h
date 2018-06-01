// GLWidget.h
//
#ifndef GUITOOLS_GLWIDGET_H
#define GUITOOLS_GLWIDGET_H

#include "QtBridge_API.h"

#include <QGLWidget>

namespace QtBridge{


class GLWidget: public QGLWidget
{
public:
    GLWidget( QWidget *parent = Q_NULLPTR );
	virtual ~GLWidget();

protected:
    virtual void initializeGL();
};

}


#endif