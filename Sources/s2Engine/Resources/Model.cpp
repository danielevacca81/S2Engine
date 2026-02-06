// Model.cpp
//
#include "Model.h"

#include <algorithm>
#include <limits>

using namespace s2::Resources;

// -------------------------------------------------------------------------------------------------
ModelPtr Model::New( const std::string& name )
{
	return std::make_shared<Model>( name );
}

// -------------------------------------------------------------------------------------------------
ModelPtr Model::New( const std::string& name, const MeshData3D& meshData )
{
	return std::make_shared<Model>( name, meshData );
}

// -------------------------------------------------------------------------------------------------
ModelPtr Model::New( const std::string& name, const MeshData3D& meshData, MaterialPtr material )
{
	return std::make_shared<Model>( name, meshData, material );
}

// -------------------------------------------------------------------------------------------------
Model::Model( const std::string& name )
	: _name( name )
{
}

// -------------------------------------------------------------------------------------------------
Model::Model( const std::string& name, const MeshData3D& meshData )
	: _name( name )
	, _meshData( meshData )
{
	computeBoundingBox();
}

// -------------------------------------------------------------------------------------------------
Model::Model( const std::string& name, const MeshData3D& meshData, MaterialPtr material )
	: _name( name )
	, _meshData( meshData )
	, _material( material )
{
	computeBoundingBox();
}

// -------------------------------------------------------------------------------------------------
void Model::setMeshData( const MeshData3D& meshData )
{
	_meshData = meshData;
	computeBoundingBox();
}

// -------------------------------------------------------------------------------------------------
void Model::setMaterial( MaterialPtr material )
{
	_material = material;

	// If setting a single material, clear submeshes
	if( material )
		_submeshes.clear();
}

// -------------------------------------------------------------------------------------------------
void Model::addSubmesh( const SubMesh& submesh )
{
	_submeshes.push_back( submesh );

	// Clear single material when using submeshes
	_material = nullptr;
}

// -------------------------------------------------------------------------------------------------
void Model::addSubmesh( const std::string& name, MaterialPtr material, uint32_t indexStart, uint32_t indexCount )
{
	SubMesh submesh( name, material, indexStart, indexCount );
	addSubmesh( submesh );
}

// -------------------------------------------------------------------------------------------------
void Model::clearSubmeshes()
{
	_submeshes.clear();
}

// -------------------------------------------------------------------------------------------------
SubMesh* Model::getSubmesh( size_t index )
{
	if( index >= _submeshes.size() )
		return nullptr;

	return &_submeshes[index];
}

// -------------------------------------------------------------------------------------------------
const SubMesh* Model::getSubmesh( size_t index ) const
{
	if( index >= _submeshes.size() )
		return nullptr;

	return &_submeshes[index];
}

// -------------------------------------------------------------------------------------------------
SubMesh* Model::findSubmesh( const std::string& name )
{
	auto it = std::find_if( _submeshes.begin(), _submeshes.end(),
							[&name] ( const SubMesh& sm ) { return sm.name == name; } );

	return ( it != _submeshes.end() ) ? &( *it ) : nullptr;
}

// -------------------------------------------------------------------------------------------------
const SubMesh* Model::findSubmesh( const std::string& name ) const
{
	auto it = std::find_if( _submeshes.begin(), _submeshes.end(),
							[&name] ( const SubMesh& sm ) { return sm.name == name; } );

	return ( it != _submeshes.end() ) ? &( *it ) : nullptr;
}

// -------------------------------------------------------------------------------------------------
std::vector<MaterialPtr> Model::getMaterials() const
{
	std::vector<MaterialPtr> materials;

	// Add single material if present
	if( _material )
	{
		materials.push_back( _material );
		return materials;
	}

	// Collect materials from submeshes (avoid duplicates)
	for( const auto& submesh : _submeshes )
	{
		if( submesh.material )
		{
			// Check if already added
			auto it = std::find( materials.begin(), materials.end(), submesh.material );
			if( it == materials.end() )
				materials.push_back( submesh.material );
		}
	}

	return materials;
}

// -------------------------------------------------------------------------------------------------
size_t Model::materialCount() const
{
	if( _material )
		return 1;

	// Count unique materials in submeshes
	std::vector<MaterialPtr> uniqueMaterials;
	for( const auto& submesh : _submeshes )
	{
		if( submesh.material )
		{
			auto it = std::find( uniqueMaterials.begin(), uniqueMaterials.end(), submesh.material );
			if( it == uniqueMaterials.end() )
				uniqueMaterials.push_back( submesh.material );
		}
	}

	return uniqueMaterials.size();
}

// -------------------------------------------------------------------------------------------------
void Model::computeBoundingBox()
{
	if( _meshData.vertices.empty() )
	{
		_boundingBox = Math::dbox3();
		return;
	}

	Math::dvec3 min( std::numeric_limits<double>::max() );
	Math::dvec3 max( std::numeric_limits<double>::lowest() );

	for( const auto& vertex : _meshData.vertices )
	{
		min.x = std::min( min.x, vertex.x );
		min.y = std::min( min.y, vertex.y );
		min.z = std::min( min.z, vertex.z );

		max.x = std::max( max.x, vertex.x );
		max.y = std::max( max.y, vertex.y );
		max.z = std::max( max.z, vertex.z );
	}

	_boundingBox = Math::dbox3( min, max );

	// Update submesh bounding boxes if needed
	for( auto& submesh : _submeshes )
	{
		// Compute per-submesh bounding box
		if( submesh.indexStart + submesh.indexCount <= _meshData.indices.size() )
		{
			Math::dvec3 subMin( std::numeric_limits<double>::max() );
			Math::dvec3 subMax( std::numeric_limits<double>::lowest() );

			for( uint32_t i = submesh.indexStart; i < submesh.indexStart + submesh.indexCount; ++i )
			{
				const auto& vertex = _meshData.vertices[_meshData.indices[i]];

				subMin.x = std::min( subMin.x, vertex.x );
				subMin.y = std::min( subMin.y, vertex.y );
				subMin.z = std::min( subMin.z, vertex.z );

				subMax.x = std::max( subMax.x, vertex.x );
				subMax.y = std::max( subMax.y, vertex.y );
				subMax.z = std::max( subMax.z, vertex.z );
			}

			submesh.boundingBox = Math::dbox3( subMin, subMax );
		}
	}
}

// -------------------------------------------------------------------------------------------------
bool Model::isValid() const
{
	// Must have vertices
	if( _meshData.vertices.empty() )
		return false;

	// If using submeshes, validate them
	if( !_submeshes.empty() )
	{
		for( const auto& submesh : _submeshes )
		{
			// Check index range
			if( submesh.indexStart + submesh.indexCount > _meshData.indices.size() )
				return false;

			// Each submesh should have a material
			if( !submesh.material )
				return false;
		}
	}

	return true;
}