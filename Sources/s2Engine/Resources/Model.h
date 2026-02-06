// Model.h
//
#ifndef S2_RESOURCES_MODEL_H
#define S2_RESOURCES_MODEL_H

#include "s2Engine_API.h"

#include "Material.h"
#include "Geometry/MeshData.h"
#include "Math/Box.h"

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace s2 {
namespace Resources {

/************************************************************************************************/
/*                                          SubMesh                                             */
/************************************************************************************************/

/**
 * SubMesh represents a portion of a model with its own material.
 * Multiple submeshes allow a single model to have different materials on different parts.
 */
struct S2ENGINE_API SubMesh
{
	std::string    name;
	MaterialPtr    material;
	
	// Index range in the main mesh's index buffer
	uint32_t       indexStart  = 0;
	uint32_t       indexCount  = 0;
	
	// Bounding box for this submesh
	Math::dbox3    boundingBox;

	SubMesh() = default;
	SubMesh( const std::string& n, MaterialPtr mat, uint32_t start, uint32_t count )
		: name( n )
		, material( mat )
		, indexStart( start )
		, indexCount( count )
	{}
};

/************************************************************************************************/
/*                                           Model                                              */
/************************************************************************************************/

class Model;
typedef std::shared_ptr<Model> ModelPtr;

/**
 * Model class encapsulates 3D geometry and associated materials.
 * Supports:
 * - Single material models
 * - Multi-material models (via submeshes)
 * - Bounding box calculation
 * - Material management
 */
class S2ENGINE_API Model
{
public:
	static ModelPtr New( const std::string& name );
	static ModelPtr New( const std::string& name, const MeshData3D& meshData );
	static ModelPtr New( const std::string& name, const MeshData3D& meshData, MaterialPtr material );

public:
	Model( const std::string& name );
	Model( const std::string& name, const MeshData3D& meshData );
	Model( const std::string& name, const MeshData3D& meshData, MaterialPtr material );
	~Model() = default;

	// Identification
	std::string name() const { return _name; }
	void setName( const std::string& name ) { _name = name; }

	// Mesh data access
	const MeshData3D& meshData() const { return _meshData; }
	MeshData3D& meshData() { return _meshData; }
	void setMeshData( const MeshData3D& meshData );

	// Single material mode (for simple models)
	MaterialPtr material() const { return _material; }
	void setMaterial( MaterialPtr material );

	// Multi-material mode (submeshes)
	size_t submeshCount() const { return _submeshes.size(); }
	const std::vector<SubMesh>& submeshes() const { return _submeshes; }
	
	void addSubmesh( const SubMesh& submesh );
	void addSubmesh( const std::string& name, MaterialPtr material, uint32_t indexStart, uint32_t indexCount );
	void clearSubmeshes();
	
	SubMesh* getSubmesh( size_t index );
	const SubMesh* getSubmesh( size_t index ) const;
	SubMesh* findSubmesh( const std::string& name );
	const SubMesh* findSubmesh( const std::string& name ) const;

	// Material collection (all unique materials used by this model)
	std::vector<MaterialPtr> getMaterials() const;
	size_t materialCount() const;

	// Bounding box
	Math::dbox3 boundingBox() const { return _boundingBox; }
	void setBoundingBox( const Math::dbox3& box ) { _boundingBox = box; }
	void computeBoundingBox();

	// Validation
	bool isValid() const;
	bool hasMultipleMaterials() const { return !_submeshes.empty(); }

	// Statistics
	size_t vertexCount() const { return _meshData.vertices.size(); }
	size_t triangleCount() const { return _meshData.indices.size() / 3; }

private:
	void updateBoundingBox();

private:
	std::string  _name;
	MeshData3D   _meshData;

	// Single material (simple models)
	MaterialPtr  _material;

	// Multi-material support (complex models)
	std::vector<SubMesh> _submeshes;

	// Spatial information
	Math::dbox3  _boundingBox;
};

}
}

#endif // !S2_RESOURCES_MODEL_H