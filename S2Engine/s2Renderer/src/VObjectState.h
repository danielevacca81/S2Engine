// VObjectState.h
//
#ifndef VOBJECTSTATE_ONCE
#define VOBJECTSTATE_ONCE

#include "s2Scene_API.h"

#include <bitset>

namespace s2 {
namespace Scene {

class S2SCENE_API VObjectState
{
public:
	enum StateChange { Selected=0, Hilighted, Visible };

public:
	// ----------------------------------------------------------------------------------------
	VObjectState() { clear(); }

	// ----------------------------------------------------------------------------------------
	void clear()
	{
		_state.reset();
		_state.set( Visible );
		_changes.reset();
	}

	// ----------------------------------------------------------------------------------------
	void set( const StateChange &s, bool value = true )
	{
		_changes.reset();
		if( _state.test( s ) != value )
		{
			_changes.set( s );
			_state.set( s, value );
		}
	}

	// ----------------------------------------------------------------------------------------
	VObjectState operator+=( const StateChange &s ) { _changes.reset(); if( !_state.test( s ) ) _changes.set( s ); _state.set( s, true );  return *this; }
	VObjectState operator-=( const StateChange &s ) { _changes.reset(); if( _state.test( s ) ) _changes.set( s ); _state.set( s, false ); return *this; }
	VObjectState operator^=( const StateChange &s ) { _changes.reset(); _state.flip( s ); _changes.set( s );                              return *this; }

	// ----------------------------------------------------------------------------------------
	bool operator[]( const StateChange &s )  const { return _state[s]; }
	bool operator==( const VObjectState &s ) const { return _state == s._state; }
	bool operator!=( const VObjectState &s ) const { return _state != s._state; }
	VObjectState operator=( const VObjectState &rhs )
	{
		for( size_t i = 0; i < _state.size(); ++i )
		{
			_changes[i] = rhs._changes[i];
			_state[i] = rhs._state[i];
		}

		return *this;
	}

	// ----------------------------------------------------------------------------------------
	bool  changes( StateChange sc ) const { return _changes[sc]; }
	bool  isEmpty()                 const { return _state.none(); }

private:
	std::bitset<3>         _state;
	mutable std::bitset<3> _changes;
};

}
}


#endif // !VOBJECTSTATE_ONCE
