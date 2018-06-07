// Math.h
//
#ifndef CORE_MATH_H
#define CORE_MATH_H

#include "glm.hpp"

#include "gtc/quaternion.hpp"
#include "gtc/random.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/matrix_access.hpp"
#include "gtc/matrix_inverse.hpp"
#include "gtc/type_ptr.hpp"
#include "gtc/type_precision.hpp"

#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#endif // GLM_ENABLE_EXPERIMENTAL

#include "gtx/color_space.hpp"
#include "gtx/closest_point.hpp"
#include "gtx/gradient_paint.hpp"
#include "gtx/io.hpp"
#include "gtx/polar_coordinates.hpp"
#include "gtx/spline.hpp"

namespace Math = glm;

namespace glm {
	//// ------------------------------------------------------------------------------------------------
	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_QUALIFIER typename vec<L, T, Q>::value_type
	angleBetween( const vec<L,T,Q> &d,
				  const vec<L,T,Q> &u  )
	{
		typename vec<L,T,Q>::value_type angle =  atan2( u.y,u.x ) - atan2( d.y,d.x );
		while(angle >   pi<double>() ) angle -= pi<double>()*2.0;
		while(angle <= -pi<double>() ) angle += pi<double>()*2.0;
		return angle;
	}

	// -----------------------------------------------------------------------------------------------
	template <typename genType> 
	GLM_FUNC_DECL genType 
		round( genType const & value, int precision )
	{		
		genType g = (genType)1;
		if( precision > 0 ) 
			g = glm::pow<genType>( (genType)10,precision );

		return round( value*g ) / g;
	}

	// ------------------------------------------------------------------------------------------------
	inline int log2(unsigned int x)
	{
		int y = 0;
		while (x >>= 1) ++y;
		return y;
	}

	// ------------------------------------------------------------------------------------------------
	inline bool isPowerOfTwo(unsigned int x)
	{
		return ((x & (x - 1)) == 0);
	}

	// ------------------------------------------------------------------------------------------------
	inline unsigned int nextPowOfTwo(unsigned int x)
	{
		x--;
		x |= x >> 1;  // handle  2 bit numbers
		x |= x >> 2;  // handle  4 bit numbers
		x |= x >> 4;  // handle  8 bit numbers
		x |= x >> 8;  // handle 16 bit numbers
		x |= x >> 16; // handle 32 bit numbers
#if 0	
		// @TODO: Enable only for 64bit compilers
		x |= x >> 32; // handle 64 bit numbers
#endif

		x++;

		return x;
	}
}

#endif