// LRUCache.h
//
#ifndef	COMMON_LRUCACHE_H
#define CORE_LRUCACHE_H

#include <map>
#include <list>
#include <vector>

template <typename K,typename T>
class LRUCache
{
public:
	typedef typename std::map<K,T>::iterator iterator;

public:
	LRUCache( int capacity = 8 ) : _capacity(capacity) {}
	~LRUCache() {}

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
			_map[ k ] = v;
		}
		else
			use( k );
		
		if( _map.size() > _capacity )
		{
			// get the less recent used key
			// adjust size by removing it from the entries list
			K lru = _entries.back();
			_entries.pop_back();
			
			// remove from the map
			_map.erase(lru);
		}
	}

	// --------------------------------------------------------------------------------------------
	void use(const K &key)
	{
		_entries.remove(key);
			
		// re-insert at the beginning
		_entries.push_front(key);	
	}
	
	// --------------------------------------------------------------------------------------------
	iterator find(const K &key)  { return _map.find(key); }
	iterator begin()             { return _map.begin();   }
	iterator end()               { return _map.end();     }

	// --------------------------------------------------------------------------------------------
	T remove(const K &key)
	{
		T ret = 0;
		if( find(key) )
		{
			ret = _map[key];
			
			_map.erase(key);
			_entries.remove(key);
		}

		return ret;
	}

	// --------------------------------------------------------------------------------------------
	// returns a copy of current entries
	std::vector<T> entries()
	{
		std::vector<T> v;
		for(auto it = _map.begin();
			it != _map.end();
			++it)
			v.push_back(it->second);

		return v;
	}

	// --------------------------------------------------------------------------------------------
	// returns a copy of current key set
	std::vector<K> keys()
	{
		std::vector<K> v;
		for(auto it = _map.begin();
			it != _map.end();
			++it)
			v.push_back(it->first);

		return v;
	}

	// --------------------------------------------------------------------------------------------
	bool contains( const K &k ) const
	{
		return _map.find(k) != _map.end();
	}

	// @todo
	// lruEntry() 
	// mruEntry()

	// --------------------------------------------------------------------------------------------
	T* entry( const K &k )
	{
		auto it = _map.find(k);

		if( it == _map.end() )
			return 0;

		return &it->second;
	}


	// --------------------------------------------------------------------------------------------
	void invalidate()
	{
		_entries.clear();
		_map.clear();
	}

private:
	int             _capacity;
	std::list<K>	_entries;
	std::map<K, T>	_map;
};


#endif
