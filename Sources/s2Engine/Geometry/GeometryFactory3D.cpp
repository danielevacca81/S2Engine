// GeometryFactory3D.cpp
// 
#include "GeometryFactory3D.h"

#include "GeometryFactory2D.h"
#include "Math/Space.h"

#include <array>

using namespace s2;

// ------------------------------------------------------------------------------------------------
static inline std::vector<Math::dvec3> generateCircle3D( const Math::dvec3& center, 
														 const Math::dvec3& normal,
														 double radius, int slices )
{
	std::vector<Math::dvec3> circlePoints;
	const Math::dmat4 rotateAroundCenter = Math::localFrame( normal );
	const double step = Math::two_pi<double>() / slices;
	for( int i = 0; i < slices; ++i )
	{
		const double angle = i * step;
		const double cx = radius * Math::cos( angle );
		const double cy = radius * Math::sin( angle );
		
		// Build a circle on the YZ plane and rotate it to align with the normal
		const Math::dvec3 p = rotateAroundCenter * Math::dvec4( 0.0, cx, cy, 1.0 );
		circlePoints.push_back( center + p );
	}
	return circlePoints;
}

// ------------------------------------------------------------------------------------------------
MeshData3D GeometryFactory3D::createTorus( double innerRadius, double outerRadius, int numc, int numt )
{
	if( innerRadius <= 0.0 || outerRadius <= 0.0 || numc < 3 || numt < 3 )
		return {};

	MeshData3D g;

	// numc = number of circles (sectors)
	for( int i = 0; i < numc; ++i )
	{
		// numt = points per circle
		for( int j = 0; j < numt; ++j )
		{
			const double t = Math::two_pi<double>() * i / (double) numc;
			const double p = Math::two_pi<double>() * j / (double) numt;

			const double x = ( innerRadius + outerRadius * Math::cos( p ) ) * Math::cos( t );
			const double y = ( innerRadius + outerRadius * Math::cos( p ) ) * Math::sin( t );
			const double z =  outerRadius * Math::sin( p );

			g.vertices.push_back( Math::dvec3( x, y, z ) );

			const Math::dvec3 T( -Math::sin( t ), Math::cos( t ), 0 );
			const Math::dvec3 B( Math::cos( t )*( -Math::sin( p ) ),
								 Math::sin( t )*( -Math::sin( p ) ),
								 Math::cos( p ) );

			g.normals.push_back( Math::cross( T, B ) );

			// First triangle
			g.indices.push_back( ( i * numt ) + j );
			g.indices.push_back( ( ( ( i + 1 ) % numc ) * numt ) + j );
			g.indices.push_back( ( ( ( i + 1 ) % numc ) * numt ) + ( j + 1 ) % numt );

			// Second triangle
			g.indices.push_back( ( i * numt ) + j );
			g.indices.push_back( ( ( ( i + 1 ) % numc ) * numt ) + ( j + 1 ) % numt );
			g.indices.push_back( ( i * numt ) + ( j + 1 ) % numt );
		}
	}

	return g;
}

// ------------------------------------------------------------------------------------------------
MeshData3D GeometryFactory3D::createCylinder(
	const Math::dvec3& startPoint,
	const Math::dvec3& endPoint,
	double radius,
	bool capStart,
	bool capEnd,
	int slices )
{
	if( slices < 4 )
		return {};

	// Check for degenerate case
	if( Math::length( endPoint - startPoint ) < Math::epsilon<double>() )
		return {};

	const Math::dvec3 dir = Math::normalize( endPoint - startPoint );
	const double len = Math::length( endPoint - startPoint );

	// Generate base circle
	const std::vector<Math::dvec3> circle = generateCircle3D( startPoint, dir, radius, slices );

	MeshData3D cylinder;

	// Generate vertices for cylinder sides (base + top rings)
	for( int i = 0; i < slices; ++i )
	{
		const Math::dvec3 pBase = circle[i];
		const Math::dvec3 pTop = pBase + dir * len;

		cylinder.vertices.emplace_back( pBase );
		cylinder.vertices.emplace_back( pTop );

		// Radial normal (perpendicular to cylinder axis)
		const auto n = Math::normalize( pBase - startPoint );
		cylinder.normals.emplace_back( n );
		cylinder.normals.emplace_back( n );
	}

	// Generate indices for cylinder sides
	const int loopIndex = slices * 2;
	for( int idx = 0; idx < slices * 2; idx += 2 )
	{
		// Two triangles per quad
		// Triangle 1: base[i], top[i], top[i+1]
		cylinder.indices.emplace_back( idx + 0 );
		cylinder.indices.emplace_back( ( idx + 3 ) % loopIndex );
		cylinder.indices.emplace_back( idx + 1 );

		// Triangle 2: base[i], top[i+1], base[i+1]
		cylinder.indices.emplace_back( idx + 0 );
		cylinder.indices.emplace_back( ( idx + 2 ) % loopIndex );
		cylinder.indices.emplace_back( ( idx + 3 ) % loopIndex );
	}

	// Add start cap if requested
	if( capStart )
	{
		const int centerIdx = (int) cylinder.vertices.size();

		// Center vertex
		cylinder.vertices.emplace_back( startPoint );
		cylinder.normals.emplace_back( -dir );

		// Circle vertices (duplicated with -dir normal)
		for( int i = 0; i < slices; ++i )
		{
			cylinder.vertices.emplace_back( circle[i] );
			cylinder.normals.emplace_back( -dir );
		}

		// Fan triangulation (CW winding when viewed from -dir)
		for( int i = 0; i < slices; ++i )
		{
			cylinder.indices.emplace_back( centerIdx );
			cylinder.indices.emplace_back( centerIdx + 1 + ( i + 1 ) % slices );
			cylinder.indices.emplace_back( centerIdx + 1 + i );
		}
	}

	// Add end cap if requested
	if( capEnd )
	{
		const int centerIdx = (int) cylinder.vertices.size();

		// Center vertex
		cylinder.vertices.emplace_back( endPoint );
		cylinder.normals.emplace_back( dir );

		// Circle vertices (duplicated with +dir normal)
		for( int i = 0; i < slices; ++i )
		{
			cylinder.vertices.emplace_back( circle[i] + dir * len );
			cylinder.normals.emplace_back( dir );
		}

		// Fan triangulation (CCW winding when viewed from +dir)
		for( int i = 0; i < slices; ++i )
		{
			cylinder.indices.emplace_back( centerIdx );
			cylinder.indices.emplace_back( centerIdx + 1 + i );
			cylinder.indices.emplace_back( centerIdx + 1 + ( i + 1 ) % slices );
		}
	}

	return cylinder;
}

// ------------------------------------------------------------------------------------------------
MeshData3D GeometryFactory3D::createSphere( const Math::dvec3& center, double radius, int slices )
{
	if( slices < 4 )
		return {};

	MeshData3D sphere;
	const int    rings = slices;
	const double twopi = Math::two_pi<double>();
	const double dTheta = twopi / double( slices );
	const double dPhi   = Math::pi<double>() / double( rings );
	
	// Generate vertices and normals
	for( int r = 0; r < rings + 1; ++r )
	{
		const double phi    = Math::half_pi<double>() - double( r ) * dPhi;
		const double cosPhi = Math::cos( phi );
		const double sinPhi = Math::sin( phi );
		
		for( int s = 0; s < slices + 1; ++s )
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
			
			sphere.vertices.emplace_back( center + p * radius );
			sphere.normals.emplace_back( -p );
		}
	}
	
	// Generate indices
	for( int r = 0; r < rings; ++r )
	{
		for( int s = 0; s < slices; ++s )
		{
			const int current = r * ( slices + 1 ) + s;
			const int next    = current + slices + 1;
			
			// First triangle
			sphere.indices.emplace_back( current );
			sphere.indices.emplace_back( next );
			sphere.indices.emplace_back( current + 1 );
			
			// Second triangle
			sphere.indices.emplace_back( current + 1 );
			sphere.indices.emplace_back( next );
			sphere.indices.emplace_back( next + 1 );
		}
	}

	return sphere;
}

// ------------------------------------------------------------------------------------------------
MeshData3D GeometryFactory3D::createCone( const Math::dvec3& center, const Math::dvec3& tip, double baseRadius, bool cap, int slices )
{
	if( slices < 4 )
		return {};

	// Check for degenerate case
	if( Math::length( tip - center ) < Math::epsilon<double>() )
		return {};

	const Math::dvec3 dir = Math::normalize( tip - center );
	const std::vector<Math::dvec3> circle = generateCircle3D( center, dir, baseRadius, slices );

	MeshData3D cone;
	
	// Vertex 0: tip
	cone.vertices.emplace_back( tip );
	cone.normals.emplace_back( dir );
	
	// Vertices 1..slices: cone base
	for( int i = 0; i < slices; ++i )
	{
		const auto &p0 = circle[i];
		const auto &p1 = circle[( i + 1 ) % slices];
		
		cone.vertices.emplace_back( p0 );
		
		// Compute normal for triangle [tip, p0, p1]
		const auto edge1 = p0 - tip;
		const auto edge2 = p1 - tip;
		const auto n = Math::normalize( Math::cross( edge1, edge2 ) );
		cone.normals.emplace_back( n );
	}
	
	// Generate indices for cone sides
	for( int i = 0; i < slices; ++i )
	{
		const int current = i + 1;                    // Current base vertex
		const int next = ( i + 1 ) % slices + 1;      // Next base vertex
		
		// Triangle: tip -> current -> next (CCW winding)
		cone.indices.emplace_back( 0 );
		cone.indices.emplace_back( current );
		cone.indices.emplace_back( next );
	}
	
	// Add cap if requested
	if( cap )
	{
		const int firstCapVertex = (int)cone.vertices.size();
		
		// Add vertices for cap (duplicated with -dir normal)
		for( int i = 0; i < slices; ++i )
		{
			cone.vertices.emplace_back( circle[i] );
			cone.normals.emplace_back( -dir );
		}
		
		// Generate indices for cap (fan triangulation)
		for( int i = 2; i < slices; ++i )
		{
			cone.indices.emplace_back( firstCapVertex );
			cone.indices.emplace_back( firstCapVertex + i );
			cone.indices.emplace_back( firstCapVertex + i - 1 );
		}
	}
	
	return cone;
}

// ------------------------------------------------------------------------------------------------
MeshData3D GeometryFactory3D::createCube( const Math::dvec3& center, double size )
{
	MeshData3D cube;
	const double halfSize = size * 0.5;
	
	// 24 vertices (4 per face for proper normals)
	cube.vertices =
	{
		// Front face
		Math::dvec3( center.x - halfSize, center.y - halfSize, center.z + halfSize ), // 0
		Math::dvec3( center.x + halfSize, center.y - halfSize, center.z + halfSize ), // 1
		Math::dvec3( center.x + halfSize, center.y + halfSize, center.z + halfSize ), // 2
		Math::dvec3( center.x - halfSize, center.y + halfSize, center.z + halfSize ), // 3

		// Right face
		Math::dvec3( center.x + halfSize, center.y - halfSize, center.z + halfSize ), // 4
		Math::dvec3( center.x + halfSize, center.y - halfSize, center.z - halfSize ), // 5
		Math::dvec3( center.x + halfSize, center.y + halfSize, center.z - halfSize ), // 6
		Math::dvec3( center.x + halfSize, center.y + halfSize, center.z + halfSize ), // 7

		// Back face
		Math::dvec3( center.x + halfSize, center.y - halfSize, center.z - halfSize ), // 8
		Math::dvec3( center.x - halfSize, center.y - halfSize, center.z - halfSize ), // 9
		Math::dvec3( center.x - halfSize, center.y + halfSize, center.z - halfSize ), // 10
		Math::dvec3( center.x + halfSize, center.y + halfSize, center.z - halfSize ), // 11

		// Left face
		Math::dvec3( center.x - halfSize, center.y - halfSize, center.z - halfSize ), // 12
		Math::dvec3( center.x - halfSize, center.y - halfSize, center.z + halfSize ), // 13
		Math::dvec3( center.x - halfSize, center.y + halfSize, center.z + halfSize ), // 14
		Math::dvec3( center.x - halfSize, center.y + halfSize, center.z - halfSize ), // 15

		// Top face
		Math::dvec3( center.x - halfSize, center.y + halfSize, center.z + halfSize ), // 16
		Math::dvec3( center.x + halfSize, center.y + halfSize, center.z + halfSize ), // 17
		Math::dvec3( center.x + halfSize, center.y + halfSize, center.z - halfSize ), // 18
		Math::dvec3( center.x - halfSize, center.y + halfSize, center.z - halfSize ), // 19

		// Bottom face
		Math::dvec3( center.x - halfSize, center.y - halfSize, center.z - halfSize ), // 20
		Math::dvec3( center.x + halfSize, center.y - halfSize, center.z - halfSize ), // 21
		Math::dvec3( center.x + halfSize, center.y - halfSize, center.z + halfSize ), // 22
		Math::dvec3( center.x - halfSize, center.y - halfSize, center.z + halfSize ), // 23
	};
	
	// Normals (one per face, repeated for each vertex)
	cube.normals =
	{
		Math::dvec3(  0.0,  0.0,  1.0 ),
		Math::dvec3(  0.0,  0.0,  1.0 ),
		Math::dvec3(  0.0,  0.0,  1.0 ),
		Math::dvec3(  0.0,  0.0,  1.0 ),

		Math::dvec3(  1.0,  0.0,  0.0 ),
		Math::dvec3(  1.0,  0.0,  0.0 ),
		Math::dvec3(  1.0,  0.0,  0.0 ),
		Math::dvec3(  1.0,  0.0,  0.0 ),

		Math::dvec3(  0.0,  0.0, -1.0 ),
		Math::dvec3(  0.0,  0.0, -1.0 ),
		Math::dvec3(  0.0,  0.0, -1.0 ),
		Math::dvec3(  0.0,  0.0, -1.0 ),

		Math::dvec3( -1.0,  0.0,  0.0 ),
		Math::dvec3( -1.0,  0.0,  0.0 ),
		Math::dvec3( -1.0,  0.0,  0.0 ),
		Math::dvec3( -1.0,  0.0,  0.0 ),

		Math::dvec3(  0.0,  1.0,  0.0 ),
		Math::dvec3(  0.0,  1.0,  0.0 ),
		Math::dvec3(  0.0,  1.0,  0.0 ),
		Math::dvec3(  0.0,  1.0,  0.0 ),

		Math::dvec3(  0.0, -1.0,  0.0 ),
		Math::dvec3(  0.0, -1.0,  0.0 ),
		Math::dvec3(  0.0, -1.0,  0.0 ),
		Math::dvec3(  0.0, -1.0,  0.0 ),
	};

	// Indices (2 triangles per face)
	cube.indices =
	{
		0, 1, 2,   0, 2, 3,    // Front face
		4, 5, 6,   4, 6, 7,    // Right face
		8, 9, 10,  8, 10, 11,  // Back face
		12, 13, 14, 12, 14, 15, // Left face		
		16, 17, 18, 16, 18, 19, // Top face
		20, 21, 22, 20, 22, 23  // Bottom face
	};
	
	return cube;
}

// ------------------------------------------------------------------------------------------------
// Teapot generation using Bézier patches
// ------------------------------------------------------------------------------------------------
namespace {

// Teapot control points data (Utah Teapot by Martin Newell, 1975)
constexpr std::array<Math::dvec3, 269> TEAPOT_CONTROL_POINTS = {
	Math::dvec3{  1.4   ,   0.0   ,  2.4     }, Math::dvec3{  1.4   ,  -0.784 ,  2.4     },
	Math::dvec3{  0.784 ,  -1.4   ,  2.4     }, Math::dvec3{  0.0   ,  -1.4   ,  2.4     },
	Math::dvec3{  1.3375,   0.0   ,  2.53125 }, Math::dvec3{  1.3375,  -0.749 ,  2.53125 },
	Math::dvec3{  0.749 ,  -1.3375,  2.53125 }, Math::dvec3{  0.0   ,  -1.3375,  2.53125 },
	Math::dvec3{  1.4375,    0.0  ,  2.53125 }, Math::dvec3{  1.4375,  -0.805 ,  2.53125 },
	Math::dvec3{  0.805 ,  -1.4375,  2.53125 }, Math::dvec3{  0.0   ,  -1.4375,  2.53125 },
	Math::dvec3{  1.5   ,   0.0   ,  2.4     }, Math::dvec3{  1.5   ,  -0.84  ,  2.4     },
	Math::dvec3{  0.84  ,  -1.5   ,  2.4     }, Math::dvec3{  0.0   ,  -1.5   ,  2.4     },
	Math::dvec3{ -0.784 ,  -1.4   ,  2.4     }, Math::dvec3{ -1.4   ,  -0.784 ,  2.4     },
	Math::dvec3{ -1.4   ,   0.0   ,  2.4     }, Math::dvec3{ -0.749 ,  -1.3375,  2.53125 },
	Math::dvec3{ -1.3375,  -0.749 ,  2.53125 }, Math::dvec3{ -1.3375,   0.0   ,  2.53125 },
	Math::dvec3{ -0.805 ,  -1.4375,  2.53125 }, Math::dvec3{ -1.4375,  -0.805 ,  2.53125 },
	Math::dvec3{ -1.4375,   0.0   ,  2.53125 }, Math::dvec3{ -0.84  ,  -1.5   ,  2.4     },
	Math::dvec3{ -1.5   ,  -0.84  ,  2.4     }, Math::dvec3{ -1.5   ,   0.0   ,  2.4     },
	Math::dvec3{ -1.4   ,   0.784 ,  2.4     }, Math::dvec3{ -0.784 ,   1.4   ,  2.4     },
	Math::dvec3{  0.0   ,   1.4   ,  2.4     }, Math::dvec3{ -1.3375,   0.749 ,  2.53125 },
	Math::dvec3{ -0.749 ,   1.3375,  2.53125 }, Math::dvec3{  0.0   ,   1.3375,  2.53125 },
	Math::dvec3{ -1.4375,   0.805 ,  2.53125 }, Math::dvec3{ -0.805 ,   1.4375,  2.53125 },
	Math::dvec3{  0.0   ,   1.4375,  2.53125 }, Math::dvec3{ -1.5   ,   0.84  ,  2.4     },
	Math::dvec3{ -0.84  ,   1.5   ,  2.4     }, Math::dvec3{  0.0   ,   1.5   ,  2.4     },
	Math::dvec3{  0.784 ,   1.4   ,  2.4     }, Math::dvec3{  1.4   ,   0.784 ,  2.4     },
	Math::dvec3{  0.749 ,   1.3375,  2.53125 }, Math::dvec3{  1.3375,   0.749 ,  2.53125 },
	Math::dvec3{  0.805 ,   1.4375,  2.53125 }, Math::dvec3{  1.4375,   0.805 ,  2.53125 },
	Math::dvec3{  0.84  ,   1.5   ,  2.4     }, Math::dvec3{  1.5   ,   0.84  ,  2.4     },
	Math::dvec3{  1.75  ,   0.0   ,  1.875   }, Math::dvec3{  1.75  ,  -0.98  ,  1.875   },
	Math::dvec3{  0.98  ,  -1.75  ,  1.875   }, Math::dvec3{  0.0   ,  -1.75  ,  1.875   },
	Math::dvec3{  2.0   ,   0.0   ,  1.35    }, Math::dvec3{  2.0   ,  -1.12  ,  1.35    },
	Math::dvec3{  1.12  ,  -2.0   ,  1.35    }, Math::dvec3{  0.0   ,  -2.0   ,  1.35    },
	Math::dvec3{  2.0   ,   0.0   ,  0.9     }, Math::dvec3{  2.0   ,  -1.12  ,  0.9     },
	Math::dvec3{  1.12  ,  -2.0   ,  0.9     }, Math::dvec3{  0.0   ,  -2.0   ,  0.9     },
	Math::dvec3{ -0.98  ,  -1.75  ,  1.875   }, Math::dvec3{ -1.75  ,  -0.98  ,  1.875   },
	Math::dvec3{ -1.75  ,   0.0   ,  1.875   }, Math::dvec3{ -1.12  ,  -2.0   ,  1.35    },
	Math::dvec3{ -2.0   ,  -1.12  ,  1.35    }, Math::dvec3{ -2.0   ,   0.0   ,  1.35    },
	Math::dvec3{ -1.12  ,  -2.0   ,  0.9     }, Math::dvec3{ -2.0   ,  -1.12  ,  0.9     },
	Math::dvec3{ -2.0   ,   0.0   ,  0.9     }, Math::dvec3{ -1.75  ,   0.98  ,  1.875   },
	Math::dvec3{ -0.98  ,   1.75  ,  1.875   }, Math::dvec3{  0.0   ,   1.75  ,  1.875   },
	Math::dvec3{ -2.0   ,   1.12  ,  1.35    }, Math::dvec3{ -1.12  ,   2.0   ,  1.35    },
	Math::dvec3{  0.0   ,   2.0   ,  1.35    }, Math::dvec3{ -2.0   ,   1.12  ,  0.9     },
	Math::dvec3{ -1.12  ,   2.0   ,  0.9     }, Math::dvec3{  0.0   ,   2.0   ,  0.9     },
	Math::dvec3{  0.98  ,   1.75  ,  1.875   }, Math::dvec3{  1.75  ,   0.98  ,  1.875   },
	Math::dvec3{  1.12  ,   2.0   ,  1.35    }, Math::dvec3{  2.0   ,   1.12  ,  1.35    },
	Math::dvec3{  1.12  ,   2.0   ,  0.9     }, Math::dvec3{  2.0   ,   1.12  ,  0.9     },
	Math::dvec3{  2.0   ,   0.0   ,  0.45    }, Math::dvec3{  2.0   ,  -1.12  ,  0.45    },
	Math::dvec3{  1.12  ,  -2.0   ,  0.45    }, Math::dvec3{  0.0   ,  -2.0   ,  0.45    },
	Math::dvec3{  1.5   ,   0.0   ,  0.225   }, Math::dvec3{  1.5   ,  -0.84  ,  0.225   },
	Math::dvec3{  0.84  ,  -1.5   ,  0.225   }, Math::dvec3{  0.0   ,  -1.5   ,  0.225   },
	Math::dvec3{  1.5   ,   0.0   ,  0.15    }, Math::dvec3{  1.5   ,  -0.84  ,  0.15    },
	Math::dvec3{  0.84  ,  -1.5   ,  0.15    }, Math::dvec3{  0.0   ,  -1.5   ,  0.15    },
	Math::dvec3{ -1.12  ,  -2.0   ,  0.45    }, Math::dvec3{ -2.0   ,  -1.12  ,  0.45    },
	Math::dvec3{ -2.0   ,   0.0   ,  0.45    }, Math::dvec3{ -0.84  ,  -1.5   ,  0.225   },
	Math::dvec3{ -1.5   ,  -0.84  ,  0.225   }, Math::dvec3{ -1.5   ,   0.0   ,  0.225   },
	Math::dvec3{ -0.84  ,  -1.5   ,  0.15    }, Math::dvec3{ -1.5   ,  -0.84  ,  0.15    },
	Math::dvec3{ -1.5   ,   0.0   ,  0.15    }, Math::dvec3{ -2.0   ,   1.12  ,  0.45    },
	Math::dvec3{ -1.12  ,   2.0   ,  0.45    }, Math::dvec3{  0.0   ,   2.0   ,  0.45    },
	Math::dvec3{ -1.5   ,   0.84  ,  0.225   }, Math::dvec3{ -0.84  ,   1.5   ,  0.225   },
	Math::dvec3{  0.0   ,   1.5   ,  0.225   }, Math::dvec3{ -1.5   ,   0.84  ,  0.15    },
	Math::dvec3{ -0.84  ,   1.5   ,  0.15    }, Math::dvec3{  0.0   ,   1.5   ,  0.15    },
	Math::dvec3{  1.12  ,   2.0   ,  0.15    }, Math::dvec3{  2.0   ,   1.12  ,  0.45    },
	Math::dvec3{  0.84  ,   1.5   ,  0.225   }, Math::dvec3{  1.5   ,   0.84  ,  0.225   },
	Math::dvec3{  0.84  ,   1.5   ,  0.15    }, Math::dvec3{  1.5   ,   0.84  ,  0.15    },
	Math::dvec3{ -1.6   ,   0.0   ,  2.025   }, Math::dvec3{ -1.6   ,  -0.3   ,  2.025   },
	Math::dvec3{ -1.5   ,  -0.3   ,  2.25    }, Math::dvec3{ -1.5   ,   0.0   ,  2.25    },
	Math::dvec3{ -2.3   ,   0.0   ,  2.025   }, Math::dvec3{ -2.3   ,  -0.3   ,  2.025   },
	Math::dvec3{ -2.5   ,  -0.3   ,  2.25    }, Math::dvec3{ -2.5   ,   0.0   ,  2.25    },
	Math::dvec3{ -2.7   ,   0.0   ,  2.025   }, Math::dvec3{ -2.7   ,  -0.3   ,  2.025   },
	Math::dvec3{ -3.0   ,  -0.3   ,  2.25    }, Math::dvec3{ -3.0   ,   0.0   ,  2.25    },
	Math::dvec3{ -2.7   ,   0.0   ,  1.8     }, Math::dvec3{ -2.7   ,  -0.3   ,  1.8     },
	Math::dvec3{ -3.0   ,  -0.3   ,  1.8     }, Math::dvec3{ -3.0   ,   0.0   ,  1.8     },
	Math::dvec3{ -1.5   ,   0.3   ,  2.25    }, Math::dvec3{ -1.6   ,   0.3   ,  2.025   },
	Math::dvec3{ -2.5   ,   0.3   ,  2.25    }, Math::dvec3{ -2.3   ,   0.3   ,  2.025   },
	Math::dvec3{ -3.0   ,   0.3   ,  2.25    }, Math::dvec3{ -2.7   ,   0.3   ,  2.025   },
	Math::dvec3{ -3.0   ,   0.3   ,  1.8     }, Math::dvec3{ -2.7   ,   0.3   ,  1.8     },
	Math::dvec3{ -2.7   ,   0.0   ,  1.575   }, Math::dvec3{ -2.7   ,  -0.3   ,  1.575   },
	Math::dvec3{ -3.0   ,  -0.3   ,  1.35    }, Math::dvec3{ -3.0   ,   0.0   ,  1.35    },
	Math::dvec3{ -2.5   ,   0.0   ,  1.125   }, Math::dvec3{ -2.5   ,  -0.3   ,  1.125   },
	Math::dvec3{ -2.65  ,  -0.3   ,  0.9375  }, Math::dvec3{ -2.65  ,   0.0   ,  0.9375  },
	Math::dvec3{ -2.0   ,  -0.3   ,  0.9     }, Math::dvec3{ -1.9   ,  -0.3   ,  0.6     },
	Math::dvec3{ -1.9   ,   0.0   ,  0.6     }, Math::dvec3{ -3.0   ,   0.3   ,  1.35    },
	Math::dvec3{ -2.7   ,   0.3   ,  1.575   }, Math::dvec3{ -2.65  ,   0.3   ,  0.9375  },
	Math::dvec3{ -2.5   ,   0.3   ,  1.1255  }, Math::dvec3{ -1.9   ,   0.3   ,  0.6     },
	Math::dvec3{ -2.0   ,   0.3   ,  0.9     }, Math::dvec3{  1.7   ,   0.0   ,  1.425   },
	Math::dvec3{  1.7   ,  -0.66  ,  1.425   }, Math::dvec3{  1.7   ,  -0.66  ,  0.6     },
	Math::dvec3{  1.7   ,   0.0   ,  0.6     }, Math::dvec3{  2.6   ,   0.0   ,  1.425   },
	Math::dvec3{  2.6   ,  -0.66  ,  1.425   }, Math::dvec3{  3.1   ,  -0.66  ,  0.825   },
	Math::dvec3{  3.1   ,   0.0   ,  0.825   }, Math::dvec3{  2.3   ,   0.0   ,  2.1     },
	Math::dvec3{  2.3   ,  -0.25  ,  2.1     }, Math::dvec3{  2.4   ,  -0.25  ,  2.025   },
	Math::dvec3{  2.4   ,   0.0   ,  2.025   }, Math::dvec3{  2.7   ,   0.0   ,  2.4     },
	Math::dvec3{  2.7   ,  -0.25  ,  2.4     }, Math::dvec3{  3.3   ,  -0.25  ,  2.4     },
	Math::dvec3{  3.3   ,   0.0   ,  2.4     }, Math::dvec3{  1.7   ,   0.66  ,  0.6     },
	Math::dvec3{  1.7   ,   0.66  ,  1.425   }, Math::dvec3{  3.1   ,   0.66  ,  0.825   },
	Math::dvec3{  2.6   ,   0.66  ,  1.425   }, Math::dvec3{  2.4   ,   0.25  ,  2.025   },
	Math::dvec3{  2.3   ,   0.25  ,  2.1     }, Math::dvec3{  3.3   ,   0.25  ,  2.4     },
	Math::dvec3{  2.7   ,   0.25  ,  2.4     }, Math::dvec3{  2.8   ,   0.0   ,  2.475   },
	Math::dvec3{  2.8   ,  -0.25  ,  2.475   }, Math::dvec3{  3.525 ,  -0.25  ,  2.49375 },
	Math::dvec3{  3.525 ,   0.0   ,  2.49375 }, Math::dvec3{  2.9   ,   0.0   ,  2.475   },
	Math::dvec3{  2.9   ,  -0.15  ,  2.475   }, Math::dvec3{  3.45  ,  -0.15  ,  2.5125  },
	Math::dvec3{  3.45  ,   0.0   ,  2.5125  }, Math::dvec3{  2.8   ,   0.0   ,  2.4     },
	Math::dvec3{  2.8   ,  -0.15  ,  2.4     }, Math::dvec3{  3.2   ,  -0.15  ,  2.4     },
	Math::dvec3{  3.2   ,   0.0   ,  2.4     }, Math::dvec3{  3.525 ,   0.25  ,  2.49375 },
	Math::dvec3{  2.8   ,   0.25  ,  2.475   }, Math::dvec3{  3.45  ,   0.15  ,  2.5125  },
	Math::dvec3{  2.9   ,   0.15  ,  2.475   }, Math::dvec3{  3.2   ,   0.15  ,  2.4     },
	Math::dvec3{  2.8   ,   0.15  ,  2.4     }, Math::dvec3{  0.0   ,   0.0   ,  3.15    },
	Math::dvec3{  0.0   ,  -0.002 ,  3.15    }, Math::dvec3{  0.002 ,   0.0   ,  3.15    },
	Math::dvec3{  0.8   ,   0.0   ,  3.15    }, Math::dvec3{  0.8   ,  -0.45  ,  3.15    },
	Math::dvec3{  0.45  ,  -0.8   ,  3.15    }, Math::dvec3{  0.0   ,  -0.8   ,  3.15    },
	Math::dvec3{  0.0   ,   0.0   ,  2.85    }, Math::dvec3{  0.2   ,   0.0   ,  2.7     },
	Math::dvec3{  0.2   ,  -0.112 ,  2.7     }, Math::dvec3{  0.112 ,  -0.2   ,  2.7     },
	Math::dvec3{  0.0   ,  -0.2   ,  2.7     }, Math::dvec3{ -0.002 ,   0.0   ,  3.15    },
	Math::dvec3{ -0.45  ,  -0.8   ,  3.15    }, Math::dvec3{ -0.8   ,  -0.45  ,  3.15    },
	Math::dvec3{ -0.8   ,   0.0   ,  3.15    }, Math::dvec3{ -0.112 ,  -0.2   ,  2.7     },
	Math::dvec3{ -0.2   ,  -0.112 ,  2.7     }, Math::dvec3{ -0.2   ,   0.0   ,  2.7     },
	Math::dvec3{  0.0   ,   0.002 ,  3.15    }, Math::dvec3{ -0.8   ,   0.45  ,  3.15    },
	Math::dvec3{ -0.45  ,   0.8   ,  3.15    }, Math::dvec3{  0.0   ,   0.8   ,  3.15    },
	Math::dvec3{ -0.2   ,   0.112 ,  2.7     }, Math::dvec3{ -0.112 ,   0.2   ,  2.7     },
	Math::dvec3{  0.0   ,   0.2   ,  2.7     }, Math::dvec3{  0.45  ,   0.8   ,  3.15    },
	Math::dvec3{  0.8   ,   0.45  ,  3.15    }, Math::dvec3{  0.112 ,   0.2   ,  2.7     },
	Math::dvec3{  0.2   ,   0.112 ,  2.7     }, Math::dvec3{  0.4   ,   0.0   ,  2.55    },
	Math::dvec3{  0.4   ,  -0.224 ,  2.55    }, Math::dvec3{  0.224 ,  -0.4   ,  2.55    },
	Math::dvec3{  0.0   ,  -0.4   ,  2.55    }, Math::dvec3{  1.3   ,   0.0   ,  2.55    },
	Math::dvec3{  1.3   ,  -0.728 ,  2.55    }, Math::dvec3{  0.728 ,  -1.3   ,  2.55    },
	Math::dvec3{  0.0   ,  -1.3   ,  2.55    }, Math::dvec3{  1.3   ,   0.0   ,  2.4     },
	Math::dvec3{  1.3   ,  -0.728 ,  2.4     }, Math::dvec3{  0.728 ,  -1.3   ,  2.4     },
	Math::dvec3{  0.0   ,  -1.3   ,  2.4     }, Math::dvec3{ -0.224 ,  -0.4   ,  2.55    },
	Math::dvec3{ -0.4   ,  -0.224 ,  2.55    }, Math::dvec3{ -0.4   ,   0.0   ,  2.55    },
	Math::dvec3{ -0.728 ,  -1.3   ,  2.55    }, Math::dvec3{ -1.3   ,  -0.728 ,  2.55    },
	Math::dvec3{ -1.3   ,   0.0   ,  2.55    }, Math::dvec3{ -0.728 ,  -1.3   ,  2.4     },
	Math::dvec3{ -1.3   ,  -0.728 ,  2.4     }, Math::dvec3{ -1.3   ,   0.0   ,  2.4     },
	Math::dvec3{ -0.4   ,   0.224 ,  2.55    }, Math::dvec3{ -0.224 ,   0.4   ,  2.55    },
	Math::dvec3{  0.0   ,   0.4   ,  2.55    }, Math::dvec3{ -1.3   ,   0.728 ,  2.55    },
	Math::dvec3{ -0.728 ,   1.3   ,  2.55    }, Math::dvec3{  0.0   ,   1.3   ,  2.55    },
	Math::dvec3{ -1.3   ,   0.728 ,  2.4     }, Math::dvec3{ -0.728 ,   1.3   ,  2.4     },
	Math::dvec3{  0.0   ,   1.3   ,  2.4     }, Math::dvec3{  0.224 ,   0.4   ,  2.55    },
	Math::dvec3{  0.4   ,   0.224 ,  2.55    }, Math::dvec3{  0.728 ,   1.3   ,  2.55    },
	Math::dvec3{  1.3   ,   0.728 ,  2.55    }, Math::dvec3{  0.728 ,   1.3   ,  2.4     },
	Math::dvec3{  1.3   ,   0.728 ,  2.4     }
};

// Teapot patch indices (28 Bézier patches of order 3)
constexpr int TEAPOT_NUM_PATCHES = 28;
constexpr int BEZIER_ORDER = 3;

constexpr std::array<std::array<std::array<uint16_t, 4>, 4>, TEAPOT_NUM_PATCHES> TEAPOT_PATCHES = {{
	// Rim
	{{{   0,   1,   2,   3 }, {   4,   5,   6,   7 }, {   8,   9,  10,  11 }, {  12,  13,  14,  15 }}},
	{{{   3,  16,  17,  18 }, {   7,  19,  20,  21 }, {  11,  22,  23,  24 }, {  15,  25,  26,  27 }}},
	{{{  18,  28,  29,  30 }, {  21,  31,  32,  33 }, {  24,  34,  35,  36 }, {  27,  37,  38,  39 }}},
	{{{  30,  40,  41,   0 }, {  33,  42,  43,   4 }, {  36,  44,  45,   8 }, {  39,  46,  47,  12 }}},
	// Body
	{{{  12,  13,  14,  15 }, {  48,  49,  50,  51 }, {  52,  53,  54,  55 }, {  56,  57,  58,  59 }}},
	{{{  15,  25,  26,  27 }, {  51,  60,  61,  62 }, {  55,  63,  64,  65 }, {  59,  66,  67,  68 }}},
	{{{  27,  37,  38,  39 }, {  62,  69,  70,  71 }, {  65,  72,  73,  74 }, {  68,  75,  76,  77 }}},
	{{{  39,  46,  47,  12 }, {  71,  78,  79,  48 }, {  74,  80,  81,  52 }, {  77,  82,  83,  56 }}},
	{{{  56,  57,  58,  59 }, {  84,  85,  86,  87 }, {  88,  89,  90,  91 }, {  92,  93,  94,  95 }}},
	{{{  59,  66,  67,  68 }, {  87,  96,  97,  98 }, {  91,  99, 100, 101 }, {  95, 102, 103, 104 }}},
	{{{  68,  75,  76,  77 }, {  98, 105, 106, 107 }, { 101, 108, 109, 110 }, { 104, 111, 112, 113 }}},
	{{{  77,  82,  83,  56 }, { 107, 114, 115,  84 }, { 110, 116, 117,  88 }, { 113, 118, 119,  92 }}},
	// Handle
	{{{ 120, 121, 122, 123 }, { 124, 125, 126, 127 }, { 128, 129, 130, 131 }, { 132, 133, 134, 135 }}},
	{{{ 123, 136, 137, 120 }, { 127, 138, 139, 124 }, { 131, 140, 141, 128 }, { 135, 142, 143, 132 }}},
	{{{ 132, 133, 134, 135 }, { 144, 145, 146, 147 }, { 148, 149, 150, 151 }, {  68, 152, 153, 154 }}},
	{{{ 135, 142, 143, 132 }, { 147, 155, 156, 144 }, { 151, 157, 158, 148 }, { 154, 159, 160,  68 }}},
	// Spout
	{{{ 161, 162, 163, 164 }, { 165, 166, 167, 168 }, { 169, 170, 171, 172 }, { 173, 174, 175, 176 }}},
	{{{ 164, 177, 178, 161 }, { 168, 179, 180, 165 }, { 172, 181, 182, 169 }, { 176, 183, 184, 173 }}},
	{{{ 173, 174, 175, 176 }, { 185, 186, 187, 188 }, { 189, 190, 191, 192 }, { 193, 194, 195, 196 }}},
	{{{ 176, 183, 184, 173 }, { 188, 197, 198, 185 }, { 192, 199, 200, 189 }, { 196, 201, 202, 193 }}},
	// Lid
	{{{ 203, 203, 203, 203 }, { 206, 207, 208, 209 }, { 210, 210, 210, 210 }, { 211, 212, 213, 214 }}},
	{{{ 203, 203, 203, 203 }, { 209, 216, 217, 218 }, { 210, 210, 210, 210 }, { 214, 219, 220, 221 }}},
	{{{ 203, 203, 203, 203 }, { 218, 223, 224, 225 }, { 210, 210, 210, 210 }, { 221, 226, 227, 228 }}},
	{{{ 203, 203, 203, 203 }, { 225, 229, 230, 206 }, { 210, 210, 210, 210 }, { 228, 231, 232, 211 }}},
	{{{ 211, 212, 213, 214 }, { 233, 234, 235, 236 }, { 237, 238, 239, 240 }, { 241, 242, 243, 244 }}},
	{{{ 214, 219, 220, 221 }, { 236, 245, 246, 247 }, { 240, 248, 249, 250 }, { 244, 251, 252, 253 }}},
	{{{ 221, 226, 227, 228 }, { 247, 254, 255, 256 }, { 250, 257, 258, 259 }, { 253, 260, 261, 262 }}},
	{{{ 228, 231, 232, 211 }, { 256, 263, 264, 233 }, { 259, 265, 266, 237 }, { 262, 267, 268, 241 }}}
}};

// Bezier curve evaluation
class BezierPatch {
public:
	// Compute binomial coefficient C(n, k) = n! / (k! * (n-k)!)
	static constexpr double binomialCoeff( int n, int k ) noexcept {
		if( k > n || k < 0 ) return 0.0;
		if( k == 0 || k == n ) return 1.0;
		
		double result = 1.0;
		for( int i = 1; i <= k; ++i ) {
			result *= static_cast<double>( n - (k - i) ) / static_cast<double>( i );
		}
		return result;
	}

	// Bernstein polynomial: B(i,n,t) = C(n,i) * t^i * (1-t)^(n-i)
	static double bernstein( int i, int n, double t ) noexcept {
		return binomialCoeff( n, i ) * std::pow( t, i ) * std::pow( 1.0 - t, n - i );
	}

	// Evaluate Bezier surface at parametric coordinates (u, v)
	static Math::dvec3 evaluate(
		const std::array<std::array<Math::dvec3, 4>, 4>& controlPoints,
		double u,
		double v ) noexcept
	{
		Math::dvec3 result{ 0.0, 0.0, 0.0 };
		
		for( int i = 0; i <= BEZIER_ORDER; ++i ) {
			const double b_i = bernstein( i, BEZIER_ORDER, u );
			
			for( int j = 0; j <= BEZIER_ORDER; ++j ) {
				const double b_j = bernstein( j, BEZIER_ORDER, v );
				result += controlPoints[i][j] * (b_i * b_j);
			}
		}
		
		return result;
	}

	// Get control points for a specific patch
	static std::array<std::array<Math::dvec3, 4>, 4> getControlPoints( int patchIndex ) {
		std::array<std::array<Math::dvec3, 4>, 4> points;
		
		for( int i = 0; i <= BEZIER_ORDER; ++i ) {
			for( int j = 0; j <= BEZIER_ORDER; ++j ) {
				points[i][j] = TEAPOT_CONTROL_POINTS[ TEAPOT_PATCHES[patchIndex][i][j] ];
			}
		}
		
		return points;
	}
};

} // anonymous namespace

// ------------------------------------------------------------------------------------------------
MeshData3D GeometryFactory3D::createTeapot( int resU, int resV )
{
	// Validate resolution
	resU = Math::clamp( resU, 2, 64 );
	resV = Math::clamp( resV, 2, 64 );

	MeshData3D teapot;
	teapot.vertices.reserve( TEAPOT_NUM_PATCHES * resU * resV );
	teapot.indices.reserve( TEAPOT_NUM_PATCHES * (resU - 1) * (resV - 1) * 6 );

	// Generate mesh for each Bézier patch
	for( int patchIdx = 0; patchIdx < TEAPOT_NUM_PATCHES; ++patchIdx ) {
		const auto controlPoints = BezierPatch::getControlPoints( patchIdx );
		const int baseVertex = static_cast<int>( teapot.vertices.size() );

		// Generate vertices for this patch
		for( int ru = 0; ru < resU; ++ru ) {
			const double u = static_cast<double>( ru ) / static_cast<double>( resU - 1 );
			
			for( int rv = 0; rv < resV; ++rv ) {
				const double v = static_cast<double>( rv ) / static_cast<double>( resV - 1 );
				
				// Evaluate Bézier surface
				const Math::dvec3 vertex = BezierPatch::evaluate( controlPoints, u, v );
				teapot.vertices.push_back( vertex );
			}
		}

		// Generate indices (two triangles per quad)
		for( int ru = 0; ru < resU - 1; ++ru ) {
			for( int rv = 0; rv < resV - 1; ++rv ) {
				const int i0 = baseVertex + ru * resV + rv;
				const int i1 = baseVertex + ru * resV + (rv + 1);
				const int i2 = baseVertex + (ru + 1) * resV + (rv + 1);
				const int i3 = baseVertex + (ru + 1) * resV + rv;

				// First triangle (CCW winding)
				teapot.indices.push_back( i0 );
				teapot.indices.push_back( i1 );
				teapot.indices.push_back( i2 );

				// Second triangle (CCW winding)
				teapot.indices.push_back( i0 );
				teapot.indices.push_back( i2 );
				teapot.indices.push_back( i3 );
			}
		}
	}

	// Compute normals (average of adjacent face normals)
	teapot.normals.resize( teapot.vertices.size(), Math::dvec3{ 0.0, 0.0, 0.0 } );

	for( size_t i = 0; i < teapot.indices.size(); i += 3 ) {
		const auto& v0 = teapot.vertices[ teapot.indices[i + 0] ];
		const auto& v1 = teapot.vertices[ teapot.indices[i + 1] ];
		const auto& v2 = teapot.vertices[ teapot.indices[i + 2] ];

		// Compute face normal
		const Math::dvec3 edge1 = v1 - v0;
		const Math::dvec3 edge2 = v2 - v0;
		const Math::dvec3 faceNormal = Math::normalize( Math::cross( edge1, edge2 ) );

		// Accumulate to vertex normals
		teapot.normals[ teapot.indices[i + 0] ] += faceNormal;
		teapot.normals[ teapot.indices[i + 1] ] += faceNormal;
		teapot.normals[ teapot.indices[i + 2] ] += faceNormal;
	}

	// Normalize all vertex normals
	for( auto& normal : teapot.normals ) {
		normal = Math::normalize( normal );
	}

	return teapot;
}