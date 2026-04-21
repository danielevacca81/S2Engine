// GeometryFactory.h
// 
#ifndef GEOMETRY_GEOMETRYFACTORY3D_ONCE
#define GEOMETRY_GEOMETRYFACTORY3D_ONCE

#include "s2Engine_API.h"

#include "MeshData.h"
#include "Math/Math.h"

#include <vector>

namespace s2 {

class S2ENGINE_API GeometryFactory3D
{
public:
	static MeshData3D createTorus( double innerRadius, double outerRadius, int sidesCount, int ringsCount );
	static MeshData3D createCylinder( const Math::dvec3& startPoint, const Math::dvec3& endPoint, double radius, bool capStart, bool capEnd, int slices );
	static MeshData3D createSphere( const Math::dvec3& center, double radius, int slices );
	static MeshData3D createCone( const Math::dvec3& center, const Math::dvec3& tip, double baseRadius, bool cap, int slices );
	static MeshData3D createCube( const Math::dvec3& center, double size );
	static MeshData3D createCapsule( const Math::dvec3& startPoint, const Math::dvec3& endPoint, double radius, int slices, int rings );
	static MeshData3D createTeapot( int resU, int resV );



	// static Mesh teapot( int resU, int resV );

public:
	GeometryFactory3D() = delete;
};

}

#endif