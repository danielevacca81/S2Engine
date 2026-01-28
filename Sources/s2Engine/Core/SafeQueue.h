// SafeQueue.h
//
#ifndef CORE_SAFEQUEUE_H
#define CORE_SAFEQUEUE_H

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

template <typename T>
class SafeQueue
{
public:
	SafeQueue() = default;
	// non copiable and not assignable
	SafeQueue( const SafeQueue & ) = delete;
	SafeQueue &operator=( const SafeQueue & ) = delete;

	// ------------------------------------------------------------------------------------------------
	void  enqueue( const T &item )
	{
		std::unique_lock<std::mutex> lock( _queueLock );
		_queueData.push( item );
		lock.unlock();
		_condition.notify_one();
	}

	// ------------------------------------------------------------------------------------------------
	void  enqueue( T &&item )
	{
		std::unique_lock<std::mutex> lock( _queueLock );
		_queueData.push( std::move( item ) );
		lock.unlock();
		_condition.notify_one();
	}

	// ------------------------------------------------------------------------------------------------
	bool dequeue( T& item )
	{
		std::unique_lock<std::mutex> lock( _queueLock );
		if( _queueData.empty() )
			_condition.wait( lock );

		// queue is empty after a clear
		if( _queueData.empty() )
			return false;

		item = _queueData.front();
		_queueData.pop();
		return true;
	}

	// ------------------------------------------------------------------------------------------------
	void clear()
	{
		std::unique_lock<std::mutex> lock( _queueLock );
		while( !_queueData.empty() )
			_queueData.pop();
		lock.unlock();
		_condition.notify_one();
	}

	// ------------------------------------------------------------------------------------------------
	bool isEmpty() const
	{
		std::lock_guard<std::mutex> lock( _queueLock );
		return _queueData.empty();
	}

	// ------------------------------------------------------------------------------------------------
	int  size()    const
	{
		std::lock_guard<std::mutex> lock( _queueLock );
		return (int) _queueData.size();
	}

private:
	std::queue<T>           _queueData;
	mutable std::mutex      _queueLock;
	std::condition_variable _condition;
};

#endif // !COMMON_SAFEQUEUE_H
