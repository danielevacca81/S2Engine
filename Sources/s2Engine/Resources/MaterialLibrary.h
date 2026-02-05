// MaterialLibrary.h
//
#ifndef S2_RESOURCES_MATERIAL_LIBRARY_H
#define S2_RESOURCES_MATERIAL_LIBRARY_H

#include "s2Engine_API.h"

#include "Material.h"
#include "CubemapTexture.h"

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <functional>

namespace s2 {
namespace Resources {

class MaterialLibrary;
typedef std::shared_ptr<MaterialLibrary> MaterialLibraryPtr;

/**
 * MaterialLibrary manages a collection of materials and their associated textures.
 * Provides caching, creation, and retrieval of materials and textures.
 */
class S2ENGINE_API MaterialLibrary
{
public:
	static MaterialLibraryPtr New();

public:
	MaterialLibrary() = default;
	~MaterialLibrary() = default;

	// Material management
	bool add( MaterialPtr material );
	bool remove( const std::string& name );
	void clear();

	MaterialPtr get( const std::string& name ) const;
	bool exists( const std::string& name ) const;

	// Material creation
	MaterialPtr create( const std::string& name );
	MaterialPtr create( const std::string& name, const Color& baseColor );

	// Predefined materials
	MaterialPtr getOrCreateDefault();
	MaterialPtr createWireframe( const std::string& name, const Color& color = Color::white() );
	MaterialPtr createUnlit( const std::string& name, const Color& color = Color::white() );

	// Texture management (integrated)
	TexturePtr createTexture( const std::string& name,
							  int width,
							  int height,
							  int channels = 4,
							  const uint8_t* data = nullptr );

	TexturePtr createTexture( const std::string& name,
							  const s2::Pixmap<uint8_t>& pixmap );

	TexturePtr getTexture( const std::string& name ) const;
	bool hasTexture( const std::string& name ) const;
	void removeTexture( const std::string& name );

	//// Skybox management
	//SkyboxPtr createSkybox( const std::string& name );
	//SkyboxPtr createSkyboxSolidColor( const std::string& name, const Color& color );
	//SkyboxPtr createSkyboxGradient( const std::string& name,
	//                                const Color& topColor,
	//                                const Color& bottomColor );
	//SkyboxPtr createSkyboxCubemap( const std::string& name, CubemapTexturePtr cubemap );

	//SkyboxPtr getSkybox( const std::string& name ) const;
	bool hasSkybox( const std::string& name ) const;
	void removeSkybox( const std::string& name );

	// Cubemap management
	CubemapTexturePtr createCubemap( const std::string& name, int faceSize, int channels = 4 );
	CubemapTexturePtr createCubemap( const std::string& name,
	                                 const std::array<s2::Pixmap<uint8_t>, 6>& faces );
	CubemapTexturePtr loadCubemap( const std::string& name,
	                               const std::array<std::string, 6>& facePaths );

	CubemapTexturePtr getCubemap( const std::string& name ) const;
	bool hasCubemap( const std::string& name ) const;

	// File loading (to be implemented)
	MaterialPtr loadMaterial( const std::string& name, const std::filesystem::path & filepath );
	TexturePtr loadTexture( const std::string& name, const std::filesystem::path& filepath );

	// Statistics
	size_t materialCount() const { return _materials.size(); }
	size_t textureCount() const { return _textures.size(); }
	//size_t skyboxCount() const { return _skyboxes.size(); }
	size_t cubemapCount() const { return _cubemaps.size(); }

	std::vector<std::string> materialNames() const;
	std::vector<std::string> textureNames() const;

	// Iteration (for advanced usage)
	void forEachMaterial( const std::function<void( MaterialPtr )>& func ) const;
	void forEachTexture( const std::function<void( TexturePtr )>& func ) const;

private:
	std::unordered_map<std::string, MaterialPtr> _materials;
	std::unordered_map<std::string, TexturePtr> _textures;
	//std::unordered_map<std::string, SkyboxPtr> _skyboxes;
	std::unordered_map<std::string, CubemapTexturePtr> _cubemaps;
};

}
}

#endif // !S2_RESOURCES_MATERIAL_LIBRARY_H