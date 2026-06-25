// Math.h
//
#ifndef MATH_MATH_H
#define MATH_MATH_H

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include "glm/glm.hpp"

#include "glm/gtc/quaternion.hpp"
#include "glm/gtc/random.hpp"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_access.hpp"
#include "glm/gtc/matrix_inverse.hpp"

#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/type_precision.hpp"
#include "glm/gtc/epsilon.hpp"


#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#endif

#include "glm/gtx/projection.hpp"
#include "glm/gtx/spline.hpp"
#include "glm/gtx/matrix_decompose.hpp"
#include "glm/gtx/easing.hpp"


namespace Math = glm;

namespace glm {
	
	// -----------------------------------------------------------------------------------------------
	template < typename genType >
	GLM_FUNC_QUALIFIER genType
	normalizeAngle_rad( const genType& rad )
	{
		auto angle = rad;
		while( angle >   pi<genType>() ) angle -= two_pi<genType>();
		while( angle <= -pi<genType>() ) angle += two_pi<genType>();
		return angle;
	}

	// -----------------------------------------------------------------------------------------------
	template < typename genType >
	GLM_FUNC_QUALIFIER genType
	normalizeAngle_deg( const genType& deg )
	{
		return degrees( normalizeAngleRad( radians( deg ) ) );
	}

	// -----------------------------------------------------------------------------------------------
	template < typename T, precision P >
	GLM_FUNC_QUALIFIER tvec2<T, P>::value_type
	angleBetween_rad( const tvec2<T, P>& d, const tvec2<T, P>& u )
	{
		typename tvec2<T, P>::value_type angle = atan2( u.y, u.x ) - atan2( d.y, d.x );
		return normalizeAngle_rad( angle );
	}

	// -----------------------------------------------------------------------------------------------
	template < typename T, precision P >
	GLM_FUNC_QUALIFIER tvec3<T, P>::value_type
	angleBetween_rad( const tvec3<T, P>& d, const tvec3<T, P>& u )
	{
		typename tvec3<T, P>::value_type angle = atan2( u.y, u.x ) - atan2( d.y, d.x );
		return normalizeAngle_rad( angle );
	}

	// -----------------------------------------------------------------------------------------------
	template <typename genType>
	GLM_FUNC_DECL genType
	round( genType const& value, int precision )
	{
		genType g = (genType) 1;
		if( precision > 0 )
			g = pow<genType>( (genType) 10, precision );

		return round( value * g ) / g;
	}

	// -----------------------------------------------------------------------------------------------
	inline int log2( unsigned int x )
	{
		int y = 0;
		while( x >>= 1 ) ++y;
		return y;
	}

	// ------------------------------------------------------------------------------------------------
	inline bool isPowerOfTwo( uint32_t x )
	{
		return x && ( x & ( x - 1 ) ) == 0;
	}

	// ------------------------------------------------------------------------------------------------
	inline uint32_t nextPowOfTwo( uint32_t x )
	{
		x--;
		x |= x >> 1;  // handle  2 bit numbers
		x |= x >> 2;  // handle  4 bit numbers
		x |= x >> 4;  // handle  8 bit numbers
		x |= x >> 8;  // handle 16 bit numbers
		x |= x >> 16; // handle 32 bit numbers
	#if 0	
		// @TODO: Enable for 64bit compilers
		x |= x >> 32; // handle 64 bit numbers
	#endif

		x++;

		return x;
	}

	// ------------------------------------------------------------------------------------------------
	/// Returns the component-wise comparison of |x - y| / min(|x|,|y|) <= epsilon.
	/// True if this expression is satisfied.
	template<typename genType>
	GLM_FUNC_DECL bool epsilonEqualRelative( genType const& x, genType const& y, genType const& epsilon );

	template<>
	GLM_FUNC_QUALIFIER bool epsilonEqualRelative
	(
		float const& x,
		float const& y,
		float const& epsilon
	)
	{
		return	( x == y ) ||
			abs( x - y ) / min( abs( x ), abs( y ) ) <= epsilon;
	}

	template<>
	GLM_FUNC_QUALIFIER bool epsilonEqualRelative
	(
		double const& y,
		double const& x,
		double const& epsilon
	)
	{
		return	( x == y ) ||
			abs( x - y ) / min( abs( x ), abs( y ) ) <= epsilon;
	}

	// ------------------------------------------------------------------------------------------------
	/// Returns the component-wise comparison of |x - y| / min(|x|,|y|) <= epsilon.
	/// True if this expression is satisfied.
	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_DECL vec<L, bool, Q> epsilonEqualRelative( vec<L, T, Q> const& x, vec<L, T, Q> const& y, T const& epsilon );

	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_QUALIFIER vec<L, bool, Q> epsilonEqualRelative( vec<L, T, Q> const& x, vec<L, T, Q> const& y, T const& epsilon )
	{
		return	equal( x, y ) ||
			lessThanEqual( abs( x - y ) / min( abs( x ), abs( y ) ), vec<L, T, Q>( epsilon ) );
	}

	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_QUALIFIER vec<L, bool, Q> epsilonEqualRelative( vec<L, T, Q> const& x, vec<L, T, Q> const& y, vec<L, T, Q> const& epsilon )
	{
		return	equal( x, y ) ||
			lessThanEqual( abs( x - y ) / min( abs( x ), abs( y ) ), epsilon );
	}

	// ------------------------------------------------------------------------------------------------
	template<typename genType>
	bool equals( genType const& x, genType const& y )
	{
		return epsilonEqualRelative( x, y, Math::epsilon<genType>() );
	}

}

#endif // !MATH_MATH_H