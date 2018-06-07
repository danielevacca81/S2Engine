// GestureStatus.cpp
//
#include "GestureStatus.h"

#include <iostream>
#include <QWidget>

using namespace s2Qt;

// ------------------------------------------------------------------------------------------------
GestureStatus::GestureStatus()
: _scaleFactor(1.0)
, _angleDeg(0.0)
, _gestureCode( None )
{}

// ------------------------------------------------------------------------------------------------
GestureStatus::Direction GestureStatus::direction() const
{
	if( _gestureCode & Swipe == 0 )
		return NoDirection;

	Direction d = (Direction)(_gestureCode & 0x000F);
	return d;
}

// ------------------------------------------------------------------------------------------------
void GestureStatus::update( QGestureEvent *e )
{
	_gestureCode = None;

	if( QGesture *g = e->gesture( Qt::SwipeGesture ) )
	{
        _gestureCode = Swipe;

		QSwipeGesture *swipe = static_cast<QSwipeGesture *>(g);
		//swipe->setGestureCancelPolicy( QGesture::CancelAllInContext );

		if( g->state() == Qt::GestureFinished )
		{        
			if( swipe->horizontalDirection() == QSwipeGesture::Left )   { std::cout << "LEFT" ; _gestureCode |= (unsigned int) Left; }
			if( swipe->horizontalDirection() == QSwipeGesture::Right )  { std::cout << "RIGHT"; _gestureCode |= (unsigned int) Right; }
			if( swipe->verticalDirection()   == QSwipeGesture::Up )     { std::cout << "UP"   ; _gestureCode |= (unsigned int) Up; }
			if( swipe->verticalDirection()   == QSwipeGesture::Down )   { std::cout << "DOWN" ; _gestureCode |= (unsigned int) Down; }
        }

		//if( g->state() == Qt::NoGesture )       std::cout << " no gesture";
		//if( g->state() == Qt::GestureStarted  ) std::cout << " started";
		//if( g->state() == Qt::GestureUpdated  ) std::cout << " updated";
		//if( g->state() == Qt::GestureFinished ) std::cout << " finished";
		//if( g->state() == Qt::GestureCanceled ) std::cout << " canceled";		
		std::cout << std::endl;

		e->accept( swipe );
	}
    //else if (QGesture *g = e->gesture(Qt::PanGesture))
    //{
    //    _gestureCode = Pan;

    //    QPanGesture *pan = static_cast<QPanGesture *>(g);
    //    _delta = e->mapToGraphicsScene(pan->delta());
    //    _center += _delta;
    //    std::wcout << L"QanGesture current (" << _center.x() << L":" << _center.y() << L")" << std::endl;

    //    std::wcout << L"PAN ";
    //    if (g->state() == Qt::NoGesture)       std::wcout << L"no gesture";
    //    if (g->state() == Qt::GestureStarted)  std::wcout << L"started";
    //    if (g->state() == Qt::GestureUpdated)  std::wcout << L"updated";
    //    if (g->state() == Qt::GestureFinished) std::wcout << L"finished";
    //    if (g->state() == Qt::GestureCanceled) std::wcout << L"canceled";
    //    std::wcout << std::endl;

    //    e->accept(pan);
    //}
	
	// @note: The QPinchGesture has a panning component
	if( QGesture *g = e->gesture( Qt::PinchGesture ) )
	{
        _gestureCode = Pinch;

		QPinchGesture *pinchGesture            = static_cast<QPinchGesture *>(g);
		QPinchGesture::ChangeFlags changeFlags = pinchGesture->changeFlags();

		if( changeFlags & QPinchGesture::RotationAngleChanged )
			_angleDeg    += pinchGesture->rotationAngle() - pinchGesture->lastRotationAngle();

		if( changeFlags & QPinchGesture::ScaleFactorChanged )
			_scaleFactor = pinchGesture->scaleFactor();

        if (changeFlags & QPinchGesture::CenterPointChanged)
        {
            QPointF p = e->mapToGraphicsScene(pinchGesture->centerPoint());
            _center = QPointF(p.x(), e->widget()->height() - p.y() - 1);
            p = e->mapToGraphicsScene(pinchGesture->lastCenterPoint());
            _prevCenter = QPointF(p.x(), e->widget()->height() - p.y() - 1);

            //std::wcout << L"QPinchGesture current (" << _center.x() << L":" << _center.y() << L")" << std::endl;
            //std::wcout << L"QPinchGesture prev (" << _prevCenter.x() << L":" << _prevCenter.y() << L")" << std::endl;
        }

#if 0
		std::wcout << L"PINCH ";
		if( g->state() == Qt::NoGesture )       std::wcout << L"no gesture";
		if( g->state() == Qt::GestureStarted  ) std::wcout << L"started";
		if( g->state() == Qt::GestureUpdated  ) std::wcout << L"updated";
        if (g->state() == Qt::GestureFinished)  std::wcout << L"finished";
		if( g->state() == Qt::GestureCanceled ) std::wcout << L"canceled";		
		std::wcout << std::endl;
#endif	
		e->accept( pinchGesture );
	}

    if (QGesture *g = e->gesture(Qt::TapGesture))
    {
        //std::wcout << L"TapGesture" << std::endl;
        QTapGesture *tapGesture = static_cast<QTapGesture *>(g);
        if (g->state() == Qt::GestureFinished)
        {
            _gestureCode = Tap;

            QPointF tapPoint = e->mapToGraphicsScene(tapGesture->position());
            _center = QPointF(tapPoint.x(), e->widget()->height() - tapPoint.y() - 1);
            _prevCenter = _center;
            
            //std::wcout << L"TapGesture (" << _center.x() << L":" << _center.y() << L")" << std::endl;
        }
        //e->accept(tapGesture);
    }

    if (QGesture *g = e->gesture(Qt::TapAndHoldGesture))
    {
        //std::wcout << L"TapAndHoldGesture" << std::endl;
        QTapAndHoldGesture *tapHoldGesture = static_cast<QTapAndHoldGesture *>(g);
        if (g->state() == Qt::GestureFinished)
        {
            _gestureCode = TapAndHold;

            QPointF tapPoint = e->mapToGraphicsScene(tapHoldGesture->position());
            _center = QPointF(tapPoint.x(), e->widget()->height() - tapPoint.y() - 1);
            _prevCenter = _center;

            //std::wcout << L"TapAndHoldGesture (" << _center.x() << L":" << _center.y() << L")" << std::endl;
        }
        e->accept(tapHoldGesture);
    }

    if( _gestureCode != None )
		e->accept();
}
