// TextureUnit.cpp
//
#include "TextureUnit.h"

#include "OpenGL.h"
#include "Device.h"

using namespace s2::Renderer;

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
TextureUnits::TextureUnits()
{
	_count = Device::get().numberOfTextureUnits();
	_units = std::vector<TextureUnit>( _count );

	for( int i=0; i < _count; ++i )
		_units[i] = TextureUnit( i );
}

// ------------------------------------------------------------------------------------------------
TextureUnit &TextureUnits::operator[] ( int index ) { return _units.at( index ); }
int TextureUnits::count()                     const { return _count; }

// ------------------------------------------------------------------------------------------------
void TextureUnits::set() const
{
	for( auto &u : _units )
		u.bind();
}
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------
TextureUnit::TextureUnit( int index )
: _textureUnitIndex( index )
, _textureUnit( GL_TEXTURE0 + index )
, _changes( Changes::None )
{}

// ------------------------------------------------------------------------------------------------
Texture2DPtr TextureUnit::texture() const { return _texture; }
SamplerPtr   TextureUnit::sampler() const { return _sampler; }

// ------------------------------------------------------------------------------------------------
void TextureUnit::setTexture( const Texture2DPtr &texture )
{
	if( texture != _texture )
	{
		if( _changes == Changes::None )
		{
			// notify ?
		}

		_changes = Changes( _changes | Changes::TextureChanged );
		_texture = texture;
	}
}

// ------------------------------------------------------------------------------------------------
void TextureUnit::setSampler( const SamplerPtr &sampler )
{
	if( sampler != _sampler )
	{
		if( _changes == Changes::None )
		{
			// notify ?
		}

		_changes = Changes( _changes | Changes::SamplerChanged );
		_sampler = sampler;
	}
}


// ------------------------------------------------------------------------------------------------
void TextureUnit::bind()
{
	if( _changes != Changes::None )
	{
		validate();

		glActiveTexture( _textureUnit );

		if( ( _changes & Changes::TextureChanged ) == Changes::TextureChanged )
		{
			if( _texture )
			{
				_texture->bind();
			}
			else
			{
				Texture2D::unbindAll();
			}
		}

		if( ( _changes & Changes::SamplerChanged ) == Changes::SamplerChanged )
		{
			if( _sampler )
			{
				_sampler->bind( _textureUnitIndex );
			}
			else
			{
				Sampler::unbindAll( _textureUnitIndex );
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
	//	if (_sampler == null)
	//	{
	//		throw new InvalidOperationException("A texture sampler must be assigned to a texture unit with one or more bound textures.");
	//	}

	//	if (_texture.Target == TextureTarget.TextureRectangle)
	//	{
	//		if (_sampler.MinificationFilter != TextureMinificationFilter.Linear &&
	//			_sampler.MinificationFilter != TextureMinificationFilter.Nearest)
	//		{
	//			throw new InvalidOperationException("The texture sampler is incompatible with the rectangle texture bound to the same texture unit.  Rectangle textures only support linear and nearest minification filters.");
	//		}

	//		if (_sampler.WrapS == TextureWrap.Repeat ||
	//			_sampler.WrapS == TextureWrap.MirroredRepeat ||
	//			_sampler.WrapT == TextureWrap.Repeat ||
	//			_sampler.WrapT == TextureWrap.MirroredRepeat)
	//		{
	//			throw new InvalidOperationException("The texture sampler is incompatible with the rectangle texture bound to the same texture unit.  Rectangle textures do not support repeat or mirrored repeat wrap modes.");
	//		}
	//	}
	//}
}
