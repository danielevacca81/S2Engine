// RenderThread.h
//
#ifndef APPLICATION_RENDERTHREAD_H
#define APPLICATION_RENDERTHREAD_H

#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <thread>

namespace s2 {

/// Manages a dedicated render thread that executes GL commands.
/// The main thread submits work via enqueueFrame(); synchronization
/// is handled internally so callers don't need to reason about locks.
class RenderThread
{
public:
    using FrameJob = std::function<void()>;

    RenderThread() = default;
    ~RenderThread() { stop(); }

    RenderThread( const RenderThread& ) = delete;
    RenderThread& operator=( const RenderThread& ) = delete;

    /// Start the render thread.
    /// initJob runs once on the render thread before the frame loop begins.
    void start( FrameJob initJob = {} );

    /// Signal the render thread to stop, run shutdownJob as its last operation,
    /// then block until the thread has fully exited.
    /// shutdownJob runs on the render thread after the frame loop exits.
    /// Safe to call multiple times: no-op if the thread is not running.
    void stop( FrameJob shutdownJob = {} );

    /// Submit a frame job. Blocks until the previous frame is consumed.
    void enqueueFrame( FrameJob job );

    /// Block until the current frame job has completed.
    void waitFrameComplete();

    bool isRunning() const noexcept;

private:
    void threadLoop();

private:
    std::thread             _thread;

    mutable std::mutex      _mutex;
    std::condition_variable _jobReady;
    std::condition_variable _jobDone;

    FrameJob                _initJob;
    FrameJob                _shutdownJob;
    FrameJob                _pendingJob;

    bool                    _running   { false }; // protected by _mutex
    bool                    _hasJob    { false }; // protected by _mutex
    bool                    _frameDone { true };  // protected by _mutex
};

} // namespace s2
#endif // APPLICATION_RENDERTHREAD_H