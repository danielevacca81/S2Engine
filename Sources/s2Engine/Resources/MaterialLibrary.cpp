// MaterialLibrary.cpp
//
#include "MaterialLibrary.h"

#include <algorithm>

using namespace s2::Resources;

static constexpr const char* kDefaultMaterialName = "__default__";

// -------------------------------------------------------------------------------------------------
MaterialLibraryPtr MaterialLibrary::New()
{
	return std::make_shared<MaterialLibrary>();
}

// -------------------------------------------------------------------------------------------------
bool MaterialLibrary::add( MaterialPtr material )
{
	if( !material )
		return false;

	const std::string& name = material->name();

	if( exists( name ) )
		return false;

	_materials[name] = material;
	return true;
}

// -------------------------------------------------------------------------------------------------
bool MaterialLibrary::remove( const std::string& name )
{
	auto it = _materials.find( name );

	if( it == _materials.end() )
		return false;

	_materials.erase( it );
	return true;
}

// -------------------------------------------------------------------------------------------------
void MaterialLibrary::clear()
{
	_materials.clear();
	_textures.clear();
}

// -------------------------------------------------------------------------------------------------
MaterialPtr MaterialLibrary::get( const std::string& name ) const
{
	auto it = _materials.find( name );
	return ( it != _materials.end() ) ? it->second : nullptr;
}

// -------------------------------------------------------------------------------------------------
bool MaterialLibrary::exists( const std::string& name ) const
{
	return _materials.find( name ) != _materials.end();
}

// -------------------------------------------------------------------------------------------------
MaterialPtr MaterialLibrary::create( const std::string& name )
{
	if( exists( name ) )
		return nullptr;

	auto material = Material::New( name );
	_materials[name] = material;

	return material;
}

// -------------------------------------------------------------------------------------------------
MaterialPtr MaterialLibrary::create( const std::string& name, const Color& baseColor )
{
	if( exists( name ) )
		return nullptr;

	auto material = Material::New( name, baseColor );
	_materials[name] = material;

	return material;
}

// -------------------------------------------------------------------------------------------------
MaterialPtr MaterialLibrary::getOrCreateDefault()
{
	auto material = get( kDefaultMaterialName );

	if( !material )
		material = create( kDefaultMaterialName, Color::gray() );

	return material;
}

// -------------------------------------------------------------------------------------------------
MaterialPtr MaterialLibrary::createWireframe( const std::string& name, const Color& color )
{
	auto material = create( name, color );

	if( material )
	{
		material->setRoughness( 1.0f );
		material->setMetallic( 0.0f );
	}

	return material;
}

// -------------------------------------------------------------------------------------------------
MaterialPtr MaterialLibrary::createUnlit( const std::string& name, const Color& color )
{
	auto material = create( name );

	if( material )
	{
		material->setBaseColor( Color::black() );
		material->setEmissiveColor( color );
	}

	return material;
}

// -------------------------------------------------------------------------------------------------
TexturePtr MaterialLibrary::createTexture( const std::string& name,
										   int width,
										   int height,
										   int channels,
										   const uint8_t* data )
{
	if( hasTexture( name ) )
		return nullptr;

	auto texture = Texture::New( name, width, height, channels, data );
	_textures[name] = texture;

	return texture;
}

// -------------------------------------------------------------------------------------------------
TexturePtr MaterialLibrary::createTexture( const std::string& name,
										   const s2::Pixmap<uint8_t>& pixmap )
{
	if( hasTexture( name ) )
		return nullptr;

	auto texture = Texture::New( name, pixmap );
	_textures[name] = texture;

	return texture;
}

// -------------------------------------------------------------------------------------------------
TexturePtr MaterialLibrary::getTexture( const std::string& name ) const
{
	auto it = _textures.find( name );
	return ( it != _textures.end() ) ? it->second : nullptr;
}

// -------------------------------------------------------------------------------------------------
bool MaterialLibrary::hasTexture( const std::string& name ) const
{
	return _textures.find( name ) != _textures.end();
}

// -------------------------------------------------------------------------------------------------
void MaterialLibrary::removeTexture( const std::string& name )
{
	_textures.erase( name );
}

// -------------------------------------------------------------------------------------------------
MaterialPtr MaterialLibrary::loadMaterial( const std::string& name, const std::filesystem::path& filepath )
{
	// Check if already loaded
	if( exists( name ) )
		return get( name );

	// @TODO: Implement material file loading
	// This would typically parse a material definition file (JSON, XML, custom format)
	// and load associated textures, then construct the material
	//
	// Example structure:
	// 1. Parse material file
	// 2. Load referenced textures
	// 3. Set material properties
	// 4. Add to library

	return nullptr; // Placeholder
}

// -------------------------------------------------------------------------------------------------
TexturePtr MaterialLibrary::loadTexture( const std::string& name, const std::filesystem::path& filepath )
{
	// Check if already loaded
	if( hasTexture( name ) )
		return getTexture( name );

	// @TODO: Implement texture file loading
	// Use your preferred image loading library (stb_image, etc.)
	//
	// Example:
	// s2::Pixmap<uint8_t> image = ImageLoader::load( filepath );
	// return createTexture( name, image );

	return nullptr; // Placeholder
}

// -------------------------------------------------------------------------------------------------
std::vector<std::string> MaterialLibrary::materialNames() const
{
	std::vector<std::string> result;
	result.reserve( _materials.size() );

	for( const auto& [name, material] : _materials )
		result.push_back( name );

	std::sort( result.begin(), result.end() );
	return result;
}

// -------------------------------------------------------------------------------------------------
std::vector<std::string> MaterialLibrary::textureNames() const
{
	std::vector<std::string> result;
	result.reserve( _textures.size() );

	for( const auto& [name, texture] : _textures )
		result.push_back( name );

	std::sort( result.begin(), result.end() );
	return result;
}

// -------------------------------------------------------------------------------------------------
void MaterialLibrary::forEachMaterial( const std::function<void( MaterialPtr )>& func ) const
{
	for( const auto& [name, material] : _materials )
		func( material );
}

// -------------------------------------------------------------------------------------------------
void MaterialLibrary::forEachTexture( const std::function<void( TexturePtr )>& func ) const
{
	for( const auto& [name, texture] : _textures )
		func( texture );
}

//// -------------------------------------------------------------------------------------------------
//SkyboxPtr MaterialLibrary::createSkybox( const std::string& name )
//{
//	if( hasSkybox( name ) )
//		return nullptr;
//
//	auto skybox = Skybox::New( name );
//	_skyboxes[name] = skybox;
//	return skybox;
//}
//
//// -------------------------------------------------------------------------------------------------
//SkyboxPtr MaterialLibrary::createSkyboxSolidColor( const std::string& name, const Color& color )
//{
//	if( hasSkybox( name ) )
//		return nullptr;
//
//	auto skybox = Skybox::NewSolidColor( name, color );
//	_skyboxes[name] = skybox;
//	return skybox;
//}
//
//// -------------------------------------------------------------------------------------------------
//SkyboxPtr MaterialLibrary::createSkyboxGradient( const std::string& name,
//                                                 const Color& topColor,
//                                                 const Color& bottomColor )
//{
//	if( hasSkybox( name ) )
//		return nullptr;
//
//	auto skybox = Skybox::NewGradient( name, topColor, bottomColor );
//	_skyboxes[name] = skybox;
//	return skybox;
//}
//
//// -------------------------------------------------------------------------------------------------
//SkyboxPtr MaterialLibrary::createSkyboxCubemap( const std::string& name, CubemapTexturePtr cubemap )
//{
//	if( hasSkybox( name ) )
//		return nullptr;
//
//	auto skybox = Skybox::NewCubemap( name, cubemap );
//	_skyboxes[name] = skybox;
//	return skybox;
//}
//
//// -------------------------------------------------------------------------------------------------
//SkyboxPtr MaterialLibrary::getSkybox( const std::string& name ) const
//{
//	auto it = _skyboxes.find( name );
//	return ( it != _skyboxes.end() ) ? it->second : nullptr;
//}
//
//// -------------------------------------------------------------------------------------------------
//bool MaterialLibrary::hasSkybox( const std::string& name ) const
//{
//	return _skyboxes.find( name ) != _skyboxes.end();
//}
//
//// -------------------------------------------------------------------------------------------------
//void MaterialLibrary::removeSkybox( const std::string& name )
//{
//	_skyboxes.erase( name );
//}

// -------------------------------------------------------------------------------------------------
CubemapTexturePtr MaterialLibrary::createCubemap( const std::string& name, 
                                                  int faceSize, 
                                                  int channels )
{
	if( hasCubemap( name ) )
		return nullptr;

	auto cubemap = CubemapTexture::New( name, faceSize, channels );
	_cubemaps[name] = cubemap;
	return cubemap;
}

// -------------------------------------------------------------------------------------------------
CubemapTexturePtr MaterialLibrary::createCubemap( const std::string& name,
                                                  const std::array<s2::Pixmap<uint8_t>, 6>& faces )
{
	if( hasCubemap( name ) )
		return nullptr;

	auto cubemap = CubemapTexture::New( name, faces );
	_cubemaps[name] = cubemap;
	return cubemap;
}

// -------------------------------------------------------------------------------------------------
CubemapTexturePtr MaterialLibrary::loadCubemap( const std::string& name,
                                                const std::array<std::string, 6>& facePaths )
{
	if( hasCubemap( name ) )
		return getCubemap( name );

	// @TODO: Implement cubemap loading from files
	// Load 6 faces from file paths in order: +X, -X, +Y, -Y, +Z, -Z
	//
	// std::array<s2::Pixmap<uint8_t>, 6> faces;
	// for( int i = 0; i < 6; ++i )
	//     faces[i] = ImageLoader::load( facePaths[i] );
	// return createCubemap( name, faces );

	return nullptr; // Placeholder
}

// -------------------------------------------------------------------------------------------------
CubemapTexturePtr MaterialLibrary::getCubemap( const std::string& name ) const
{
	auto it = _cubemaps.find( name );
	return ( it != _cubemaps.end() ) ? it->second : nullptr;
}

// -------------------------------------------------------------------------------------------------
bool MaterialLibrary::hasCubemap( const std::string& name ) const
{
	return _cubemaps.find( name ) != _cubemaps.end();
}