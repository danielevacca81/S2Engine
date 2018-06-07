// Observer.h
// 
#ifndef OBSERVER_ONCE
#define OBSERVER_ONCE

#include "s2Core_API.h"

#include <vector>

namespace s2 {

class Observer;
class Observable;

// ------------------------------------------------------------------------------------------------
class S2CORE_API Observer
{
public:
	Observer()
	: _enabled(true)
	{}

	~Observer()
	{}

	void    enableNotify( bool enable ) { _enabled = enable; }
	bool    isEnabled() const           { return _enabled;}

	virtual void notify( Observable *o ) = 0;

protected:
	bool _enabled;
};

// ------------------------------------------------------------------------------------------------
class S2CORE_API Observable
{
public:
	Observable()
	{}

	~Observable()
	{}

	// ------------------------------------------------------------------------------------------------
	void registerObserver  ( Observer *o )
	{
		unregisterObserver(o);
		_observerCollection.push_back( o );
	}

	// ------------------------------------------------------------------------------------------------
	bool unregisterObserver( Observer *o )
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
	std::vector<Observer*> _observerCollection;
};


}


#endif