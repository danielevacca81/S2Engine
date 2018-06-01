// GLGraphicsSceneUpdater.cpp
//
#include "GLGraphicsSceneUpdater.h"

#include "GLGraphicsScene.h"

#include <QTimer>
#include <QElapsedTimer>
#ifdef _DEBUG
#include <iostream>
#endif
using namespace QtBridge;

// ------------------------------------------------------------------------------------------------
GLGraphicsSceneUpdater::GLGraphicsSceneUpdater( GLGraphicsScene *scene, unsigned int refreshTime )
//: QThread(scene)
: _running( false )
, _scene( scene )
, _refreshTime( refreshTime )
{
    _timer.setSingleShot(true);

    connect(&_timer, &QTimer::timeout, this, &GLGraphicsSceneUpdater::run);
}

// --------------------------------------------------------------------------------------------
GLGraphicsSceneUpdater::~GLGraphicsSceneUpdater()
{
	stop();
}

// --------------------------------------------------------------------------------------------
unsigned int GLGraphicsSceneUpdater::refreshTime() const { return _refreshTime; }

// --------------------------------------------------------------------------------------------
void GLGraphicsSceneUpdater::start()
{
	//if( _refreshTime <= 0 || _running )
    if (_running)
            return;

	if ( !_scene )
		return;

	_running = true;
	run();
	//QThread::start();
}

// --------------------------------------------------------------------------------------------
void GLGraphicsSceneUpdater::stop()
{
	_running = false;

    _timer.stop();
    QThread::msleep(_refreshTime * 2);
}
	
// --------------------------------------------------------------------------------------------
void GLGraphicsSceneUpdater::setRefreshTime( unsigned int msec )
{
    if (msec > 0 && !_running)
        start();

	if( msec == 0 && _running )
		stop();

	_refreshTime = msec;
}

// --------------------------------------------------------------------------------------------
void GLGraphicsSceneUpdater::run()
{
	//while( _running )
	if( _running )
	{
		QElapsedTimer frameTimer;
		frameTimer.start();

		_scene->refreshScene();			
		_scene->update();

		const int frameTime = frameTimer.elapsed();
#ifndef _DEBUG
        const
#endif
		int sleepTime = frameTime > _refreshTime ? 0 : _refreshTime - frameTime;
#ifdef _DEBUG
		/*if (sleepTime == 0) */sleepTime = frameTime + _refreshTime;
#endif // _DEBUG
        //std::cout << "Frametime = " << frameTime << " - Sleeptime = " << sleepTime << " - RefreshTime = " << _refreshTime << std::endl;

		if( _running )
			_timer.start( sleepTime );

		/*QThread::msleep( sleepTime );
		QThread::yieldCurrentThread();*/
	}
}