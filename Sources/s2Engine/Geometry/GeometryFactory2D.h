// GeometryFactory2D.h
// 
#ifndef GEOMETRY_FACTORY2D_ONCE
#define GEOMETRY_FACTORY2D_ONCE

#include "s2Engine_API.h"

#include "MeshData.h"
#include "Math/Math.h"

#include <vector>

namespace s2 {

class S2ENGINE_API GeometryFactory2D
{
public:
	static MeshData2D circle( const Math::dvec2& center, double radius, int slices = 32 );
	static MeshData2D rectangle( const Math::dvec2& min, const Math::dvec2& max );
	static MeshData2D ellipse( const Math::dvec2& center, double radiusX, double radiusY, int slices = 32 );
	static MeshData2D polygon( const std::vector<Math::dvec2>& points );

private:
	GeometryFactory2D() = delete;
};

}

#endif