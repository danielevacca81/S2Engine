// Material.h
//
#ifndef RENDERER_MATERIAL_H
#define RENDERER_MATERIAL_H

#include "s2Engine_API.h"

#include "Texture.h"
#include "Graphics/Color.h"

#include <memory>
#include <string>
#include <unordered_map>

namespace Renderer {

/************************************************************************************************/
/*                                          Material                                            */
/************************************************************************************************/
class Material;
typedef std::shared_ptr<Material> MaterialPtr;

/**
 * Material class encapsulates surface properties and textures.
 * Supports both simple color-based materials and texture-based materials.
 * Texture slots follow common PBR and traditional rendering conventions.
 */
class S2ENGINE_API Material
{
public:
	enum class TextureType
	{
		Diffuse,        // Base color / albedo texture
		Normal,         // Normal map for surface details
		Specular,       // Specular/reflectivity map
		Roughness,      // Roughness map (PBR)
		Metallic,       // Metallic map (PBR)
		AmbientOcclusion, // AO map
		Emissive,       // Emissive/glow map
		Opacity,        // Alpha/transparency map
		Height,         // Height/displacement map

		// Additional slots for flexibility
		Custom0,
		Custom1,
		Custom2,
		Custom3
	};

public:
	static MaterialPtr New( const std::string& name );
	static MaterialPtr New( const std::string& name, const Color& baseColor );

public:
	Material( const std::string& name );
	Material( const std::string& name, const Color& baseColor );
	~Material() = default;

	// Identification
	std::string name() const { return _name; }
	void setName( const std::string& name ) { _name = name; }

	// Color properties
	Color baseColor() const { return _baseColor; }
	void setBaseColor( const Color& color ) { _baseColor = color; }

	Color emissiveColor() const { return _emissiveColor; }
	void setEmissiveColor( const Color& color ) { _emissiveColor = color; }

	// Material properties
	float roughness() const { return _roughness; }
	void setRoughness( float value ) { _roughness = Math::clamp( value, 0.0f, 1.0f ); }

	float metallic() const { return _metallic; }
	void setMetallic( float value ) { _metallic = Math::clamp( value, 0.0f, 1.0f ); }

	float opacity() const { return _opacity; }
	void setOpacity( float value ) { _opacity = Math::clamp( value, 0.0f, 1.0f ); }

	// Texture management
	void setTexture( TextureType type, TexturePtr texture );
	TexturePtr getTexture( TextureType type ) const;
	bool hasTexture( TextureType type ) const;
	void removeTexture( TextureType type );
	void clearTextures();

	// Utility
	std::vector<TextureType> getUsedTextureSlots() const;
	size_t textureCount() const { return _textures.size(); }
	bool isTextured() const { return !_textures.empty(); }

	// Comparison
	bool operator==( const Material& other ) const;
	bool operator!=( const Material& other ) const { return !( *this == other ); }

private:
	std::string _name;

	// Color properties
	Color _baseColor { Color::white() };
	Color _emissiveColor { Color::black() };

	// Material properties
	float _roughness = 0.5f;
	float _metallic = 0.0f;
	float _opacity = 1.0f;

	// Texture slots
	std::unordered_map<TextureType, TexturePtr> _textures;
};

// Helper function to convert TextureType to string
S2ENGINE_API const char* textureTypeToString( Material::TextureType type );

}

#endif // RENDERER_MATERIAL_H