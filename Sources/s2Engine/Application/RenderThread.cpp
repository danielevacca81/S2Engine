// RenderThread.cpp
//
#include "RenderThread.h"

using namespace s2;

// ------------------------------------------------------------------------------------------------
void RenderThread::start( FrameJob initJob )
{
    {
        std::unique_lock lock( _mutex );
        if( _running )
            return; // already running

        _initJob   = std::move( initJob );
        _hasJob    = false;
        _frameDone = true;
        _running   = true;
    }

    _thread = std::thread( &RenderThread::threadLoop, this );
}

// ------------------------------------------------------------------------------------------------
void RenderThread::stop( FrameJob shutdownJob )
{
    {
        std::unique_lock lock( _mutex );
        if( !_running )
            return; // already stopped, nothing to do

        _shutdownJob = std::move( shutdownJob );
        _running     = false;
        // Do not clear _hasJob: if a job was enqueued just before stop(),
        // let the thread finish it before executing the shutdown job.
    }
    _jobReady.notify_one();

    if( _thread.joinable() )
        _thread.join();
}

// ------------------------------------------------------------------------------------------------
bool RenderThread::isRunning() const noexcept
{
    std::unique_lock lock( _mutex );
    return _running;
}

// ------------------------------------------------------------------------------------------------
void RenderThread::enqueueFrame( FrameJob job )
{
    std::unique_lock lock( _mutex );

    // Wait until the render thread has consumed the previous job.
    _jobDone.wait( lock, [this] { return _frameDone; } );

    _pendingJob = std::move( job );
    _hasJob     = true;
    _frameDone  = false;

    _jobReady.notify_one();
}

// ------------------------------------------------------------------------------------------------
void RenderThread::waitFrameComplete()
{
    std::unique_lock lock( _mutex );
    _jobDone.wait( lock, [this] { return _frameDone; } );
}

// ------------------------------------------------------------------------------------------------
void RenderThread::threadLoop()
{
    if( _initJob )
        _initJob();

    while( true )
    {
        FrameJob job;
        {
            std::unique_lock lock( _mutex );
            // Wait for a job OR a stop signal.
            _jobReady.wait( lock, [this] { return _hasJob || !_running; } );

            // If stopping and no pending job, exit the loop.
            if( !_running && !_hasJob )
                break;

            job     = std::move( _pendingJob );
            _hasJob = false;
        }

        if( job )
            job();

        // Signal job completion while holding the lock so waitFrameComplete()
        // cannot miss the notification.
        {
            std::unique_lock lock( _mutex );
            _frameDone = true;
        }
        _jobDone.notify_one();
    }

    // Last operation on this thread: run the shutdown job (e.g. release GL context).
    if( _shutdownJob )
        _shutdownJob();
}