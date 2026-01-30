// LRUCache.h
//
#ifndef	CORE_LRUCACHE_H
#define CORE_LRUCACHE_H

#include <unordered_map>
#include <list>
#include <vector>

namespace s2 {


template <typename K, typename T>
class LRUCache
{
private:
	using ListIterator = typename std::list<K>::iterator;
	using MapType = std::unordered_map<K, std::pair<T, ListIterator>>;

public:
	using iterator = typename MapType::iterator;
	using const_iterator = typename MapType::const_iterator;

public:
	LRUCache( uint32_t capacity = 8 )
		: _capacity( capacity )
	{}

	// --------------------------------------------------------------------------------------------
	void store( const  K& k, const T& v )
	{
		// find the key
		auto it = _map.find( k );
		if( it == _map.end() )
		{
			// insert a new entry
			// push front the key of the most recently used entry
			_entries.push_front( k );
			_map[k] = { v, _entries.begin() };
		}
		else
		{
			// update existing value and mark as recently used
			it->second.first = v;
			_entries.erase( it->second.second );
			_entries.push_front( k );
			it->second.second = _entries.begin();
		}
		
		// evict least recently used if over capacity
		if( _map.size() > _capacity )
		{
			// get the less recent used key
			// adjust size by removing it from the entries list
			K lru = _entries.back();
			_entries.pop_back();

			// remove from the map
			_map.erase( lru );
		}
	}

	// --------------------------------------------------------------------------------------------
	void use( const K& key )
	{
		auto it = _map.find( key );
		if( it == _map.end() )
			return;

		// move to front in O(1)
		_entries.erase( it->second.second );
		_entries.push_front( key );
		it->second.second = _entries.begin();
	}

	// --------------------------------------------------------------------------------------------
	iterator find( const K& key ) { return _map.find( key ); }
	iterator begin()              { return _map.begin(); }
	iterator end()                { return _map.end(); }

	const_iterator find( const K& key ) const { return _map.find( key ); }
	const_iterator begin()              const { return _map.begin(); }
	const_iterator end()                const { return _map.end(); }


	// --------------------------------------------------------------------------------------------
	T remove( const K& key )
	{
		auto it = _map.find( key );
		if( it == _map.end() )
			return {};

		auto ret = it->second.first;

		_entries.erase( it->second.second );
		_map.erase( it );

		return ret;
	}

	// --------------------------------------------------------------------------------------------
	// returns a copy of current entries
	std::vector<T> entries() const
	{
		std::vector<T> v;
		v.reserve( _map.size() );
		for( const auto& [key, value] : _map )
			v.push_back( value.first );

		return v;
	}

	// --------------------------------------------------------------------------------------------
	// returns a copy of current key set
	std::vector<K> keys() const
	{
		std::vector<K> v;
		v.reserve( _map.size() );
		for( const auto& [key, _] : _map )
			v.push_back( key );

		return v;
	}

	// --------------------------------------------------------------------------------------------
	bool contains( const K& k ) const
	{
		return _map.find( k ) != _map.end();
	}

	// @todo
	// lruEntry() 
	// mruEntry()

	// --------------------------------------------------------------------------------------------
	T* entry( const K& k )
	{
		auto it = _map.find( k );
		if( it == _map.end() )
			return nullptr;

		// store returning value before marking as recently used
		// ( it could be invalidated in use() )
		auto ret = &it->second.first;

		// mark as recently used
		use( k );

		return ret;
	}

	// --------------------------------------------------------------------------------------------
	void invalidate()
	{
		_entries.clear();
		_map.clear();
	}

	// --------------------------------------------------------------------------------------------
	size_t size()     const { return _map.size(); }
	size_t capacity() const { return _capacity; }

private:
	uint32_t     _capacity = 8;
	std::list<K> _entries;
	MapType      _map;
};

}

#endif