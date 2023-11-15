// TextureUnit.h
//
#ifndef TEXTUREUNIT_ONCE
#define TEXTUREUNIT_ONCE

#include "RenderSystem_API.h"

#include "Texture.h"
#include "Sampler.h"

#include <vector>

namespace RenderSystem {

// ------------------------------------------------------------------------------------------------
class RENDERSYSTEM_API TextureUnit
{
public:
	explicit TextureUnit( int index = -1);

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

	int     _textureUnitIndex {-1};
	int     _textureUnit {-1};
	Changes _changes = {None};
};


// ------------------------------------------------------------------------------------------------
class RENDERSYSTEM_API TextureUnits
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
#endif // !TEXTUREUNIT_ONCE
