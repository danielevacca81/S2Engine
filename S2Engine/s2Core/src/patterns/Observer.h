// Observer.h
// 
#ifndef OBSERVER_ONCE
#define OBSERVER_ONCE

#include "s2Core_API.h"

#include <vector>
#include <algorithm>
#include <any>

namespace s2 {

class Observer;
class Observable;

// ------------------------------------------------------------------------------------------------
class S2CORE_API Observer
{
public:
	Observer() = default;
	virtual ~Observer() = default;

	void    enableNotifcations( bool enable ) { _enabled = enable; }
	bool    isNotificationEnabled() const { return _enabled; }

	virtual void notify( Observable *o, const std::any &message ) = 0;

protected:
	bool _enabled = true;
};



// ------------------------------------------------------------------------------------------------
class S2CORE_API Observable
{
public:
	// ------------------------------------------------------------------------------------------------
	void registerObserver( Observer *o )
	{
		if( !o ) return; // assert?

		unregisterObserver( o );
		_observerCollection.push_back( o );
	}

	// ------------------------------------------------------------------------------------------------
	void unregisterObserver( Observer *o )
	{
		if( !o ) return; // assert?

		// sort
		std::sort( _observerCollection.begin(), _observerCollection.end() );
		
		// remove consecutive duplicates
		auto last = std::unique( _observerCollection.begin(), _observerCollection.end() );
		
		// remove trailing empty elements
		_observerCollection.erase( last, _observerCollection.end() );

		//_observerCollection.erase(
		//	std::remove_if(
		//		_observerCollection.begin(),
		//		_observerCollection.end(),
		//		[o] ( Observer *curr ) 
		//		{
		//			return o == curr;
		//		} )
		//);
	}

	// ------------------------------------------------------------------------------------------------
	void notifyObservers( const std::any &message )
	{
		for( auto &it : _observerCollection )
		{
			if( it->isNotificationEnabled() )
				it->notify( this, message );
		}
	}

private:
	std::vector<Observer*> _observerCollection;
};


}


#endif