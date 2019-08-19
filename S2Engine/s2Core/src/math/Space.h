// Space.h
//
#ifndef SPACE_ONCE
#define SPACE_ONCE

#include "Math.h"

#include "Plane.h"

namespace glm 
{
	template<typename T, precision P>
	GLM_FUNC_QUALIFIER typename tmat3x3<T, P>
	localFrame( const tvec3<T,P> &dir  )
	{
		// compute best candidate direction orthogonal to dir
		const auto candidate = [] ( const tvec3<T,P> &d )
		{
			const detail::tplane<T,P> p( d, static_cast<T>( 0.0 ) );
		
			const auto pX = p.projectPoint( { 1.0,0.0,0.0 } );
			const auto pY = p.projectPoint( { 0.0,1.0,0.0 } );
			const auto pZ = p.projectPoint( { 0.0,0.0,1.0 } );

			const auto lX = length( pX );
			const auto lY = length( pY );
			const auto lZ = length( pZ );
		
			if( lX > lY )
				return ( lX > lZ ) ? pX : pZ;

			return ( lY > lZ ) ? pY : pZ;
		}( dir );
	
		const auto ortho = cross( dir, candidate );
		
		// gram-schmidt orthogonalization
		const auto u0 = dir;
		const auto u1 = normalize( candidate - proj( candidate, u0 ) );
		const auto u2 = normalize( ortho     - proj( ortho, u0 )     - proj( ortho, u1 ) );

		tmat3x3<T, P> frame;
		frame[0] = u0;
		frame[1] = u1;
		frame[2] = u2;

		return frame;
	}

}

#endif // !SPACE_ONCE
