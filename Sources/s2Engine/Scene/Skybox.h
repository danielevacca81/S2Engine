// Skybox.h
//
#ifndef RENDERER_SKYBOX_H
#define RENDERER_SKYBOX_H

#include "s2Engine_API.h"

#include "Math/Math.h"

#include "Resources/CubemapTexture.h"
#include "Graphics/Color.h"

#include <memory>
#include <string>

namespace s2 {
namespace Scene {

/************************************************************************************************/
/*                                           Skybox                                             */
/************************************************************************************************/
class Skybox;
typedef std::shared_ptr<Skybox> SkyboxPtr;

/**
 * Skybox represents an environment background using either:
 * - A cubemap texture (6 faces)
 * - A solid color (procedural)
 * - A gradient (procedural)
 */
class S2ENGINE_API Skybox
{
public:
	enum class Type
	{
		None,        // No skybox
		SolidColor,  // Solid color background
		Gradient,    // Gradient background (top to bottom)
		Cubemap      // Cubemap texture
	};

public:
	static SkyboxPtr New( const std::string& name );
	static SkyboxPtr NewSolidColor( const std::string& name, const Color& color );
	static SkyboxPtr NewGradient( const std::string& name,
								  const Color& topColor,
								  const Color& bottomColor );
	static SkyboxPtr NewCubemap( const std::string& name, Resources::CubemapTexturePtr cubemap );

public:
	Skybox( const std::string& name );
	~Skybox() = default;

	// Identification
	std::string name() const { return _name; }
	void setName( const std::string& name ) { _name = name; }

	// Type
	Type type() const { return _type; }

	// Solid color setup
	void setSolidColor( const Color& color );
	Color solidColor() const { return _solidColor; }

	// Gradient setup
	void setGradient( const Color& topColor, const Color& bottomColor );
	Color topColor() const { return _topColor; }
	Color bottomColor() const { return _bottomColor; }

	// Cubemap setup
	void setCubemap( Resources::CubemapTexturePtr cubemap );
	Resources::CubemapTexturePtr cubemap() const { return _cubemap; }

	// Properties
	float intensity() const { return _intensity; }
	void setIntensity( float value ) { _intensity = Math::max( 0.0f, value ); }

	float rotation() const { return _rotation; }
	void setRotation( float degrees ) { _rotation = degrees; }

	// Validation
	bool isValid() const;

private:
	std::string _name;
	Type _type { Type::None };

	// Solid color properties
	Color _solidColor { Color::black() };

	// Gradient properties
	Color _topColor { Color::blue() };
	Color _bottomColor { Color::white() };

	// Cubemap properties
	Resources::CubemapTexturePtr _cubemap;

	// Common properties
	float _intensity { 1.0f };  // Brightness multiplier
	float _rotation { 0.0f };   // Y-axis rotation in degrees
};

}
}

#endif // !S2_SCENE_SKYBOX_H