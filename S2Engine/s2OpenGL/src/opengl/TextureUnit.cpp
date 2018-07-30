// TextureUnit.cpp
//
#include "TextureUnit.h"

#include "OpenGL.h"

using namespace s2::OpenGL;

TextureUnit::TextureUnit(int index)
: _textureUnitIndex(index)
, _textureUnit(GL_TEXTURE0+index)
, _changes(Changes::None)
{}

// ------------------------------------------------------------------------------------------------
Texture2DPtr      TextureUnit::texture() const { return _texture; }
TextureSamplerPtr TextureUnit::sampler() const { return _textureSampler; }

// ------------------------------------------------------------------------------------------------
void TextureUnit::setTexture(const Texture2DPtr &texture)
{
	if (texture != _texture)
	{
		if (_changes == Changes::None)
		{
			// notify ?
		}

		_changes = Changes(_changes | Changes::Texture);
		_texture = texture;
	}
}

// ------------------------------------------------------------------------------------------------
void TextureUnit::setTextureSampler(const TextureSamplerPtr &sampler)
{
	if (sampler != _textureSampler)
	{
		if (_changes == Changes::None)
		{
			// notify ?
		}

		_changes = Changes(_changes | Changes::Sampler);
		_textureSampler = sampler;
	}
}


// ------------------------------------------------------------------------------------------------
void TextureUnit::use()
{
	if (_changes != Changes::None)
	{
		validate();

		glActiveTexture(_textureUnit);

		if ((_changes & Changes::Texture) == Changes::Texture)
		{
			if (_texture )
			{
				_texture->bind();
			}
			else
			{
				Texture2D::unbind();
			}
		}

		if ((_changes & Changes::Sampler) == Changes::Sampler)
		{
			if (_textureSampler)
			{
				_textureSampler->bind(_textureUnitIndex);
			}
			else
			{
				TextureSampler::unbind(_textureUnitIndex);
			}
		}

		_changes = Changes::None;
	}
}


// ------------------------------------------------------------------------------------------------
void TextureUnit::validate()
{
	//if (_texture != null)
	//{
	//	if (_textureSampler == null)
	//	{
	//		throw new InvalidOperationException("A texture sampler must be assigned to a texture unit with one or more bound textures.");
	//	}

	//	if (_texture.Target == TextureTarget.TextureRectangle)
	//	{
	//		if (_textureSampler.MinificationFilter != TextureMinificationFilter.Linear &&
	//			_textureSampler.MinificationFilter != TextureMinificationFilter.Nearest)
	//		{
	//			throw new InvalidOperationException("The texture sampler is incompatible with the rectangle texture bound to the same texture unit.  Rectangle textures only support linear and nearest minification filters.");
	//		}

	//		if (_textureSampler.WrapS == TextureWrap.Repeat ||
	//			_textureSampler.WrapS == TextureWrap.MirroredRepeat ||
	//			_textureSampler.WrapT == TextureWrap.Repeat ||
	//			_textureSampler.WrapT == TextureWrap.MirroredRepeat)
	//		{
	//			throw new InvalidOperationException("The texture sampler is incompatible with the rectangle texture bound to the same texture unit.  Rectangle textures do not support repeat or mirrored repeat wrap modes.");
	//		}
	//	}
	//}
}
