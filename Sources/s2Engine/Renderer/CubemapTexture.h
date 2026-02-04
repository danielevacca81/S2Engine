.\Sources\s2Engine\Renderer\CubemapTexture.h
// CubemapTexture.h
//
#ifndef RENDERER_CUBEMAP_TEXTURE_H
#define RENDERER_CUBEMAP_TEXTURE_H

#include "s2Engine_API.h"

#include "Texture.h"

#include <array>
#include <memory>
#include <string>

namespace Renderer {

/************************************************************************************************/
/*                                      CubemapTexture                                          */
/************************************************************************************************/
class CubemapTexture;
typedef std::shared_ptr<CubemapTexture> CubemapTexturePtr;

/**
 * CubemapTexture represents a cube map texture with 6 faces.
 * Each face is a separate Texture (Pixmap-based, OpenGL-agnostic).
 * Faces are ordered as: +X, -X, +Y, -Y, +Z, -Z (right, left, top, bottom, front, back)
 */
class S2ENGINE_API CubemapTexture
{
public:
	enum class Face
	{
		PositiveX = 0,  // Right
		NegativeX = 1,  // Left
		PositiveY = 2,  // Top
		NegativeY = 3,  // Bottom
		PositiveZ = 4,  // Front
		NegativeZ = 5   // Back
	};

public:
	static CubemapTexturePtr New( const std::string& name, int faceSize, int channels = 4 );
	static CubemapTexturePtr New( const std::string& name,
								  const std::array<s2::Pixmap<uint8_t>, 6>& faces );

public:
	CubemapTexture( const std::string& name, int faceSize, int channels = 4 );
	CubemapTexture( const std::string& name,
					const std::array<s2::Pixmap<uint8_t>, 6>& faces );
	~CubemapTexture() = default;

	// Getters
	std::string name() const { return _name; }
	int faceSize() const { return _faceSize; }
	int channels() const { return _channels; }

	// Face access
	TexturePtr getFace( Face face ) const;
	void setFace( Face face, TexturePtr texture );
	void setFace( Face face, const s2::Pixmap<uint8_t>& pixmap );

	// Validation
	bool isValid() const;
	bool hasFace( Face face ) const;

	// Utility
	static const char* faceToString( Face face );
	static Face stringToFace( const std::string& str );

private:
	void validateFaceSize( const s2::Pixmap<uint8_t>& pixmap ) const;

private:
	std::string _name;
	int _faceSize;
	int _channels;
	std::array<TexturePtr, 6> _faces;
};

}

#endif // RENDERER_CUBEMAP_TEXTURE_H