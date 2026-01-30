// SafeQueue.h
//
#ifndef CORE_SAFEQUEUE_H
#define CORE_SAFEQUEUE_H

#include <deque>
#include <algorithm>
#include <thread>
#include <mutex>
#include <condition_variable>

namespace s2 {

template <typename T>
class SafeQueue
{
public:
	SafeQueue() = default;
	~SafeQueue() { shutdown(); }

	// non copyable and not assignable
	SafeQueue( const SafeQueue& ) = delete;
	SafeQueue& operator=( const SafeQueue& ) = delete;

	// ------------------------------------------------------------------------------------------------
	void  enqueue( const T& item )
	{
		{
			std::unique_lock lock( _queueLock );
			if( _shutdown )
				return;
			_queueData.push_back( item );
		}
		_condition.notify_one();
	}

	// ------------------------------------------------------------------------------------------------
	void  enqueue( T&& item )
	{
		{
			std::unique_lock lock( _queueLock );
			if( _shutdown )
				return;
			_queueData.push_back( std::move( item ) );
		}
		_condition.notify_one();
	}

	// ------------------------------------------------------------------------------------------------
	void enqueueSome( std::vector<T>& items )
	{
		if( items.empty() )
			return;

		{
			std::lock_guard lock( _queueLock );
			if( _shutdown ) 
				return;
			_queueData.insert( _queueData.end(), items.begin(), items.end() );
		}
		_condition.notify_all();
	}

	// ------------------------------------------------------------------------------------------------
	void pushFront( const T& item )
	{
		{
			std::unique_lock lock( _queueLock );
			if( _shutdown )
				return;
			_queueData.push_front( item );
		}
		_condition.notify_one();
	}

	// ------------------------------------------------------------------------------------------------
	void pushFront( T&& item )
	{
		{
			std::unique_lock lock( _queueLock );
			if( _shutdown )
				return;
			_queueData.push_front( std::move( item ) );
		}
		_condition.notify_one();
	}

	// ------------------------------------------------------------------------------------------------
	bool dequeue( T& item )
	{
		std::unique_lock lock( _queueLock );

		_condition.wait( lock, [this]
		{
			return !_queueData.empty() || _shutdown;
		} );

		if( _shutdown && _queueData.empty() )
			return false;

		item = std::move( _queueData.front() );
		_queueData.pop_front();
		return true;
	}

	// ------------------------------------------------------------------------------------------------
	void dequeueBlocking( T& item )
	{
		std::unique_lock lock( _queueLock );
		_condition.wait( lock, [this] { return !_queueData.empty() || _shutdown; } );

		if( _shutdown && _queueData.empty() )
			throw std::runtime_error( "SafeQueue: dequeue on shutdown queue" );

		item = std::move( _queueData.front() );
		_queueData.pop_front();
	}

	// ------------------------------------------------------------------------------------------------
	template< typename UnaryPredicate >
	void dequeueIf( T& item, UnaryPredicate pred )
	{
		std::unique_lock lock( _queueLock );

		auto it = _queueData.end();
		_condition.wait( lock, [this, &it, &pred]
		{
			if( _shutdown ) return true;
			it = std::find_if( _queueData.begin(), _queueData.end(), pred );
			return it != _queueData.end();
		} );

		if( _shutdown && it == _queueData.end() )
			return false;

		item = std::move( *it );
		_queueData.erase( it );
		return true;
	}

	// ------------------------------------------------------------------------------------------------
	std::vector<T> dequeueAll()
	{
		std::vector<T> retval;

		std::lock_guard lock( _queueLock );
		retval.reserve( _queueData.size() );
		std::move( _queueData.begin(), _queueData.end(), std::back_inserter( retval ) );
		_queueData.clear();

		return retval;
	}

	// ------------------------------------------------------------------------------------------------
	template< typename UnaryPredicate >
	std::vector<T> dequeueAllIf( UnaryPredicate pred )
	{
		std::unique_lock lock( _queueLock );

		// Partition: non-matching items first, matching items last
		auto it = std::partition( _queueData.begin(), _queueData.end(),
								  [&pred] ( const T& item ) { return !pred( item ); } );


		// Move matching items to result
		std::vector<T> retval;
		retval.reserve( std::distance( it, _queueData.end() ) );
		std::move( it, _queueData.end(), std::back_inserter( retval ) );

		// Erase matching items from queue
		_queueData.erase( it, _queueData.end() );

		return retval;
	}

	// ------------------------------------------------------------------------------------------------
	template< typename BinaryPredicate >
	void sort( BinaryPredicate pred )
	{
		std::unique_lock lock( _queueLock );
		std::stable_sort( _queueData.begin(), _queueData.end(), pred );
	}

	// ------------------------------------------------------------------------------------------------
	void clear()
	{
		std::unique_lock lock( _queueLock );
		_queueData.clear();
	}

	// ------------------------------------------------------------------------------------------------
	void shutdown()
	{
		{
			std::lock_guard lock( _queueLock );
			_shutdown = true;
		}
		_condition.notify_all();
	}

	// ------------------------------------------------------------------------------------------------
	bool isEmpty() const
	{
		std::lock_guard lock( _queueLock );
		return _queueData.empty();
	}

	// ------------------------------------------------------------------------------------------------
	size_t  size()    const
	{
		std::lock_guard lock( _queueLock );
		return _queueData.size();
	}

private:
	std::deque<T>           _queueData;
	mutable std::mutex      _queueLock;
	std::condition_variable _condition;
};


}
#endif // !CORE_SAFEQUEUE_H
