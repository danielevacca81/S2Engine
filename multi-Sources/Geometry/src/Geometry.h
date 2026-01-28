// Mesh.h
// 
#ifndef GEOMETRY_GEOMETRY_ONCE
#define GEOMETRY_GEOMETRY_ONCE

#include "Geometry_API.h"

#include "Math/Math.h"
#include <vector>

namespace Geometry {

struct GEOMETRY_API Geometry
{
	std::vector<Math::dvec3>   vertices;
	std::vector<Math::dvec3>   normals;
	std::vector<Math::dvec2>   uvCoords;
	std::vector<uint32_t>      indices;
};

}

#endif