// Texture.cpp
//
#include "Texture.h"

using namespace s2::Resources;

// -------------------------------------------------------------------------------------------------
TexturePtr Texture::New( const std::string& name,
                         int width,
                         int height,
                         int channels,
                         const uint8_t* data )
{
    return std::make_shared<Texture>( name, width, height, channels, data );
}

// -------------------------------------------------------------------------------------------------
TexturePtr Texture::New( const std::string& name,
                         const s2::Pixmap<uint8_t>& pixmap )
{
    return std::make_shared<Texture>( name, pixmap );
}

// -------------------------------------------------------------------------------------------------
Texture::Texture( const std::string& name,
                  int width,
                  int height,
                  int channels,
                  const uint8_t* data )
    : _name( name )
    , _data( width, height, channels, const_cast<uint8_t*>( data ) )
{
}

// -------------------------------------------------------------------------------------------------
Texture::Texture( const std::string& name,
                  const s2::Pixmap<uint8_t>& pixmap )
    : _name( name )
    , _data( pixmap )
{
}

// -------------------------------------------------------------------------------------------------
void Texture::setData( int width, int height, int channels, const uint8_t* pixels )
{
    _data.resize( width, height, channels );

    if( pixels )
        _data.setPixels( const_cast<uint8_t*>( pixels ) );
}

// -------------------------------------------------------------------------------------------------
void Texture::setData( const s2::Pixmap<uint8_t>& pixmap )
{
    _data = pixmap;
}

// -------------------------------------------------------------------------------------------------
void Texture::resize( int width, int height, int channels )
{
    _data.resize( width, height, channels );
}

// -------------------------------------------------------------------------------------------------
void Texture::clear()
{
    _data.clear();
}