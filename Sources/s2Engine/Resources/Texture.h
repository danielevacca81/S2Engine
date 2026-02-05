// Texture.h
//
#ifndef RENDERER_TEXTURE_H
#define RENDERER_TEXTURE_H

#include "s2Engine_API.h"

#include "Graphics/Pixmap.h"

#include <memory>
#include <string>

namespace Renderer {

class Texture;
typedef std::shared_ptr<Texture> TexturePtr;

class S2ENGINE_API Texture
{
public:
	static TexturePtr New( const std::string& name,
						   int width,
						   int height,
						   int channels = 4,
						   const uint8_t* data = nullptr );

	static TexturePtr New( const std::string& name,
						   const s2::Pixmap<uint8_t>& pixmap );

public:
	Texture( const std::string& name,
			 int width,
			 int height,
			 int channels = 4,
			 const uint8_t* data = nullptr );

	Texture( const std::string& name,
			 const s2::Pixmap<uint8_t>& pixmap );

	~Texture() = default;

	// Getters
	std::string name()       const { return _name; }
	int width()              const { return _data.width(); }
	int height()             const { return _data.height(); }
	int channels()           const { return _data.numChannels(); }
	bool isEmpty()           const { return _data.isEmpty(); }

	const s2::Pixmap<uint8_t>& pixmap() const { return _data; }

	// Operations
	void setData( int width, int height, int channels, const uint8_t* pixels );
	void setData( const s2::Pixmap<uint8_t>& pixmap );
	void resize( int width, int height, int channels );
	void clear();

	// Pixel access
	const uint8_t* pixels() const { return _data.pixels(); }
	const uint8_t& operator[]( int i ) const { return _data[i]; }
	uint8_t& operator[]( int i ) { return _data[i]; }

private:
	std::string _name;
	s2::Pixmap<uint8_t> _data;
};

}

#endif // RENDERER_TEXTURE_H