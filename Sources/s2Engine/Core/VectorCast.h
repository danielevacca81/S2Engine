// VectorCast.h
// 
// Vector casting utility functions
#ifndef CORE_VECTORCAST_H
#define CORE_VECTORCAST_H

#include <vector>
#include <concepts>

namespace s2 {

// ------------------------------------------------------------------------------------------------
template<class T, class U> requires std::is_convertible_v<T, U>
static inline std::vector<U> vector_cast( const std::vector<T>& v )
{
	std::vector<U> result;
	result.reserve( v.size() );
	for( const auto& e : v )
		result.push_back( static_cast<U>( e ) );
	return result;
}

} // namespace s2
#endif // !CORE_VECTORCAST_H