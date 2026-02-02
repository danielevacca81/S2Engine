// GeometryFactory2D.cpp
// 
#include "GeometryFactory2D.h"

using namespace s2;

// ------------------------------------------------------------------------------------------------
MeshData2D GeometryFactory2D::circle( const Math::dvec2& center, double radius, int slices )
{
	if( slices < 3 )
		return {};

	MeshData2D circle;
	const double step = Math::two_pi<double>() / slices;

	// center
	circle.vertices.push_back( center );

	// circumference points
	for( int i = 0; i < slices; ++i )
	{
		const double angle = i * step;
		const Math::dvec2 p = Math::dvec2( Math::cos( angle ), Math::sin( angle ) ) * radius;
		circle.vertices.push_back( center + p );
	}

	// indices
	for( int i = 0; i < slices; ++i )
	{
		circle.indices.push_back( 0 );                      // Center
		circle.indices.push_back( i + 1 );                  // current
		circle.indices.push_back( ( i + 1 ) % slices + 1 ); // next
	}

	return circle;
}

// ------------------------------------------------------------------------------------------------
MeshData2D GeometryFactory2D::rectangle( const Math::dvec2& min, const Math::dvec2& max )
{
	MeshData2D rect;

	// Vertices (CCW winding)
	rect.vertices = 
	{
		Math::dvec2( min.x, min.y ), // Bottom-left
		Math::dvec2( max.x, min.y ), // Bottom-right
		Math::dvec2( max.x, max.y ), // Top-right
		Math::dvec2( min.x, max.y )  // Top-left
	};

	// UV coordinates 
	rect.uvCoords = 
	{
		Math::dvec2( 0.0, 0.0 ),
		Math::dvec2( 1.0, 0.0 ),
		Math::dvec2( 1.0, 1.0 ),
		Math::dvec2( 0.0, 1.0 )
	};

	// Indices (two triangles)
	rect.indices = 
	{
		0, 1, 2,
		0, 2, 3
	};

	return rect;
}

// ------------------------------------------------------------------------------------------------
MeshData2D GeometryFactory2D::ellipse( const Math::dvec2& center, double radiusX, double radiusY, int slices )
{
	if( slices < 3 )
		return {};

	MeshData2D ellipse;
	const double step = Math::two_pi<double>() / slices;

	// Center
	ellipse.vertices.push_back( center );

	// points on the ellipse
	for( int i = 0; i < slices; ++i )
	{
		const double angle = i * step;
		const Math::dvec2 p = Math::dvec2( 
			Math::cos( angle ) * radiusX, 
			Math::sin( angle ) * radiusY 
		);
		ellipse.vertices.push_back( center + p );
	}

	// Indices
	for( int i = 0; i < slices; ++i )
	{
		ellipse.indices.push_back( 0 );
		ellipse.indices.push_back( i + 1 );
		ellipse.indices.push_back( ( i + 1 ) % slices + 1 );
	}

	return ellipse;
}

// ------------------------------------------------------------------------------------------------
MeshData2D GeometryFactory2D::polygon( const std::vector<Math::dvec2>& points )
{
	if( points.size() < 3 )
		return {};

	MeshData2D poly;

	// compute centroid and use it as center for triangulation
	Math::dvec2 center( 0.0 );
	for( const auto& p : points )
		center += p;
	center /= static_cast<double>( points.size() );

	// centroid as first vertex
	poly.vertices.push_back( center );

	// Add polygon points
	for( const auto& p : points )
		poly.vertices.push_back( p );

	// Fan triangulation from center
	const int n = static_cast<int>( points.size() );
	for( int i = 0; i < n; ++i )
	{
		poly.indices.push_back( 0 );
		poly.indices.push_back( i + 1 );
		poly.indices.push_back( ( i + 1 ) % n + 1 );
	}

	return poly;
}