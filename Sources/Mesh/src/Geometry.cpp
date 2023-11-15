// Geometry.cpp
// 
#include "Geometry.h"

#include "Space.h"

#include <gtx/projection.hpp>

#include <iostream>

namespace s2 {

// ------------------------------------------------------------------------------------------------
std::vector< Math::dvec2 > circle( const Math::dvec2 &center, double radius, int LOD /* = 32 */ )
{
	std::vector< Math::dvec2 > circlePoints;

	const double step = Math::two_pi<double>() / LOD;

	circlePoints.push_back( Math::dvec2( center ) ); // center as first point

	for( int i=0; i <= LOD; ++i )
	{
		const double angle  = i * step;
		const Math::dvec2 p =  Math::dvec2( Math::cos( angle ), Math::sin( angle ) )*radius;

		circlePoints.push_back( center + p );
	}

	return std::move( circlePoints );
}

// ------------------------------------------------------------------------------------------------
Math::Mesh torus( double innerRadius, double outerRadius, int numc, int numt )
{
	const double twopi = Math::two_pi<double>();

	std::vector<Math::dvec3> points;
	std::vector<unsigned int> indices;
	std::vector<Math::vec3> normals;

	// numc = numero di circonferenze (sectors)
	for( int i = 0; i < numc; ++i )
	{
		// numt = punti per circonferenza
		for( int j = 0; j < numt; ++j )
		{
			const double t = twopi * i / (double) numc;
			const double p = twopi * j / (double) numt;

			const double x = ( innerRadius + outerRadius * Math::cos( p ) ) * Math::cos( t );
			const double y = ( innerRadius + outerRadius * Math::cos( p ) ) * Math::sin( t );
			const double z =  outerRadius * Math::sin( p );

			points.push_back( Math::dvec3( x, y, z ) );

			const Math::dvec3 T( -Math::sin( t ), Math::cos( t ), 0 );
			const Math::dvec3 B( Math::cos( t )*( -Math::sin( p ) ),
								 Math::sin( t )*( -Math::sin( p ) ),
								 Math::cos( p ) );

			normals.push_back( Math::cross( T, B ) );
			//normals.push_back( Math::normalize( Math::dvec3( x, y, z ) ) );

			// first_triangle
			indices.push_back( ( i * numt ) + j );
			indices.push_back( ( ( ( i + 1 ) % numc ) * numt ) + j );
			indices.push_back( ( ( ( i + 1 ) % numc ) * numt ) + ( j + 1 ) % numt );

			// second triangle
			indices.push_back( ( i * numt ) + j );
			indices.push_back( ( ( ( i + 1 ) % numc ) * numt ) + ( j + 1 ) % numt );
			indices.push_back( ( i * numt ) + ( j + 1 ) % numt );
		}
	}

	Math::Mesh m( points, normals, indices );
	return m;
}

// ------------------------------------------------------------------------------------------------
Math::Mesh cylinder( const Math::dvec3 &startPoint, const Math::dvec3 &endPoint, double radius, bool capStart, bool capEnd, int slices )
{
	if( slices < 4 )
		return {};

	const double len      = Math::length( endPoint - startPoint );
	const Math::dvec3 dir = Math::normalize( endPoint - startPoint );
	const int loopIndex   = slices * 2;  // two rings (start/end)

	const std::vector<Math::dvec3> circle = []( int slices )
	{
		const double twopi = Math::two_pi<double>();

		std::vector<Math::dvec3> values;
		for( int s=0; s < slices; ++s )
		{
			const double theta = twopi * ( s / (double) slices );
			const double ct = Math::cos( theta );
			const double st = Math::sin( theta );

			// build a circle on the plane YZ
			values.emplace_back( Math::dvec3( 0.0, ct, st ) );
		}
		return values;	
	}( slices );

	Math::dmat4 basis = Math::localFrame( dir );
	basis[3] = {0.0, 0.0, 0.0, 1.0};

	const Math::dmat4 rotateAroundStartPoint = basis;

	std::vector<Math::dvec3>  vertices;
	std::vector<unsigned int> indices;
	std::vector<Math::vec3>   normals;

	int idx = 0;
	for( auto &s : circle )
	{
		// two points per iteration, one around at startPoint one around endPoint
		const Math::dvec3 pStart = rotateAroundStartPoint * Math::dvec4( s*radius,1.0 );
		const Math::dvec3 pEnd   = pStart + dir * len;

		vertices.emplace_back( startPoint+pEnd );
		vertices.emplace_back( startPoint+pStart );
		normals.emplace_back( Math::dmat3( basis ) * s );
		normals.emplace_back( Math::dmat3( basis ) * s );
		
		// six indices per slices, idx+=2
		// 0 , 1 , 3
		// 0 , 3 , 2		
		indices.emplace_back( idx+0 );
		indices.emplace_back( idx+1 );
		indices.emplace_back( (idx+3) % loopIndex);
		
		indices.emplace_back( idx+0 );
		indices.emplace_back( (idx+3) % loopIndex );
		indices.emplace_back( (idx+2) % loopIndex );

		idx+=2;	
	}

	// add cap circle at start
	if( capStart )
	{
		const int first = (int) vertices.size();
		int k = 0;
		for( auto &s : circle )
		{
			const Math::dvec3 pStart = rotateAroundStartPoint * Math::dvec4( s*radius,1.0 );

			vertices.emplace_back( startPoint + pStart );
			normals.emplace_back( -basis[0] );

			if( k >= 2 )
			{
				indices.emplace_back( first + 0   );
				indices.emplace_back( first + k-1 );
				indices.emplace_back( first + k   );
			}
			
			++k;
		}
	}

	// add cap circle at end
	if( capEnd )
	{
		const int first = (int) vertices.size();
		int k = 0;
		for( auto &s : circle )
		{
			const Math::dvec3 pEnd = Math::dvec3( rotateAroundStartPoint * Math::dvec4( s*radius, 1.0 ) ) + dir * len;
			
			vertices.emplace_back( startPoint + pEnd );
			normals.emplace_back( basis[0] );

			if( k >= 2 )
			{
				indices.emplace_back( first + 0   );
				indices.emplace_back( first + k-1 );
				indices.emplace_back( first + k   );
			}
			
			++k;
		}
	}

	return Math::Mesh ( vertices, normals,indices );
}

// ------------------------------------------------------------------------------------------------
Math::Mesh S2CORE_API sphere( const Math::dvec3 &center, double radius, int slices )
{
	if( slices < 4 )
		return {};

	const int    rings = slices;
	const double twopi = Math::two_pi<double>();
	
	std::vector<Math::dvec3>  vertices;
	std::vector<Math::vec3>   normals;

	const double dTheta = twopi / double( slices );
    const double dPhi   = Math::pi<double>() / double( rings );
    const double du     = 1.0 /   double( slices );
    const double dv     = 1.0 /   double( rings );

	for( int r = 0; r < rings+1; ++r )
	{
        const double phi    = Math::half_pi<double>() - double( r ) * dPhi;
        const double cosPhi = Math::cos( phi );
        const double sinPhi = Math::sin( phi );

		for( int s = 0; s < slices+1; ++s )
		{
            const double theta    = double( s ) * dTheta;
            const double cosTheta = Math::cos( theta );
            const double sinTheta = Math::sin( theta );

			const Math::dvec3 p = 
			{
				cosTheta * cosPhi,
				sinPhi,
				sinTheta * cosPhi
			};

			vertices.emplace_back( center +  p * radius );
			normals.emplace_back( p );
		}
	}
	
	std::vector<unsigned int> indices;
	{
		int faces = ( slices * 2 ) * ( rings - 2 ); // two tris per slice, for all middle rings
		faces += 2 * slices; // tri per slice for both top and bottom

		const int indexCount = faces * 3;

		indices.resize( indexCount );
		unsigned int *indexPtr = &indices[0];

		// top cap
		{
			const int nextRingStartIndex = slices + 1;
			for( int j = 0; j < slices; ++j )
			{
				*indexPtr++ = nextRingStartIndex + j;
				*indexPtr++ = 0;
				*indexPtr++ = nextRingStartIndex + j + 1;
			}
		}

		for( int i = 1; i < ( rings - 1 ); ++i )
		{
			const int ringStartIndex = i * ( slices + 1 );
			const int nextRingStartIndex = ( i + 1 ) * ( slices + 1 );

			for( int j = 0; j < slices; ++j )
			{
				// Split the quad into two triangles
				*indexPtr++ = ringStartIndex + j;
				*indexPtr++ = ringStartIndex + j + 1;
				*indexPtr++ = nextRingStartIndex + j;
				*indexPtr++ = nextRingStartIndex + j;
				*indexPtr++ = ringStartIndex + j + 1;
				*indexPtr++ = nextRingStartIndex + j + 1;
			}
		}

		// bottom cap
		{
			const int ringStartIndex = ( rings - 1 ) * ( slices + 1 );
			const int nextRingStartIndex = ( rings ) * ( slices + 1 );
			for( int j = 0; j < slices; ++j )
			{
				*indexPtr++ = ringStartIndex + j + 1;
				*indexPtr++ = nextRingStartIndex;
				*indexPtr++ = ringStartIndex + j;
			}
		}
	}

	return Math::Mesh ( vertices, normals,indices );
}




// ------------------------------------------------------------------------------------------------
#pragma region TEAPOT
struct vertex { double x, y, z; };
struct vertex teapot_cp_vertices[] = {
	// 1
	{  1.4   ,   0.0   ,  2.4     },
	{  1.4   ,  -0.784 ,  2.4     },
	{  0.784 ,  -1.4   ,  2.4     },
	{  0.0   ,  -1.4   ,  2.4     },
	{  1.3375,   0.0   ,  2.53125 },
	{  1.3375,  -0.749 ,  2.53125 },
	{  0.749 ,  -1.3375,  2.53125 },
	{  0.0   ,  -1.3375,  2.53125 },
	{  1.4375,    0.0  ,  2.53125 },
	{  1.4375,  -0.805 ,  2.53125 },
	// 11
	{  0.805 ,  -1.4375,  2.53125 },
	{  0.0   ,  -1.4375,  2.53125 },
	{  1.5   ,   0.0   ,  2.4     },
	{  1.5   ,  -0.84  ,  2.4     },
	{  0.84  ,  -1.5   ,  2.4     },
	{  0.0   ,  -1.5   ,  2.4     },
	{ -0.784 ,  -1.4   ,  2.4     },
	{ -1.4   ,  -0.784 ,  2.4     },
	{ -1.4   ,   0.0   ,  2.4     },
	{ -0.749 ,  -1.3375,  2.53125 },
	// 21
	{ -1.3375,  -0.749 ,  2.53125 },
	{ -1.3375,   0.0   ,  2.53125 },
	{ -0.805 ,  -1.4375,  2.53125 },
	{ -1.4375,  -0.805 ,  2.53125 },
	{ -1.4375,   0.0   ,  2.53125 },
	{ -0.84  ,  -1.5   ,  2.4     },
	{ -1.5   ,  -0.84  ,  2.4     },
	{ -1.5   ,   0.0   ,  2.4     },
	{ -1.4   ,   0.784 ,  2.4     },
	{ -0.784 ,   1.4   ,  2.4     },
	// 31
	{  0.0   ,   1.4   ,  2.4     },
	{ -1.3375,   0.749 ,  2.53125 },
	{ -0.749 ,   1.3375,  2.53125 },
	{  0.0   ,   1.3375,  2.53125 },
	{ -1.4375,   0.805 ,  2.53125 },
	{ -0.805 ,   1.4375,  2.53125 },
	{  0.0   ,   1.4375,  2.53125 },
	{ -1.5   ,   0.84  ,  2.4     },
	{ -0.84  ,   1.5   ,  2.4     },
	{  0.0   ,   1.5   ,  2.4     },
	// 41
	{  0.784 ,   1.4   ,  2.4     },
	{  1.4   ,   0.784 ,  2.4     },
	{  0.749 ,   1.3375,  2.53125 },
	{  1.3375,   0.749 ,  2.53125 },
	{  0.805 ,   1.4375,  2.53125 },
	{  1.4375,   0.805 ,  2.53125 },
	{  0.84  ,   1.5   ,  2.4     },
	{  1.5   ,   0.84  ,  2.4     },
	{  1.75  ,   0.0   ,  1.875   },
	{  1.75  ,  -0.98  ,  1.875   },
	// 51
	{  0.98  ,  -1.75  ,  1.875   },
	{  0.0   ,  -1.75  ,  1.875   },
	{  2.0   ,   0.0   ,  1.35    },
	{  2.0   ,  -1.12  ,  1.35    },
	{  1.12  ,  -2.0   ,  1.35    },
	{  0.0   ,  -2.0   ,  1.35    },
	{  2.0   ,   0.0   ,  0.9     },
	{  2.0   ,  -1.12  ,  0.9     },
	{  1.12  ,  -2.0   ,  0.9     },
	{  0.0   ,  -2.0   ,  0.9     },
	// 61
	{ -0.98  ,  -1.75  ,  1.875   },
	{ -1.75  ,  -0.98  ,  1.875   },
	{ -1.75  ,   0.0   ,  1.875   },
	{ -1.12  ,  -2.0   ,  1.35    },
	{ -2.0   ,  -1.12  ,  1.35    },
	{ -2.0   ,   0.0   ,  1.35    },
	{ -1.12  ,  -2.0   ,  0.9     },
	{ -2.0   ,  -1.12  ,  0.9     },
	{ -2.0   ,   0.0   ,  0.9     },
	{ -1.75  ,   0.98  ,  1.875   },
	// 71
	{ -0.98  ,   1.75  ,  1.875   },
	{  0.0   ,   1.75  ,  1.875   },
	{ -2.0   ,   1.12  ,  1.35    },
	{ -1.12  ,   2.0   ,  1.35    },
	{  0.0   ,   2.0   ,  1.35    },
	{ -2.0   ,   1.12  ,  0.9     },
	{ -1.12  ,   2.0   ,  0.9     },
	{  0.0   ,   2.0   ,  0.9     },
	{  0.98  ,   1.75  ,  1.875   },
	{  1.75  ,   0.98  ,  1.875   },
	// 81
	{  1.12  ,   2.0   ,  1.35    },
	{  2.0   ,   1.12  ,  1.35    },
	{  1.12  ,   2.0   ,  0.9     },
	{  2.0   ,   1.12  ,  0.9     },
	{  2.0   ,   0.0   ,  0.45    },
	{  2.0   ,  -1.12  ,  0.45    },
	{  1.12  ,  -2.0   ,  0.45    },
	{  0.0   ,  -2.0   ,  0.45    },
	{  1.5   ,   0.0   ,  0.225   },
	{  1.5   ,  -0.84  ,  0.225   },
	// 91
	{  0.84  ,  -1.5   ,  0.225   },
	{  0.0   ,  -1.5   ,  0.225   },
	{  1.5   ,   0.0   ,  0.15    },
	{  1.5   ,  -0.84  ,  0.15    },
	{  0.84  ,  -1.5   ,  0.15    },
	{  0.0   ,  -1.5   ,  0.15    },
	{ -1.12  ,  -2.0   ,  0.45    },
	{ -2.0   ,  -1.12  ,  0.45    },
	{ -2.0   ,   0.0   ,  0.45    },
	{ -0.84  ,  -1.5   ,  0.225   },
	// 101
	{ -1.5   ,  -0.84  ,  0.225   },
	{ -1.5   ,   0.0   ,  0.225   },
	{ -0.84  ,  -1.5   ,  0.15    },
	{ -1.5   ,  -0.84  ,  0.15    },
	{ -1.5   ,   0.0   ,  0.15    },
	{ -2.0   ,   1.12  ,  0.45    },
	{ -1.12  ,   2.0   ,  0.45    },
	{  0.0   ,   2.0   ,  0.45    },
	{ -1.5   ,   0.84  ,  0.225   },
	{ -0.84  ,   1.5   ,  0.225   },
	// 111
	{  0.0   ,   1.5   ,  0.225   },
	{ -1.5   ,   0.84  ,  0.15    },
	{ -0.84  ,   1.5   ,  0.15    },
	{  0.0   ,   1.5   ,  0.15    },
	{  1.12  ,   2.0   ,  0.15    },
	{  2.0   ,   1.12  ,  0.45    },
	{  0.84  ,   1.5   ,  0.225   },
	{  1.5   ,   0.84  ,  0.225   },
	{  0.84  ,   1.5   ,  0.15    },
	{  1.5   ,   0.84  ,  0.15    },
	// 121
	{ -1.6   ,   0.0   ,  2.025   },
	{ -1.6   ,  -0.3   ,  2.025   },
	{ -1.5   ,  -0.3   ,  2.25    },
	{ -1.5   ,   0.0   ,  2.25    },
	{ -2.3   ,   0.0   ,  2.025   },
	{ -2.3   ,  -0.3   ,  2.025   },
	{ -2.5   ,  -0.3   ,  2.25    },
	{ -2.5   ,   0.0   ,  2.25    },
	{ -2.7   ,   0.0   ,  2.025   },
	{ -2.7   ,  -0.3   ,  2.025   },
	// 131
	{ -3.0   ,  -0.3   ,  2.25    },
	{ -3.0   ,   0.0   ,  2.25    },
	{ -2.7   ,   0.0   ,  1.8     },
	{ -2.7   ,  -0.3   ,  1.8     },
	{ -3.0   ,  -0.3   ,  1.8     },
	{ -3.0   ,   0.0   ,  1.8     },
	{ -1.5   ,   0.3   ,  2.25    },
	{ -1.6   ,   0.3   ,  2.025   },
	{ -2.5   ,   0.3   ,  2.25    },
	{ -2.3   ,   0.3   ,  2.025   },
	// 141
	{ -3.0   ,   0.3   ,  2.25    },
	{ -2.7   ,   0.3   ,  2.025   },
	{ -3.0   ,   0.3   ,  1.8     },
	{ -2.7   ,   0.3   ,  1.8     },
	{ -2.7   ,   0.0   ,  1.575   },
	{ -2.7   ,  -0.3   ,  1.575   },
	{ -3.0   ,  -0.3   ,  1.35    },
	{ -3.0   ,   0.0   ,  1.35    },
	{ -2.5   ,   0.0   ,  1.125   },
	{ -2.5   ,  -0.3   ,  1.125   },
	// 151
	{ -2.65  ,  -0.3   ,  0.9375  },
	{ -2.65  ,   0.0   ,  0.9375  },
	{ -2.0   ,  -0.3   ,  0.9     },
	{ -1.9   ,  -0.3   ,  0.6     },
	{ -1.9   ,   0.0   ,  0.6     },
	{ -3.0   ,   0.3   ,  1.35    },
	{ -2.7   ,   0.3   ,  1.575   },
	{ -2.65  ,   0.3   ,  0.9375  },
	{ -2.5   ,   0.3   ,  1.1255  },
	{ -1.9   ,   0.3   ,  0.6     },
	// 161
	{ -2.0   ,   0.3   ,  0.9     },
	{  1.7   ,   0.0   ,  1.425   },
	{  1.7   ,  -0.66  ,  1.425   },
	{  1.7   ,  -0.66  ,  0.6     },
	{  1.7   ,   0.0   ,  0.6     },
	{  2.6   ,   0.0   ,  1.425   },
	{  2.6   ,  -0.66  ,  1.425   },
	{  3.1   ,  -0.66  ,  0.825   },
	{  3.1   ,   0.0   ,  0.825   },
	{  2.3   ,   0.0   ,  2.1     },
	// 171
	{  2.3   ,  -0.25  ,  2.1     },
	{  2.4   ,  -0.25  ,  2.025   },
	{  2.4   ,   0.0   ,  2.025   },
	{  2.7   ,   0.0   ,  2.4     },
	{  2.7   ,  -0.25  ,  2.4     },
	{  3.3   ,  -0.25  ,  2.4     },
	{  3.3   ,   0.0   ,  2.4     },
	{  1.7   ,   0.66  ,  0.6     },
	{  1.7   ,   0.66  ,  1.425   },
	{  3.1   ,   0.66  ,  0.825   },
	// 181
	{  2.6   ,   0.66  ,  1.425   },
	{  2.4   ,   0.25  ,  2.025   },
	{  2.3   ,   0.25  ,  2.1     },
	{  3.3   ,   0.25  ,  2.4     },
	{  2.7   ,   0.25  ,  2.4     },
	{  2.8   ,   0.0   ,  2.475   },
	{  2.8   ,  -0.25  ,  2.475   },
	{  3.525 ,  -0.25  ,  2.49375 },
	{  3.525 ,   0.0   ,  2.49375 },
	{  2.9   ,   0.0   ,  2.475   },
	// 191
	{  2.9   ,  -0.15  ,  2.475   },
	{  3.45  ,  -0.15  ,  2.5125  },
	{  3.45  ,   0.0   ,  2.5125  },
	{  2.8   ,   0.0   ,  2.4     },
	{  2.8   ,  -0.15  ,  2.4     },
	{  3.2   ,  -0.15  ,  2.4     },
	{  3.2   ,   0.0   ,  2.4     },
	{  3.525 ,   0.25  ,  2.49375 },
	{  2.8   ,   0.25  ,  2.475   },
	{  3.45  ,   0.15  ,  2.5125  },
	// 201
	{  2.9   ,   0.15  ,  2.475   },
	{  3.2   ,   0.15  ,  2.4     },
	{  2.8   ,   0.15  ,  2.4     },
	{  0.0   ,   0.0   ,  3.15    },
	{  0.0   ,  -0.002 ,  3.15    },
	{  0.002 ,   0.0   ,  3.15    },
	{  0.8   ,   0.0   ,  3.15    },
	{  0.8   ,  -0.45  ,  3.15    },
	{  0.45  ,  -0.8   ,  3.15    },
	{  0.0   ,  -0.8   ,  3.15    },
	// 211
	{  0.0   ,   0.0   ,  2.85    },
	{  0.2   ,   0.0   ,  2.7     },
	{  0.2   ,  -0.112 ,  2.7     },
	{  0.112 ,  -0.2   ,  2.7     },
	{  0.0   ,  -0.2   ,  2.7     },
	{ -0.002 ,   0.0   ,  3.15    },
	{ -0.45  ,  -0.8   ,  3.15    },
	{ -0.8   ,  -0.45  ,  3.15    },
	{ -0.8   ,   0.0   ,  3.15    },
	{ -0.112 ,  -0.2   ,  2.7     },
	// 221
	{ -0.2   ,  -0.112 ,  2.7     },
	{ -0.2   ,   0.0   ,  2.7     },
	{  0.0   ,   0.002 ,  3.15    },
	{ -0.8   ,   0.45  ,  3.15    },
	{ -0.45  ,   0.8   ,  3.15    },
	{  0.0   ,   0.8   ,  3.15    },
	{ -0.2   ,   0.112 ,  2.7     },
	{ -0.112 ,   0.2   ,  2.7     },
	{  0.0   ,   0.2   ,  2.7     },
	{  0.45  ,   0.8   ,  3.15    },
	// 231
	{  0.8   ,   0.45  ,  3.15    },
	{  0.112 ,   0.2   ,  2.7     },
	{  0.2   ,   0.112 ,  2.7     },
	{  0.4   ,   0.0   ,  2.55    },
	{  0.4   ,  -0.224 ,  2.55    },
	{  0.224 ,  -0.4   ,  2.55    },
	{  0.0   ,  -0.4   ,  2.55    },
	{  1.3   ,   0.0   ,  2.55    },
	{  1.3   ,  -0.728 ,  2.55    },
	{  0.728 ,  -1.3   ,  2.55    },
	// 241
	{  0.0   ,  -1.3   ,  2.55    },
	{  1.3   ,   0.0   ,  2.4     },
	{  1.3   ,  -0.728 ,  2.4     },
	{  0.728 ,  -1.3   ,  2.4     },
	{  0.0   ,  -1.3   ,  2.4     },
	{ -0.224 ,  -0.4   ,  2.55    },
	{ -0.4   ,  -0.224 ,  2.55    },
	{ -0.4   ,   0.0   ,  2.55    },
	{ -0.728 ,  -1.3   ,  2.55    },
	{ -1.3   ,  -0.728 ,  2.55    },
	// 251
	{ -1.3   ,   0.0   ,  2.55    },
	{ -0.728 ,  -1.3   ,  2.4     },
	{ -1.3   ,  -0.728 ,  2.4     },
	{ -1.3   ,   0.0   ,  2.4     },
	{ -0.4   ,   0.224 ,  2.55    },
	{ -0.224 ,   0.4   ,  2.55    },
	{  0.0   ,   0.4   ,  2.55    },
	{ -1.3   ,   0.728 ,  2.55    },
	{ -0.728 ,   1.3   ,  2.55    },
	{  0.0   ,   1.3   ,  2.55    },
	// 261
	{ -1.3   ,   0.728 ,  2.4     },
	{ -0.728 ,   1.3   ,  2.4     },
	{  0.0   ,   1.3   ,  2.4     },
	{  0.224 ,   0.4   ,  2.55    },
	{  0.4   ,   0.224 ,  2.55    },
	{  0.728 ,   1.3   ,  2.55    },
	{  1.3   ,   0.728 ,  2.55    },
	{  0.728 ,   1.3   ,  2.4     },
	{  1.3   ,   0.728 ,  2.4     },
};
#define TEAPOT_NB_PATCHES 28
#define ORDER 3
unsigned short teapot_patches[][ORDER + 1][ORDER + 1] = {
	// rim
	{ {   1,   2,   3,   4 }, {   5,   6,   7,   8 }, {   9,  10,  11,  12 }, {  13,  14,  15,  16, } },
	{ {   4,  17,  18,  19 }, {   8,  20,  21,  22 }, {  12,  23,  24,  25 }, {  16,  26,  27,  28, } },
	{ {  19,  29,  30,  31 }, {  22,  32,  33,  34 }, {  25,  35,  36,  37 }, {  28,  38,  39,  40, } },
	{ {  31,  41,  42,   1 }, {  34,  43,  44,   5 }, {  37,  45,  46,   9 }, {  40,  47,  48,  13, } },
	// body
	{ {  13,  14,  15,  16 }, {  49,  50,  51,  52 }, {  53,  54,  55,  56 }, {  57,  58,  59,  60, } },
	{ {  16,  26,  27,  28 }, {  52,  61,  62,  63 }, {  56,  64,  65,  66 }, {  60,  67,  68,  69, } },
	{ {  28,  38,  39,  40 }, {  63,  70,  71,  72 }, {  66,  73,  74,  75 }, {  69,  76,  77,  78, } },
	{ {  40,  47,  48,  13 }, {  72,  79,  80,  49 }, {  75,  81,  82,  53 }, {  78,  83,  84,  57, } },
	{ {  57,  58,  59,  60 }, {  85,  86,  87,  88 }, {  89,  90,  91,  92 }, {  93,  94,  95,  96, } },
	{ {  60,  67,  68,  69 }, {  88,  97,  98,  99 }, {  92, 100, 101, 102 }, {  96, 103, 104, 105, } },
	{ {  69,  76,  77,  78 }, {  99, 106, 107, 108 }, { 102, 109, 110, 111 }, { 105, 112, 113, 114, } },
	{ {  78,  83,  84,  57 }, { 108, 115, 116,  85 }, { 111, 117, 118,  89 }, { 114, 119, 120,  93, } },
	// handle
	{ { 121, 122, 123, 124 }, { 125, 126, 127, 128 }, { 129, 130, 131, 132 }, { 133, 134, 135, 136, } },
	{ { 124, 137, 138, 121 }, { 128, 139, 140, 125 }, { 132, 141, 142, 129 }, { 136, 143, 144, 133, } },
	{ { 133, 134, 135, 136 }, { 145, 146, 147, 148 }, { 149, 150, 151, 152 }, {  69, 153, 154, 155, } },
	{ { 136, 143, 144, 133 }, { 148, 156, 157, 145 }, { 152, 158, 159, 149 }, { 155, 160, 161,  69, } },
	// spout
	{ { 162, 163, 164, 165 }, { 166, 167, 168, 169 }, { 170, 171, 172, 173 }, { 174, 175, 176, 177, } },
	{ { 165, 178, 179, 162 }, { 169, 180, 181, 166 }, { 173, 182, 183, 170 }, { 177, 184, 185, 174, } },
	{ { 174, 175, 176, 177 }, { 186, 187, 188, 189 }, { 190, 191, 192, 193 }, { 194, 195, 196, 197, } },
	{ { 177, 184, 185, 174 }, { 189, 198, 199, 186 }, { 193, 200, 201, 190 }, { 197, 202, 203, 194, } },
	// lid
	{ { 204, 204, 204, 204 }, { 207, 208, 209, 210 }, { 211, 211, 211, 211 }, { 212, 213, 214, 215, } },
	{ { 204, 204, 204, 204 }, { 210, 217, 218, 219 }, { 211, 211, 211, 211 }, { 215, 220, 221, 222, } },
	{ { 204, 204, 204, 204 }, { 219, 224, 225, 226 }, { 211, 211, 211, 211 }, { 222, 227, 228, 229, } },
	{ { 204, 204, 204, 204 }, { 226, 230, 231, 207 }, { 211, 211, 211, 211 }, { 229, 232, 233, 212, } },
	{ { 212, 213, 214, 215 }, { 234, 235, 236, 237 }, { 238, 239, 240, 241 }, { 242, 243, 244, 245, } },
	{ { 215, 220, 221, 222 }, { 237, 246, 247, 248 }, { 241, 249, 250, 251 }, { 245, 252, 253, 254, } },
	{ { 222, 227, 228, 229 }, { 248, 255, 256, 257 }, { 251, 258, 259, 260 }, { 254, 261, 262, 263, } },
	{ { 229, 232, 233, 212 }, { 257, 264, 265, 234 }, { 260, 266, 267, 238 }, { 263, 268, 269, 242, } },
	// no bottom!
};
#define RESU 10
#define RESV 10
static struct vertex  teapot_vertices[TEAPOT_NB_PATCHES * RESU*RESV];
static unsigned short teapot_elements[TEAPOT_NB_PATCHES * ( RESU - 1 )*( RESV - 1 ) * 2 * 3];
static double         teapot_colors[TEAPOT_NB_PATCHES * RESU*RESV * 3];

static unsigned short teapot_cp_elements[TEAPOT_NB_PATCHES][ORDER + 1][ORDER + 1];
static double         teapot_cp_colors[269 * 3];

static void build_control_points_k( int p, struct vertex control_points_k[][ORDER + 1] );
static struct vertex compute_position( struct vertex control_points_k[][ORDER + 1], double u, double v );
static double bernstein_polynomial( int i, int n, double u );
static double binomial_coefficient( int i, int n );
static int factorial( int n );

static void build_teapot() {
	// Vertices
	for( int p = 0; p < TEAPOT_NB_PATCHES; p++ ) {
		struct vertex control_points_k[ORDER + 1][ORDER + 1];
		build_control_points_k( p, control_points_k );
		for( int ru = 0; ru <= RESU - 1; ru++ ) {
			double u = 1.0 * ru / ( RESU - 1 );
			for( int rv = 0; rv <= RESV - 1; rv++ ) {
				double v = 1.0 * rv / ( RESV - 1 );
				teapot_vertices[p*RESU*RESV + ru * RESV + rv] = compute_position( control_points_k, u, v );
				teapot_colors[p*RESU*RESV * 3 + ru * RESV * 3 + rv * 3 + 0] = 1.0 * p / TEAPOT_NB_PATCHES;
				teapot_colors[p*RESU*RESV * 3 + ru * RESV * 3 + rv * 3 + 1] = 1.0 * p / TEAPOT_NB_PATCHES;
				teapot_colors[p*RESU*RESV * 3 + ru * RESV * 3 + rv * 3 + 2] = 0.8;
			}
		}
	}

	// Elements
	int n = 0;
	for( int p = 0; p < TEAPOT_NB_PATCHES; p++ )
		for( int ru = 0; ru < RESU - 1; ru++ )
			for( int rv = 0; rv < RESV - 1; rv++ ) {
				// 1 square ABCD = 2 triangles ABC + CDA
				// ABC
				teapot_elements[n] = p * RESU*RESV + ru * RESV + rv; n++;
				teapot_elements[n] = p * RESU*RESV + ru * RESV + ( rv + 1 ); n++;
				teapot_elements[n] = p * RESU*RESV + ( ru + 1 )*RESV + ( rv + 1 ); n++;
				// CDA
				teapot_elements[n] = p * RESU*RESV + ( ru + 1 )*RESV + ( rv + 1 ); n++;
				teapot_elements[n] = p * RESU*RESV + ( ru + 1 )*RESV + rv; n++;
				teapot_elements[n] = p * RESU*RESV + ru * RESV + rv; n++;
			}

	// Control points elements for debugging
	memset( teapot_cp_colors, 0, sizeof( teapot_cp_colors ) ); // black
	for( int p = 0; p < TEAPOT_NB_PATCHES; p++ )
		for( int i = 0; i < ( ORDER + 1 ); i++ )
			for( int j = 0; j < ( ORDER + 1 ); j++ )
				teapot_cp_elements[p][i][j] = teapot_patches[p][i][j] - 1;
}

static void build_control_points_k( int p, struct vertex control_points_k[][ORDER + 1] ) {
	for( int i = 0; i <= ORDER; i++ ) {
		for( int j = 0; j <= ORDER; j++ ) {
			control_points_k[i][j].x = teapot_cp_vertices[teapot_patches[p][i][j] - 1].x;
			control_points_k[i][j].y = teapot_cp_vertices[teapot_patches[p][i][j] - 1].y;
			control_points_k[i][j].z = teapot_cp_vertices[teapot_patches[p][i][j] - 1].z;
		}
	}
}

static struct vertex compute_position( struct vertex control_points_k[][ORDER + 1], double u, double v ) {
	struct vertex result = { 0.0, 0.0, 0.0 };
	for( int i = 0; i <= ORDER; i++ ) {
		double poly_i = bernstein_polynomial( i, ORDER, u );
		for( int j = 0; j <= ORDER; j++ ) {
			double poly_j = bernstein_polynomial( j, ORDER, v );
			result.x += poly_i * poly_j * control_points_k[i][j].x;
			result.y += poly_i * poly_j * control_points_k[i][j].y;
			result.z += poly_i * poly_j * control_points_k[i][j].z;
		}
	}
	return result;
}

static double bernstein_polynomial( int i, int n, double u ) {
	return binomial_coefficient( i, n ) * pow( u, i ) * pow( 1 - u, n - i );
}

static double binomial_coefficient( int i, int n ) {
	assert( i >= 0 ); assert( n >= 0 );
	return 1.0 * factorial( n ) / ( factorial( i ) * factorial( n - i ) );
}

static int factorial( int n ) {
	assert( n >= 0 );
	int result = 1;
	for( int i = n; i > 1; i-- )
		result *= i;
	return result;
}

#pragma endregion

//VMesh Geometry::teapot(  int resU, int resV )
//{
//	build_teapot();
//
//	//teapot_vertices[TEAPOT_NB_PATCHES * RESU*RESV];
//	//teapot_elements[TEAPOT_NB_PATCHES * (RESU-1)*(RESV-1) * 2*3];
//	//teapot_colors[TEAPOT_NB_PATCHES * RESU*RESV * 3];
//	std::vector<Math::dvec3> vertices;
//	std::vector<unsigned int> indices;
//
//	for( int i=0; i<TEAPOT_NB_PATCHES * RESU*RESV; ++i )
//		vertices.push_back( Math::dvec3(teapot_vertices[i].x,teapot_vertices[i].y,teapot_vertices[i].z) );
//
//	for( int i=0; i<TEAPOT_NB_PATCHES * (RESU-1)*(RESV-1) * 2*3; ++i )
//		indices.push_back( teapot_elements[i] );
//
//	VMesh mesh(vertices, indices);
//	mesh.computeNormals();
//	return mesh;
//}
}