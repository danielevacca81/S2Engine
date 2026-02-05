// Material.cpp
//
#include "Material.h"

#include "Math/Math.h"

using namespace Renderer;

// -------------------------------------------------------------------------------------------------
MaterialPtr Material::New( const std::string& name )
{
	return std::make_shared<Material>( name );
}

// -------------------------------------------------------------------------------------------------
MaterialPtr Material::New( const std::string& name, const Color& baseColor )
{
	return std::make_shared<Material>( name, baseColor );
}

// -------------------------------------------------------------------------------------------------
Material::Material( const std::string& name )
	: _name( name )
{
}

// -------------------------------------------------------------------------------------------------
Material::Material( const std::string& name, const Color& baseColor )
	: _name( name )
	, _baseColor( baseColor )
{
}

// -------------------------------------------------------------------------------------------------
void Material::setTexture( TextureType type, TexturePtr texture )
{
	if( texture )
		_textures[type] = texture;
	else
		_textures.erase( type );
}

// -------------------------------------------------------------------------------------------------
TexturePtr Material::getTexture( TextureType type ) const
{
	auto it = _textures.find( type );
	return ( it != _textures.end() ) ? it->second : nullptr;
}

// -------------------------------------------------------------------------------------------------
bool Material::hasTexture( TextureType type ) const
{
	return _textures.find( type ) != _textures.end();
}

// -------------------------------------------------------------------------------------------------
void Material::removeTexture( TextureType type )
{
	_textures.erase( type );
}

// -------------------------------------------------------------------------------------------------
void Material::clearTextures()
{
	_textures.clear();
}

// -------------------------------------------------------------------------------------------------
std::vector<Material::TextureType> Material::getUsedTextureSlots() const
{
	std::vector<TextureType> result;
	result.reserve( _textures.size() );

	for( const auto& [type, texture] : _textures )
		result.push_back( type );

	return result;
}

// -------------------------------------------------------------------------------------------------
bool Material::operator==( const Material& other ) const
{
	if( _name != other._name )
		return false;

	if( _baseColor != other._baseColor ||
		_emissiveColor != other._emissiveColor )
		return false;

	if( !Math::equals( _roughness, other._roughness ) ||
		!Math::equals( _metallic, other._metallic ) ||
		!Math::equals( _opacity, other._opacity ) )
		return false;

	if( _textures.size() != other._textures.size() )
		return false;

	for( const auto& [type, texture] : _textures )
	{
		auto it = other._textures.find( type );
		if( it == other._textures.end() || it->second != texture )
			return false;
	}

	return true;
}

// -------------------------------------------------------------------------------------------------
const char* Renderer::textureTypeToString( Material::TextureType type )
{
	switch( type )
	{
	case Material::TextureType::Diffuse:           return "Diffuse";
	case Material::TextureType::Normal:            return "Normal";
	case Material::TextureType::Specular:          return "Specular";
	case Material::TextureType::Roughness:         return "Roughness";
	case Material::TextureType::Metallic:          return "Metallic";
	case Material::TextureType::AmbientOcclusion:  return "AmbientOcclusion";
	case Material::TextureType::Emissive:          return "Emissive";
	case Material::TextureType::Opacity:           return "Opacity";
	case Material::TextureType::Height:            return "Height";
	case Material::TextureType::Custom0:           return "Custom0";
	case Material::TextureType::Custom1:           return "Custom1";
	case Material::TextureType::Custom2:           return "Custom2";
	case Material::TextureType::Custom3:           return "Custom3";
	default:                                       return "Unknown";
	}
}