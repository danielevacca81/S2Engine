// Mesh.h
// 
#ifndef GEOMETRY_GEOMETRY_ONCE
#define GEOMETRY_GEOMETRY_ONCE

#include "s2Engine_API.h"

#include "Math/Math.h"
#include <vector>

namespace s2 {
namespace Geometry {

struct S2ENGINE_API Geometry
{
	std::vector<Math::dvec3>   vertices;
	std::vector<Math::dvec3>   normals;
	std::vector<Math::dvec2>   uvCoords;
	std::vector<uint32_t>      indices;
};

}

}

#endif