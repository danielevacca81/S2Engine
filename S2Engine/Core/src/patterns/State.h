// State.h
// 
#ifndef STATE_ONCE
#define STATE_ONCE

#include "Core_API.h"

#include <bitset>

namespace s2 {

class CORE_API StateChange
{

};

template< int N >
class CORE_API State
{
private:
	std::bitset<N>         _state;
	mutable std::bitset<N> _changes;

public:
	// ----------------------------------------------------------------------------------------
	State()  { clear(); }

	// ----------------------------------------------------------------------------------------
	void clear()
	{
		_state.reset();
		//_state.set(Visible);
		_changes.reset();
	}

	// ----------------------------------------------------------------------------------------
	void set( const StateChange &s, bool value = true )
	{ 
		_changes.reset();
		if( _state.test(s) != value )
		{ 
			_changes.set(s); 
			_state.set(s,value); 
		}
	}

	// ----------------------------------------------------------------------------------------
	State operator+=( const StateChange &s )       { _changes.reset(); if( !_state.test(s) ) _changes.set(s); _state.set( s,true );  return *this; }
	State operator-=( const StateChange &s )       { _changes.reset(); if(  _state.test(s) ) _changes.set(s); _state.set( s,false ); return *this; }
	State operator^=( const StateChange &s )       { _changes.reset(); _state.flip(s); _changes.set(s);                              return *this; }
		
	// ----------------------------------------------------------------------------------------
	bool operator[]( const StateChange &s ) const { return _state[s];          }
	bool operator==( const State &s       ) const { return _state == s._state; }
	bool operator!=( const State &s       ) const { return _state != s._state; }
	State operator=( const State &rhs     )
	{ 
		_changes.reset();
		for( size_t i=0; i<_state.size(); ++i ) 
		{
			_changes.set(i, _state[i]!=rhs._state[i]);
			_state[i] = rhs._state[i];
		}
		return *this;
	}

	// ----------------------------------------------------------------------------------------
	bool  changes( const StateChange &sc ) const    { return _changes.at(sc); }
	bool  isEmpty()                        const    { return _state.none();   }
};

}

#endif