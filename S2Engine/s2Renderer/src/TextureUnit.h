// TextureUnit.h
//
#ifndef TEXTUREUNIT_ONCE
#define TEXTUREUNIT_ONCE

#include "s2Renderer_API.h"

#include "Texture.h"
#include "Sampler.h"

#include <vector>

namespace s2 {
namespace Renderer {

// ------------------------------------------------------------------------------------------------
class S2RENDERER_API TextureUnit
{
public:
	TextureUnit( int index = -1);

	void setTexture( const Texture2DPtr &texture );
	void setSampler( const SamplerPtr &sampler );

	Texture2DPtr texture() const;
	SamplerPtr   sampler() const;

	void bind();

private:
	void validate();

private:
	enum Changes
	{
		None = 0,
		TextureChanged = 1,
		SamplerChanged = 2,
		Both = TextureChanged | SamplerChanged
	};

private:
	Texture2DPtr _texture;
	SamplerPtr   _sampler;

	int     _textureUnitIndex;
	int     _textureUnit;
	Changes _changes;
};


// ------------------------------------------------------------------------------------------------
class S2RENDERER_API TextureUnits
{
public:
	TextureUnits();

	//TextureUnit &unit( int index );
	TextureUnit &operator[] ( int index );

	void set()const;

	int count() const;

private:
	int                      _count;
	mutable std::vector<TextureUnit> _units;
};

}

}

#endif // !TEXTUREUNIT_ONCE
