// Observer.h
// 
#ifndef OBSERVER_ONCE
#define OBSERVER_ONCE

#include "S2CommonAPI.h"

#include <vector>

namespace s2 {

class IObserver;
class IObservable;

// ------------------------------------------------------------------------------------------------
class S2COMMON_API IObserver
{
public:
	IObserver()
	: _enabled(true)
	{}

	~IObserver()
	{}

	void    enableNotify( bool enable ) { _enabled = enable; }
	bool    isEnabled() const           { return _enabled;}

	virtual void notify( IObservable *o ) = 0;

protected:
	bool _enabled;

};

// ------------------------------------------------------------------------------------------------
class S2COMMON_API IObservable
{
public:
	IObservable()
	{}

	~IObservable()
	{}

	// ------------------------------------------------------------------------------------------------
	void registerObserver  ( IObserver *o )
	{
		unregisterObserver(o);
		_observerCollection.push_back( o );
	}

	// ------------------------------------------------------------------------------------------------
	bool unregisterObserver( IObserver *o )
	{
		bool success = false;
		for( auto it = _observerCollection.begin();
			it     != _observerCollection.end();
			++it )
		{
			if( *it == o )
			{
				_observerCollection.erase( it );
				success = true;
				break;
			}
		}

		return success;
	}

	// ------------------------------------------------------------------------------------------------
	void notifyObservers()
	{
		for( auto it = _observerCollection.begin();
			it     != _observerCollection.end();
			++it )
			if( (*it)->isEnabled() )
				(*it)->notify(this);
	}

private:
	std::vector<IObserver*> _observerCollection;
};


}


#endif