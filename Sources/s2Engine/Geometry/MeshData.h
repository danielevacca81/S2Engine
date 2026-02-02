// MeshData.h
// 
#ifndef GEOMETRY_MESHDATA_ONCE
#define GEOMETRY_MESHDATA_ONCE

#include "s2Engine_API.h"

#include "Math/Math.h"
#include <vector>

namespace s2 {

// ================================================================================================
// Template for 2D-3D meshes data
// ================================================================================================
template<typename VectorType>
struct MeshData
{
	std::vector<VectorType>    vertices;
	std::vector<VectorType>    normals;   // optional for 2D (can be empty)
	std::vector<Math::dvec2>   uvCoords;
	std::vector<uint32_t>      indices;

	// Helper methods
	bool isEmpty() const { return vertices.empty(); }
	void clear() 
	{ 
		vertices.clear(); 
		normals.clear(); 
		uvCoords.clear(); 
		indices.clear(); 
	}
};

// ================================================================================================
// Type aliases
// ================================================================================================

/// @brief Mesh data for 2D geometry
using MeshData2D = MeshData<Math::dvec2>;

/// @brief Mesh data for 3D geometry
using MeshData3D = MeshData<Math::dvec3>;

}

#endif // GEOMETRY_MESHDATA_ONCE